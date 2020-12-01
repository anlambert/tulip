/**
 *
 * Copyright (C) 2019-2020  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <algorithm>
#include <cstring>
#include <ctime>
#include <string>
#include <sstream>
#include <clocale>
#include <cerrno>
#include <random>
#include <chrono>

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <utf8.h>
#include <codecvt>
#include <locale>
#ifdef _MSC_VER
#include <dbghelp.h>
#endif
#ifdef __GLIBCXX__
#include <ext/stdio_filebuf.h>
#endif
#else
#include <dirent.h>
#include <dlfcn.h>
#endif

#include <gzstream.h>
#include <zststream.h>

#include <talipot/Exception.h>
#include <talipot/TlpTools.h>
#include <talipot/Plugin.h>
#include <talipot/PluginLoader.h>
#include <talipot/PropertyTypes.h>
#include <talipot/Release.h>
#if defined(_OPENMP) && defined(__APPLE__)
#include <talipot/ParallelTools.h>
#endif

using namespace std;
using namespace tlp;

#ifndef __EMSCRIPTEN__
static const char *TALIPOT_PLUGINS_PATH_VARIABLE = "TLP_PLUGINS_PATH";
#endif

// the relative path (a string), from the install dir
// of the directory where the talipot libraries are installed
#define TALIPOT_INSTALL_LIBDIR_STR STRINGIFY(TALIPOT_INSTALL_LIBDIR)

string tlp::TalipotLibDir;
string tlp::TalipotPluginsPath;
string tlp::TalipotBitmapDir;
string tlp::TalipotShareDir;
#ifdef _WIN32
const char tlp::PATH_DELIMITER = ';';
#else
const char tlp::PATH_DELIMITER = ':';
#endif

#ifndef __EMSCRIPTEN__
// A function that retrieves the Talipot libraries directory based on
// the path of the loaded shared library libtalipot-core-X.Y.[dll, so, dylib]
extern "C" {
char *getTalipotLibDir(char *buf) {
  std::string talipotLibDir;
  std::string libTalipotName;

#ifdef _WIN32
#ifdef __MINGW32__
  libTalipotName =
      "libtalipot-core-" + getMajor(TALIPOT_VERSION) + "." + getMinor(TALIPOT_VERSION) + ".dll";
#else
  libTalipotName =
      "talipot-core-" + getMajor(TALIPOT_VERSION) + "_" + getMinor(TALIPOT_VERSION) + ".dll";
#endif
  HMODULE hmod = GetModuleHandle(libTalipotName.c_str());

  if (hmod != nullptr) {
    TCHAR szPath[512 + 1];
    DWORD dwLen = GetModuleFileName(hmod, szPath, 512);

    if (dwLen > 0) {
      std::string tmp = szPath;
      std::replace(tmp.begin(), tmp.end(), '\\', '/');
      talipotLibDir = tmp.substr(0, tmp.rfind('/') + 1) + "../" + TALIPOT_INSTALL_LIBDIR_STR;
    }
  }

#else
#ifdef __APPLE__
  libTalipotName =
      "libtalipot-core-" + getMajor(TALIPOT_VERSION) + "." + getMinor(TALIPOT_VERSION) + ".dylib";
#else
  libTalipotName =
      "libtalipot-core-" + getMajor(TALIPOT_VERSION) + "." + getMinor(TALIPOT_VERSION) + ".so";
#endif
  void *ptr = dlopen(libTalipotName.c_str(), RTLD_LAZY);

  if (ptr != nullptr) {
    void *symbol = dlsym(ptr, "getTalipotLibDir");

    if (symbol != nullptr) {
      Dl_info info;
      if (dladdr(symbol, &info)) {
        std::string tmp = info.dli_fname;
        talipotLibDir = tmp.substr(0, tmp.rfind('/') + 1);
        talipotLibDir.append("../").append(TALIPOT_INSTALL_LIBDIR_STR);
      }
    }
    dlclose(ptr);
  }

#endif
  return strcpy(buf, talipotLibDir.c_str());
}
}

// throw an exception if an expected directory does not exist
static void checkDirectory(std::string dir, bool tlpDirSet, bool throwEx) {
  // remove ending / separator if any
  // bug detected on Windows
  if (dir[dir.length() - 1] == '/') {
    dir.erase(dir.length() - 1);
  }

  if (!pathExists(dir)) {
    std::stringstream ess;
    ess << "Error - " << dir << ":" << std::endl << strerror(errno) << std::endl;
    if (tlpDirSet) {
      ess << std::endl << "Check your TLP_DIR environment variable";
    }
    if (throwEx) {
      throw Exception(ess.str());
    } else if ( // output only if not in a python installed wheel
        (dir.find("/talipot/native/") == string::npos) &&
        // and not building talipot-core bindings
        (dir.find("library/talipot-core/src") == string::npos)) {
      tlp::error() << ess.str();
    }
  }
}

//=========================================================
void tlp::initTalipotLib(const char *appDirPath) {
  if (!TalipotShareDir.empty()) { // already initialized
    return;
  }

  char *getEnvTlp;
  string::size_type pos;
  // we use curDir to ease debugging
  // Talipot..Dir global variables may be invisible
  // when using gdb
  std::string curDir;

  getEnvTlp = getenv("TLP_DIR");

  if (getEnvTlp == nullptr) {
    if (appDirPath) {
#ifdef _WIN32
      curDir = std::string(appDirPath) + "/../" + TALIPOT_INSTALL_LIBDIR_STR;
#else
      // one dir up to initialize the lib dir
      curDir.append(appDirPath, strlen(appDirPath) - strlen(strrchr(appDirPath, '/') + 1));
      curDir.append(TALIPOT_INSTALL_LIBDIR_STR);

#endif
    } else {
      char buf[1024];
      // if no appDirPath is provided, retrieve dynamically the Talipot lib dir
      curDir = getTalipotLibDir(buf);
    }
  } else {
    curDir = string(getEnvTlp);
  }

#ifdef _WIN32
  // ensure it is a unix-style path
  pos = curDir.find('\\', 0);

  while (pos != string::npos) {
    curDir[pos] = '/';
    pos = curDir.find('\\', pos);
  }

#endif

  // ensure it is '/' terminated
  if (curDir[curDir.length() - 1] != '/') {
    curDir += '/';
  }

  bool tlpDirSet = (getEnvTlp != nullptr);
  bool throwExOnCheck = appDirPath != nullptr;

  TalipotLibDir = curDir;
  if (!TalipotLibDir.empty()) {
    checkDirectory(TalipotLibDir, tlpDirSet, throwExOnCheck);
  }

  getEnvTlp = getenv(TALIPOT_PLUGINS_PATH_VARIABLE);

  if (getEnvTlp != nullptr) {
    curDir = string(getEnvTlp);
#ifdef _WIN32
    // ensure it is a unix-style path
    pos = curDir.find('\\', 0);

    while (pos != string::npos) {
      curDir[pos] = '/';
      pos = curDir.find('\\', pos);
    }

#endif
    curDir = TalipotLibDir + "talipot" + PATH_DELIMITER + curDir;
  } else {
    curDir = TalipotLibDir + "talipot";
  }
  TalipotPluginsPath = curDir;

  // one dir up to initialize the share dir
  pos = TalipotLibDir.length() - 2;
  pos = TalipotLibDir.rfind("/", pos);
  curDir = TalipotLibDir.substr(0, pos + 1) + "share/talipot/";

#ifndef _WIN32
  // special case for Debian when Talipot install prefix is /usr
  // as libraries are installed in <prefix>/lib/<arch>
  if (!pathExists(curDir) != 0) {
    pos = TalipotLibDir.rfind("/", pos - 1);
    curDir = TalipotLibDir.substr(0, pos + 1) + "share/talipot/";
  }
#endif

#if defined(_OPENMP) && defined(__APPLE__)
  // Register an exit handler to prevent using OpenMP locks
  // when application shutdowns as some crashes have been observed
  // on some MacOS runtimes (10.12 for instance)
  OpenMPLock::registerExitHandler();
#endif

  TalipotShareDir = curDir;
  if (!TalipotLibDir.empty()) {
    checkDirectory(TalipotShareDir, tlpDirSet, throwExOnCheck);
  }

  TalipotBitmapDir = TalipotShareDir + "bitmaps/";

  if (!TalipotLibDir.empty()) {
    checkDirectory(TalipotBitmapDir, tlpDirSet, throwExOnCheck);
  }

  // initialize serializers
  initTypeSerializers();

  // initialize pseudo random number generator seed
  initRandomSequence();
}
//=========================================================

// tlp class names demangler
#if defined(__GNUC__)
#include <cxxabi.h>
std::string tlp::demangleClassName(const char *className, bool hideTlp) {
  static char demangleBuffer[1024];
  int status;
  size_t length = 1024;
  abi::__cxa_demangle(className, demangleBuffer, &length, &status);

  // skip tlp::
  if (hideTlp && strstr(demangleBuffer, "tlp::") == demangleBuffer) {
    return std::string(demangleBuffer + 5);
  }

  return std::string(demangleBuffer);
}
#elif defined(_MSC_VER)
// With Visual Studio, typeid(tlp::T).name() does not return a mangled type name
// but a human readable type name in the form "class tlp::T"
// so just remove the first 11 characters to return T
std::string tlp::demangleClassName(const char *className, bool hideTlp) {
  char *clName = const_cast<char *>(className);

  if (strstr(className, "class ") == className)
    clName += 6;

  if (hideTlp && strstr(clName, "tlp::") == clName)
    return std::string(clName + 5);

  return std::string(clName);
}
#else
#error define symbols demangling function
#endif

#else // __EMSCRIPTEN__

void initTalipotLib(const char *) {}

std::string tlp::demangleClassName(const char *className, bool) {
  return std::string(className);
}

#endif // __EMSCRIPTEN__

#ifdef _WIN32
static std::wstring u16stringToWstring(const std::u16string &s) {
  static std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, little_endian>, wchar_t> conv;
  return conv.from_bytes(reinterpret_cast<const char *>(&s[0]),
                         reinterpret_cast<const char *>(&s[0] + s.size()));
}
#endif

// random sequence management
//=========================================================

static unsigned int randomSeed = UINT_MAX;
// uniformly-distributed integer random number generator that produces non-deterministic random
// numbers
static std::random_device rd;
// Mersenne Twister pseudo-random generator of 32-bit numbers
static std::mt19937 mt;

void tlp::setSeedOfRandomSequence(unsigned int seed) {
  randomSeed = seed;
}

unsigned int tlp::getSeedOfRandomSequence() {
  return randomSeed;
}

void tlp::initRandomSequence() {
  // init seed from random sequence with std::random_device
  if (randomSeed == UINT_MAX) {
#ifndef __MINGW32__
    mt.seed(rd());
#else
    // std::random_device implementation is deterministic in MinGW so initialize seed with current
    // time (microsecond precision)
    mt.seed(uint(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
#endif
  } else {
    mt.seed(randomSeed);
  }
}

int tlp::randomInteger(int max) {
  if (max == 0) {
    return 0;
  } else if (max > 0) {
    std::uniform_int_distribution<int> dist(0, max);
    return dist(mt);
  } else {
    std::uniform_int_distribution<int> dist(max, 0);
    return dist(mt);
  }
}

unsigned int tlp::randomUnsignedInteger(unsigned int max) {
  if (max == 0) {
    return 0;
  } else {
    std::uniform_int_distribution<unsigned int> dist(0, max);
    return dist(mt);
  }
}

double tlp::randomDouble(double max) {
  std::uniform_real_distribution<double> dist(0, std::nextafter(max, DBL_MAX));
  return dist(mt);
}

// files management
//=========================================================

int tlp::statPath(const std::string &pathname, tlp_stat_t *buf) {
#ifndef WIN32
  return stat(pathname.c_str(), buf);
#else
  std::wstring utf16pathname = u16stringToWstring(utf8::utf8to16(pathname));
  return _wstat(utf16pathname.c_str(), buf);
#endif
}

bool tlp::pathExists(const std::string &pathname) {
  tlp_stat_t infoEntry;
  return statPath(pathname, &infoEntry) == 0;
}

// file streams management
//=========================================================

#if defined(WIN32) && defined(__GLIBCXX__)
// function to get the string representation of the bitwise combination of std::ios_base::openmode
// flags
static std::wstring openmodeToWString(std::ios_base::openmode mode) {
  std::wstring ret;
  bool testb = (mode & std::ios_base::binary) == std::ios_base::binary;
  bool testi = (mode & std::ios_base::in) == std::ios_base::in;
  bool testo = (mode & std::ios_base::out) == std::ios_base::out;
  bool testt = (mode & std::ios_base::trunc) == std::ios_base::trunc;
  bool testa = (mode & std::ios_base::app) == std::ios_base::app;

  if (!testi && testo && !testt && !testa)
    ret = L"w";

  if (!testi && testo && !testt && testa)
    ret = L"a";

  if (!testi && testo && testt && !testa)
    ret = L"w";

  if (testi && !testo && !testt && !testa)
    ret = L"r";

  if (testi && testo && !testt && !testa)
    ret = L"r+";

  if (testi && testo && testt && !testa)
    ret = L"w+";

  if (testb)
    ret += L"b";

  return ret;
}

// class to open a file for reading whose path contains non ascii characters (MinGW only)
class wifilestream : public std::istream {
public:
  wifilestream(const std::wstring &wfilename, std::ios_base::openmode mode)
      : fp(nullptr), buffer(nullptr) {
    fp = _wfopen(wfilename.c_str(), openmodeToWString(mode).c_str());

    if (fp) {
      buffer = new __gnu_cxx::stdio_filebuf<char>(fp, mode);
    }

    init(buffer);
  }
  ~wifilestream() {
    delete buffer;

    if (fp) {
      fclose(fp);
    }
  }

private:
  FILE *fp;
  __gnu_cxx::stdio_filebuf<char> *buffer;
};

// class to open a file for writing whose path contains non ascii characters (MinGW only)
class wofilestream : public std::ostream {
public:
  wofilestream(const std::wstring &wfilename, std::ios_base::openmode open_mode)
      : fp(nullptr), buffer(nullptr) {
    fp = _wfopen(wfilename.c_str(), openmodeToWString(open_mode).c_str());

    if (fp) {
      buffer = new __gnu_cxx::stdio_filebuf<char>(fp, open_mode);
    }

    init(buffer);
  }
  ~wofilestream() {
    delete buffer;

    if (fp) {
      fclose(fp);
    }
  }

private:
  FILE *fp;
  __gnu_cxx::stdio_filebuf<char> *buffer;
};
#endif

//=========================================================

std::istream *tlp::getInputFileStream(const std::string &filename, std::ios_base::openmode mode) {
#ifndef WIN32
  // On Linux and Mac OS, UTF-8 encoded paths are supported by std::ifstream
  return new std::ifstream(filename.c_str(), mode);
#else
  // On Windows, the path name (possibly containing non ascii characters) has to be converted to
  // UTF-16 in order to open a stream
  std::wstring utf16filename = u16stringToWstring(utf8::utf8to16(filename));
#ifdef __GLIBCXX__
  // With MinGW, it's a little bit tricky to get an input stream
  return new wifilestream(utf16filename, mode);
#elif defined(_MSC_VER)
  // Visual Studio has wide char version of std::ifstream
  return new std::ifstream(utf16filename.c_str(), mode);
#else
  // Fallback
  return new std::ifstream(filename.c_str(), mode);
#endif
#endif
}

//=========================================================

std::ostream *tlp::getOutputFileStream(const std::string &filename, std::ios_base::openmode mode) {
#ifndef WIN32
  // On Linux and Mac OS, UTF-8 encoded paths are supported by std::ofstream
  return new std::ofstream(filename.c_str(), mode);
#else
  // On Windows, the path name (possibly containing non ascii characters) has to be converted to
  // UTF-16 in order to open a stream
  std::wstring utf16filename = u16stringToWstring(utf8::utf8to16(filename));
#ifdef __GLIBCXX__
  // With MinGW, it's a little bit tricky to get an output stream
  return new wofilestream(utf16filename, mode);
#elif defined(_MSC_VER)
  // Visual Studio has wide char version of std::ofstream
  return new std::ofstream(utf16filename.c_str(), mode);
#else
  // Fallback
  return new std::ofstream(filename.c_str(), mode);
#endif
#endif
}

//=========================================================

std::istream *tlp::getZlibInputFileStream(const std::string &filename) {
#if defined(WIN32) && ZLIB_VERNUM >= 0x1270
  std::wstring utf16filename = u16stringToWstring(utf8::utf8to16(filename));
  return new igzstream(utf16filename.c_str(), ios::in | ios::binary);
#else
  return new igzstream(filename.c_str(), ios::in | ios::binary);
#endif
}

//=========================================================

std::ostream *tlp::getZlibOutputFileStream(const std::string &filename) {
#if defined(WIN32) && ZLIB_VERNUM >= 0x1270
  std::wstring utf16filename = u16stringToWstring(utf8::utf8to16(filename));
  return new ogzstream(utf16filename.c_str(), ios::out | ios::binary);
#else
  return new ogzstream(filename.c_str(), ios::out | ios::binary);
#endif
}

//=========================================================

std::istream *tlp::getZstdInputFileStream(const std::string &filename) {
  return new zstd::ZstdIStream(getInputFileStream(filename));
}

//=========================================================

std::ostream *tlp::getZstdOutputFileStream(const std::string &filename, int compressionLevel) {
  return new zstd::ZstdOStream(getOutputFileStream(filename), compressionLevel);
}