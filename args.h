#ifndef _ARGS_H
#define _ARGS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ARGS_MAX_ARGC
#define ARGS_MAX_ARGC 255
#endif

#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define ARG_PATHSEP '\\'
#endif
#define strdup _strdup
#else
#define ARG_PATHSEP '/'
#endif

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
const bool __args_win32 = true;
const bool __args_posix = false;
#else
const bool __args_win32 = false;
const bool __args_posix = true;
#endif

#ifdef bool
#undef bool
typedef _Bool bool;
#endif

#ifndef str
typedef char *str;
#endif

#define ARGVAR(TYPE, VAR) TYPE VAR

#define ARGTYPES_X_LIST                                                        \
  X(bool)                                                                      \
  X(char)                                                                      \
  X(short)                                                                     \
  X(int)                                                                       \
  X(long)                                                                      \
  X(float)                                                                     \
  X(double)                                                                    \
  X(str)

typedef enum argtype
{
#define X(TYPE) TYPE##arg,
  ARGTYPES_X_LIST
#undef X
} argtype;

const char *argtypenames[] = {
#define X(TYPE) #TYPE,
  ARGTYPES_X_LIST
#undef X
};

static struct
{
  const char *name; // argument flags/name
  argtype type;     // argument messages
  const char *mesg; // argument messages
  void **var;       // argument variables (from macro)
  void *def;        // argument defaults (from macro)
  bool used;        // argument accessed
  bool flag;        // argument is flag
  bool reqd;        // argument is required
  bool sing;        // single-letter
} __args[ARGS_MAX_ARGC];
char *__exename; // executable name
int __argscount;

static inline bool arggetboolarg(char *arg)
{
  if (!arg)
  {
    return false;
  }
  for (int i = 0; arg[i]; i++)
  {
    arg[i] = tolower((unsigned char) arg[i]);
  }
  return strcmp(arg, "yes") == 0 || strcmp(arg, "true") == 0 ||
         strcmp(arg, "okay") == 0 || strncmp(arg, "ok", 2) == 0 ||
         strncmp(arg, "y", 1) == 0 || strncmp(arg, "t", 1) == 0 ||
         strncmp(arg, "1", 1) == 0;
}

static inline char arggetchararg(char *arg)
{
  return arg ? arg[0] : 0;
}

static inline short arggetshortarg(char *arg)
{
  return (short) atoi(arg);
}

static inline int arggetintarg(char *arg)
{
  return atoi(arg);
}

static inline long arggetlongarg(char *arg)
{
  return atol(arg);
}

static inline float arggetfloatarg(char *arg)
{
  return (float) atof(arg);
}

static inline double arggetdoublearg(char *arg)
{
  return atof(arg);
}

static inline str arggetstrarg(char *arg)
{
  return arg;
}

static inline void argsetdef(void **var, void *def)
{
  *var = def;
}

static inline void argsetvar(void **var, argtype type, char *arg)
{
  switch (type)
  {
#define X(TYPE)                                                                \
  case TYPE##arg:                                                              \
    *((TYPE *) var) = argget##TYPE##arg(arg);                                  \
    break;
    ARGTYPES_X_LIST
#undef X
    default:
      break;
  }
}

static inline void argprinthelp(FILE *const stream)
{
  fprintf(stream, "usage: %s", __exename);
  for (int i = 0; i < __argscount; ++i)
  {
    if (!__args[i].reqd)
    {
      continue;
    }
    fprintf(stream, " -%s", __args[i].name);
    if (!__args[i].flag)
    {
      fprintf(stream, " (%s)", argtypenames[__args[i].type]);
    }
  }
  fprintf(stream, " [options] ...\n");
  fprintf(stream, "  required:\n");
  for (int i = 0; i < __argscount; ++i)
  {
    if (!__args[i].reqd)
    {
      continue;
    }
    fprintf(stream, "    -%s", __args[i].name);
    if (!__args[i].flag)
    {
      fprintf(stream, " (%s)", argtypenames[__args[i].type]);
    }
    if (__args[i].mesg)
    {
      fprintf(stream, ": %s", __args[i].mesg);
    }
    fprintf(stream, "\n");
  }
  fprintf(stream, "  options:\n");
  for (int i = 0; i < __argscount; ++i)
  {
    if (__args[i].reqd)
    {
      continue;
    }
    fprintf(stream, "    -%s", __args[i].name);
    if (!__args[i].flag)
    {
      fprintf(stream, " (%s)", argtypenames[__args[i].type]);
    }
    if (__args[i].mesg)
    {
      fprintf(stream, ": %s", __args[i].mesg);
    }
    fprintf(stream, "\n");
  }
}

#define argdefaults() argflag(help, "Show this help message and quit", true)

#define argvar(...) _argvar(__VA_ARGS__, NULL, NULL)
#define _argvar(VAR, TYPE, MESSAGE, DEFAULT, ...)                              \
  const int __arg_##VAR##_i = __argscount;                                     \
  TYPE VAR;                                                                    \
  do                                                                           \
  {                                                                            \
    __args[__argscount].name = #VAR;                                           \
    __args[__argscount].type = TYPE##arg;                                      \
    __args[__argscount].mesg = MESSAGE;                                        \
    __args[__argscount].var  = (void *) &(VAR);                                \
    __args[__argscount].def  = (void *) DEFAULT;                               \
    __args[__argscount].used = false;                                          \
    __args[__argscount].flag = false;                                          \
    __args[__argscount].reqd = false;                                          \
    __args[__argscount].sing = strlen(#VAR) == 1;                              \
    __argscount++;                                                             \
  } while (0)

#define argflag(VAR, ...)                                                      \
  argvar(VAR, bool, __VA_ARGS__, NULL, NULL);                                  \
  do                                                                           \
  {                                                                            \
    __args[__arg_##VAR##_i].flag = true;                                       \
  } while (0)

#define argreqd(VAR)                                                           \
  do                                                                           \
  {                                                                            \
    __args[__arg_##VAR##_i].reqd = true;                                       \
  } while (0)

#define argparse(ARGC, ARGV)                                                   \
  do                                                                           \
  {                                                                            \
    const char *appnstart = strrchr(ARGV[0], ARG_PATHSEP);                     \
    if (appnstart)                                                             \
    {                                                                          \
      appnstart += 1;                                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      appnstart = ARGV[0];                                                     \
    }                                                                          \
    char *extstart = strrchr(appnstart, '.');                                  \
    if (__args_win32 || extstart == NULL)                                      \
    {                                                                          \
      extstart = ARGV[0] + strlen(ARGV[0]);                                    \
    }                                                                          \
    size_t len = extstart - appnstart;                                         \
    __exename  = (char *) malloc(len + 1);                                     \
    strncpy(__exename, appnstart, len);                                        \
    __exename[len] = '\0';                                                     \
    for (int i = 1; i < ARGC; ++i)                                             \
    {                                                                          \
      char *arg = ARGV[i];                                                     \
      if (arg && arg[0] == '-')                                                \
      {                                                                        \
        bool found = false;                                                    \
        char *argn = &(arg)[1];                                                \
        for (int j = 0; j < __argscount; ++j)                                  \
        {                                                                      \
          int namen = strlen(__args[j].name);                                  \
          if (strncmp(__args[j].name, argn, namen) == 0)                       \
          {                                                                    \
            if (__args[j].flag)                                                \
            {                                                                  \
              argsetdef(__args[j].var, (void *) 1);                            \
            }                                                                  \
            else                                                               \
            {                                                                  \
              int argnn = strlen(argn);                                        \
              if (argnn > namen && argn[namen] == '=')                         \
              {                                                                \
                argsetvar(__args[j].var, __args[j].type, &(argn)[namen + 1]);  \
              }                                                                \
              else                                                             \
              {                                                                \
                argsetvar(__args[j].var, __args[j].type, ARGV[++i]);           \
              }                                                                \
            }                                                                  \
            __args[j].used = true;                                             \
            found          = true;                                             \
            break;                                                             \
          }                                                                    \
        }                                                                      \
        if (!found)                                                            \
        {                                                                      \
          fprintf(stderr, "ERROR: Unknown argument '%s'\n", argn);             \
          argprinthelp(stderr);                                                \
          exit(1);                                                             \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    for (int j = 0; j < __argscount; ++j)                                      \
    {                                                                          \
      if (__args[j].type == boolarg && strcmp(__args[j].name, "help") == 0 &&  \
          *((bool *) __args[j].var) == true)                                   \
      {                                                                        \
        argprinthelp(stdout);                                                  \
        exit(0);                                                               \
      }                                                                        \
      if (!__args[j].used)                                                     \
      {                                                                        \
        if (__args[j].reqd)                                                    \
        {                                                                      \
          fprintf(stderr,                                                      \
                  "ERROR: Missing required argument '%s'\n",                   \
                  __args[j].name);                                             \
          argprinthelp(stderr);                                                \
          exit(1);                                                             \
        }                                                                      \
        else                                                                   \
        {                                                                      \
          argsetdef(__args[j].var, __args[j].def);                             \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif