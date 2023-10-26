#ifndef _CASSERT_H
#define _CASSERT_H

#if !defined(NDEBUG) || defined(DEBUG) || defined(DEBUG_MODE) ||               \
  defined(_DEBUG) || defined(__DEBUG)
#define CASSERT_DEBUG
#endif

#ifdef CASSERT_NDEBUG
#ifdef CASSERT_DEBUG
#undef CASSERT_DEBUG
#endif
#endif

#ifdef CASSERT_DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define cassert_impl(EXPR, MESG, ...)                                          \
  do                                                                           \
  {                                                                            \
    if (!(EXPR))                                                               \
    {                                                                          \
      fprintf(stderr, "ERROR: %s:%d: ", __FILE__, __LINE__);                   \
      if (strlen(MESG) > 0)                                                    \
      {                                                                        \
        fprintf(stderr, "%s: ", MESG);                                         \
      }                                                                        \
      fprintf(stderr, #EXPR " != true");                                       \
      fprintf(stderr, "\n");                                                   \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

#else

#define cassert_impl(...)

#endif

#define cassert(...) cassert_impl(__VA_ARGS__, "")

#endif