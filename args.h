/*
# args.h - argument parser

## Usage

Include `args.h` and you're set.

## API Documentation

- `args_default()`: Sets up the default help message flag
- `args_var(Name, Type, [Message], [Default])`: Sets up a variable for its value
to be parsed into as an option. This will either be a single letter flag like
-a, or a long flag like -asdf, --asdf
- `args_flag(Name, [Message], [Default])`: Sets up a bool variable to be parsed
into. This kind of flag's value is set by providing the flag itself.
- `args_reqd(Name)`: Marks a flag in the arguments as required.
- `args_parse(Argc, Argv):` Parses out the arguments.
- `args_pop(Name, Type, [Message], [Default])`: Pops an argument off after
parsing the options. These are required.
- `args_printhelp(FILE *const)`: Prints the help message to the specified
stream.

## Notes

- `args_parse` will detect the help flag being true and print the help message
for you, if the name is "help", and it's a bool flag set to `true`.
- This supports -flag value and -flag=value syntax
- Compound single-letter flags like -abc are supported as long as all of the
single-letter flags exist. Else, the whole flag will be treated as invalid.
- `args_printhelp` is based off of the existing values. If you call it before
`args_pop`, which can be called after `args_parse`, then it will not contain
those arguments.
*/
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

typedef struct TYPEARG
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
} TYPEARG;

char *__exename; // executable name
static TYPEARG __args[ARGS_MAX_ARGC];
int __argscount;
static TYPEARG __argp[ARGS_MAX_ARGC];
int __argpcount;
int __argc;

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
         strcmp(arg, "okay") == 0 || strcmp(arg, "ok") == 0 ||
         strcmp(arg, "y") == 0 || strcmp(arg, "t") == 0 ||
         strcmp(arg, "1") == 0;
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

static inline void args_printhelp(FILE *const stream)
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
  fprintf(stream, " [options]");
  for (int i = 0; i < __argpcount; ++i)
  {
    fprintf(stream, " %s", __argp[i].name);
  }
  fprintf(stream, " ...\n");
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
  for (int i = 0; i < __argpcount; ++i)
  {
    if (!__argp[i].reqd)
    {
      continue;
    }
    fprintf(stream, "    -%s", __argp[i].name);
    if (!__argp[i].flag)
    {
      fprintf(stream, " (%s)", argtypenames[__argp[i].type]);
    }
    if (__argp[i].mesg)
    {
      fprintf(stream, ": %s", __argp[i].mesg);
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

#define args_default() args_flag(help, "Show this help message and quit", true)

#define args_var(...) _args_var(__VA_ARGS__, NULL, NULL)
#define _args_var(VAR, TYPE, MESSAGE, DEFAULT, ...)                            \
  const int __arg_##VAR##_i = __argscount;                                     \
  TYPE VAR;                                                                    \
  do                                                                           \
  {                                                                            \
    __args[__arg_##VAR##_i].name = #VAR;                                       \
    __args[__arg_##VAR##_i].type = TYPE##arg;                                  \
    __args[__arg_##VAR##_i].mesg = MESSAGE;                                    \
    __args[__arg_##VAR##_i].var  = (void *) &(VAR);                            \
    __args[__arg_##VAR##_i].def  = (void *) DEFAULT;                           \
    __args[__arg_##VAR##_i].used = false;                                      \
    __args[__arg_##VAR##_i].flag = false;                                      \
    __args[__arg_##VAR##_i].reqd = false;                                      \
    __args[__arg_##VAR##_i].sing = strlen(#VAR) == 1;                          \
    __argscount++;                                                             \
  } while (0)

#define args_flag(VAR, ...)                                                    \
  args_var(VAR, bool, __VA_ARGS__, NULL, NULL);                                \
  do                                                                           \
  {                                                                            \
    __args[__arg_##VAR##_i].flag = true;                                       \
  } while (0)

#define args_reqd(VAR)                                                         \
  do                                                                           \
  {                                                                            \
    __args[__arg_##VAR##_i].reqd = true;                                       \
  } while (0)

#define args_pop(...) _args_pop(__VA_ARGS__, NULL, NULL)
#define _args_pop(VAR, TYPE, MESSAGE, DEFAULT, ...)                            \
  const int __arg_##VAR##_i = __argpcount;                                     \
  TYPE VAR;                                                                    \
  do                                                                           \
  {                                                                            \
    __argp[__arg_##VAR##_i].name = #VAR;                                       \
    __argp[__arg_##VAR##_i].type = TYPE##arg;                                  \
    __argp[__arg_##VAR##_i].mesg = MESSAGE;                                    \
    __argp[__arg_##VAR##_i].var  = (void *) &(VAR);                            \
    __argp[__arg_##VAR##_i].def  = (void *) DEFAULT;                           \
    __argp[__arg_##VAR##_i].used = false;                                      \
    __argp[__arg_##VAR##_i].flag = false;                                      \
    __argp[__arg_##VAR##_i].reqd = true;                                       \
    __argp[__arg_##VAR##_i].sing = strlen(#VAR) == 1;                          \
    __argpcount++;                                                             \
  } while (0)

#define args_parse(ARGC, ARGV)                                                 \
  do                                                                           \
  {                                                                            \
    __argc                = ARGC;                                              \
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
    __argc -= 1;                                                               \
    for (int i = 1; i < ARGC; ++i)                                             \
    {                                                                          \
      char *arg = ARGV[i];                                                     \
      if (arg && arg[0] == '-')                                                \
      {                                                                        \
        bool found = false;                                                    \
        char *argn = &(arg)[1];                                                \
        for (int j = 0; j < __argscount; ++j)                                  \
        {                                                                      \
          if (!__args[j].sing && argn[0] == '-')                               \
          {                                                                    \
            argn += 1;                                                         \
          }                                                                    \
          int argnn = strlen(argn);                                            \
          int namen = strlen(__args[j].name);                                  \
          if (__args[j].flag && (argnn > namen && argn[namen] == '='))         \
          {                                                                    \
            fprintf(stderr,                                                    \
                    "ERROR: Argument '%s' doesn't accept a value\n",           \
                    __args[j].name);                                           \
            args_printhelp(stderr);                                            \
            exit(1);                                                           \
          }                                                                    \
          if (strcmp(__args[j].name, argn) == 0)                               \
          {                                                                    \
            if (__args[j].flag)                                                \
            {                                                                  \
              if ((i + 1 < __argc && ARGV[i + 1][0] != '-') ||                 \
                  (argnn > namen && argn[namen] == '='))                       \
              {                                                                \
                fprintf(stderr,                                                \
                        "ERROR: Argument '%s' doesn't accept a value\n",       \
                        __args[j].name);                                       \
                args_printhelp(stderr);                                        \
                exit(1);                                                       \
              }                                                                \
              argsetdef(__args[j].var, (void *) 1);                            \
              __argc -= 1;                                                     \
            }                                                                  \
            else                                                               \
            {                                                                  \
              if (argnn > namen && argn[namen] == '=')                         \
              {                                                                \
                argsetvar(__args[j].var, __args[j].type, &(argn)[namen + 1]);  \
                __argc -= 1;                                                   \
              }                                                                \
              else                                                             \
              {                                                                \
                argsetvar(__args[j].var, __args[j].type, ARGV[++i]);           \
                __argc -= 2;                                                   \
              }                                                                \
            }                                                                  \
            __args[j].used = true;                                             \
            found          = true;                                             \
            break;                                                             \
          }                                                                    \
        }                                                                      \
        if (!found)                                                            \
        {                                                                      \
          for (int i = 0; argn[i] != '\0'; ++i)                                \
          {                                                                    \
            bool isflag = false;                                               \
            for (int j = 0; j < __argscount; ++j)                              \
            {                                                                  \
              if (__args[j].sing && __args[j].flag &&                          \
                  argn[i] == __args[j].name[0])                                \
              {                                                                \
                __args[j].used = true;                                         \
                argsetdef(__args[j].var, (void *) true);                       \
                isflag = true;                                                 \
                break;                                                         \
              }                                                                \
            }                                                                  \
            if (!isflag)                                                       \
            {                                                                  \
              fprintf(stderr, "ERROR: Unknown argument '%s'\n", argn);         \
              args_printhelp(stderr);                                          \
              exit(1);                                                         \
            }                                                                  \
          }                                                                    \
          __argc -= 1;                                                         \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    for (int j = 0; j < __argscount; ++j)                                      \
    {                                                                          \
      if (__args[j].flag && __args[j].type == boolarg &&                       \
          strcmp(__args[j].name, "help") == 0 &&                               \
          *((bool *) __args[j].var) == true)                                   \
      {                                                                        \
        args_printhelp(stdout);                                                \
        exit(0);                                                               \
      }                                                                        \
      if (!__args[j].used)                                                     \
      {                                                                        \
        if (__args[j].reqd)                                                    \
        {                                                                      \
          fprintf(stderr,                                                      \
                  "ERROR: Missing required argument '%s'\n",                   \
                  __args[j].name);                                             \
          args_printhelp(stderr);                                              \
          exit(1);                                                             \
        }                                                                      \
        else                                                                   \
        {                                                                      \
          argsetdef(__args[j].var, __args[j].def);                             \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

#define args_popend(ARGC, ARGV)                                                \
  do                                                                           \
  {                                                                            \
    if (__argc < __argpcount)                                                  \
    {                                                                          \
      fprintf(stderr,                                                          \
              "ERROR: Missing required argument '%s'\n",                       \
              __argp[__argc].name);                                            \
      args_printhelp(stderr);                                                  \
      exit(1);                                                                 \
    }                                                                          \
    for (int j = 0; j < __argpcount; ++j)                                      \
    {                                                                          \
    }                                                                          \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif
/*
This software is served under two licenses - pick which you prefer.
--------------------------------------------------------------------------------
MIT License
Copyright 2023 Dani Yeomans

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------------------
Public Domain
Dedicated 2023 Dani Yeomans

The author of this software and associated documentation files (the "Software")
also dedicates any and all copyright interest in the Software to the public
domain. The author makes this dedication for the benefit of the public at large
and to the detriment of the author's heirs and successors. The author intends
this dedication to be an overt act of relinquishment in perpetuity of all
present and future rights to the Software under copyright law.

Any person obtaining a copy of the Software and associated documentation files
(the "Software") is free to to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to no conditions.
--------------------------------------------------------------------------------
*/
