/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#ifndef UNIX_SIGNAL_INTERPOSER_H
#define UNIX_SIGNAL_INTERPOSER_H

#include <csignal>

typedef void SignalHandlerFunc(int);
typedef void SigactionHandlerFunc(int, siginfo_t *, void *);

extern void installSignalHandler(int sig, SignalHandlerFunc *handler);

extern void installSignalHandler(int sig, SigactionHandlerFunc *handler);

#endif // UNIX_SIGNAL_INTERPOSER_H
