/**
 *
 * Copyright (C) 2019-2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include "IdManagerTest.h"

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION(IdManagerTest);

//==========================================================
void IdManagerTest::testFragmentation() {
  for (unsigned int i = 0; i < 1000; ++i) {
    CPPUNIT_ASSERT_EQUAL(i, idManager->get());
  }

  for (unsigned int i = 1; i < 100; ++i) {
    idManager->free(i);
  }

  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(99), idManager->state.freeIds.size());
  idManager->free(0);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), idManager->state.freeIds.size());

  for (unsigned int i = 900; i < 999; ++i) {
    idManager->free(i);
  }

  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(99), idManager->state.freeIds.size());
  idManager->free(999);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(100), idManager->state.freeIds.size());
}
//==========================================================
void IdManagerTest::testGetFree() {
  for (unsigned int i = 0; i < 1000; ++i) {
    CPPUNIT_ASSERT_EQUAL(i, idManager->get());
  }

  for (unsigned int i = 0; i < 500; ++i) {
    idManager->free(i * 2);
  }

  for (unsigned int i = 0; i < 500; ++i) {
#ifdef TLP_NO_IDS_REUSE
    CPPUNIT_ASSERT_EQUAL(++maxId, idManager->get());
#else
    CPPUNIT_ASSERT_EQUAL(i * 2, idManager->get());
#endif
  }

  for (unsigned int i = 100; i <= 200; ++i) {
    idManager->free(i);
  }

  for (unsigned int i = 100; i <= 200; ++i) {
#ifdef TLP_NO_IDS_REUSE
    CPPUNIT_ASSERT_EQUAL(++maxId, idManager->get());
#else
    CPPUNIT_ASSERT_EQUAL(i, idManager->get());
#endif
  }
}
//==========================================================
void IdManagerTest::testIsFree() {
  for (unsigned int i = 0; i < 1000; ++i) {
    idManager->get();
  }

  for (unsigned int i = 0; i < 500; ++i) {
    idManager->free(i * 2);
  }

  for (unsigned int i = 0; i < 500; ++i) {
    CPPUNIT_ASSERT(idManager->is_free(i * 2));
    CPPUNIT_ASSERT(!idManager->is_free(i * 2 + 1));
  }

  CPPUNIT_ASSERT(idManager->is_free(1200));
}
//==========================================================
void IdManagerTest::testIterate() {
  for (unsigned int i = 0; i < 1000; ++i) {
    idManager->get();
  }

  unsigned int id = 0;

  for (unsigned int itId : idManager->getIds<unsigned int>()) {
    CPPUNIT_ASSERT_EQUAL(id, itId);
    ++id;
  }

  for (unsigned int i = 0; i < 500; ++i) {
    idManager->free(i * 2);
  }

  id = 0;

  for (unsigned int itId : idManager->getIds<unsigned int>()) {
    CPPUNIT_ASSERT_EQUAL(2u * id + 1u, itId);
    ++id;
  }

  for (unsigned int i = 0; i < 500; ++i) {
    CPPUNIT_ASSERT(idManager->is_free(i * 2));
    CPPUNIT_ASSERT(!idManager->is_free(i * 2 + 1));
  }

  CPPUNIT_ASSERT(idManager->is_free(1200));
}
