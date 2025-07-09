#pragma once
#include "../include/templa.hpp"

#define TEMPLA_TEST_INSTANCE 1
#define TEMPLA_TEST_ALL 1

#if TEMPLA_TEST_ALL
#define TEMPLA_TEST_UNIQUE 1
#define TEMPLA_TEST_CONCAT 1
#define TEMPLA_TEST_JOIN 1
#define TEMPLA_TEST_MAX 1
#define TEMPLA_TEST_MIN 1
#define TEMPLA_TEST_REVERSE 1
#define TEMPLA_TEST_CTTI 1
#define TEMPLA_TEST_TYPE_LIST 1
#else
#ifndef TEMPLA_TEST_UNIQUE
#define TEMPLA_TEST_UNIQUE 0
#endif
#ifndef TEMPLA_TEST_CONCAT
#define TEMPLA_TEST_CONCAT 0
#endif
#ifndef TEMPLA_TEST_JOIN
#define TEMPLA_TEST_JOIN 0
#endif
#ifndef TEMPLA_TEST_MAX
#define TEMPLA_TEST_MAX 0
#endif
#ifndef TEMPLA_TEST_MIN
#define TEMPLA_TEST_MIN 0
#endif
#ifndef TEMPLA_TEST_REVERSE
#define TEMPLA_TEST_REVERSE 0
#endif
#ifndef TEMPLA_TEST_CTTI
#define TEMPLA_TEST_CTTI 0
#endif
#ifndef TEMPLA_TEST_TYPE_LIST
#define TEMPLA_TEST_TYPE_LIST 0
#endif
#endif
