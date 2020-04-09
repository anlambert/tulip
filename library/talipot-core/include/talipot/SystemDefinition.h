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

#ifndef TALIPOT_SYSTEM_DEFINITION_H
#define TALIPOT_SYSTEM_DEFINITION_H

#if defined(_WIN32)
#define OS_PLATFORM "win"
#elif defined(__APPLE__)
#define OS_PLATFORM "mac"
#elif defined(__linux__)
#define OS_PLATFORM "linux"
#else
#define OS_PLATFORM "other"
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define OS_ARCHITECTURE "x86_64"
#elif defined(__i386__) || defined(_M_IX86)
#define OS_ARCHITECTURE "x86"
#else
#define OS_ARCHITECTURE "other"
#endif

#if defined(__clang__)
#define OS_COMPILER "Clang"
#elif defined(__GNUC__)
#define OS_COMPILER "gcc"
#elif defined(_MSC_VER)
#define OS_COMPILER "MSVC"
#endif

#endif // TALIPOT_SYSTEM_DEFINITION_H
