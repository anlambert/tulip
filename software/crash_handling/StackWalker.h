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

#ifndef STACK_WALKER_H
#define STACK_WALKER_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>

#include <talipot/ConsoleUtils.h>

#ifndef _MSC_VER
#ifdef HAVE_BFD
#include "BfdWrapper.h"
#endif
#else
typedef __int64 int64_t;
#endif

class StackWalker {
public:
  virtual ~StackWalker() {}
  virtual void printCallStack(std::ostream &os, unsigned int maxDepth = 50) = 0;

  void printCallStackToStdErr(unsigned int maxDepth = 50) {
    printCallStack(std::cerr, maxDepth);
  }

  virtual void printFrameInfo(std::ostream &os, unsigned int frameId, int64_t pcAddress,
                              const std::string &moduleName, const std::string &funcName = "",
                              int64_t symbolOffset = 0, const std::string &fileName = "",
                              unsigned int line = 0) {

    if (frameId % 2 == 0) {
      os << setTextBackgroundColor(DARK_GRAY);
    } else {
      os << setTextBackgroundColor(LIGHT_GRAY);
    }

    os << bold;

    os << lightRed << std::dec << std::setfill('0') << "#" << std::setw(2) << frameId
       << lightMagenta << " 0x" << std::hex << std::setw(16) << pcAddress << lightRed << " in ";

    os << white;

    if (!funcName.empty()) {
      os << funcName;
    } else {
      os << "??";
    }

    if (symbolOffset != 0) {
      os << lightMagenta << " (+0x" << std::hex << symbolOffset << ")";
    }

    if (!fileName.empty()) {
      os << lightRed << " at " << lightGreen << fileName << ":" << std::dec << lightYellow << line;
    }

    if (!moduleName.empty()) {
      os << lightRed << " from " << lightCyan << moduleName;
    } else {
      os << lightRed << " from " << lightGreen << "??";
    }

    os << fillToEndOfLine << defaultTextColor << std::endl;
  }
};

#if defined(__unix__) || defined(__APPLE__)

#include <map>

class StackWalkerGCC : public StackWalker {

public:
  StackWalkerGCC();

  ~StackWalkerGCC() override;

  void printCallStack(std::ostream &os, unsigned int maxDepth = 50) override;

  void setCallerAddress(void *callerAddress) {
    this->callerAddress = callerAddress;
  }

private:
  void *callerAddress;
#ifdef HAVE_BFD
  std::map<std::string, BfdWrapper *> bfdMap;
#endif
};

#elif defined(__MINGW32__)

#include <map>

class StackWalkerMinGW : public StackWalker {
public:
  StackWalkerMinGW();
  ~StackWalkerMinGW();

  void printCallStack(std::ostream &os, unsigned int maxDepth = 50);

  void setContext(LPCONTEXT context) {
    this->context = context;
  }

private:
  LPCONTEXT context;
#ifdef HAVE_BFD
  std::map<std::string, BfdWrapper *> bfdMap;
#endif
};

#elif defined(_MSC_VER)

class StackWalkerMSVC : public StackWalker {
public:
  StackWalkerMSVC();
  ~StackWalkerMSVC();

  void printCallStack(std::ostream &os, unsigned int maxDepth = 50);
  void setExtraSymbolsSearchPaths(const std::string &searchPaths) {
    extraSymbolsSearchPaths = searchPaths;
  }
  void setContext(CONTEXT *context) {
    this->context = context;
  }

private:
  CONTEXT *context;
  std::string extraSymbolsSearchPaths;
};

#endif

#endif // STACK_WALKER_H
