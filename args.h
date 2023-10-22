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
- `args_help(FILE *const)`: Prints the help message to the specified
stream.

## Notes

- `args_parse` will detect the help flag being true and print the help message
for you, if the name is "help", and it's a bool flag set to `true`.
- This supports -flag value and -flag=value syntax
- Compound single-letter flags like -abc are supported as long as all of the
single-letter flags exist. Else, the whole flag will be treated as invalid.
- `args_help` is based off of the existing values. If you call it before
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
#define ARGS_PATHSEP '\\'
#endif
#define strdup _strdup
#else
#define ARGS_PATHSEP '/'
#endif

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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

#ifndef string
typedef char *string;
#endif

typedef char *filepath;
typedef char *dirpath;

#ifndef ARGTYPES_X_LIST_CUSTOM
#define ARGTYPES_X_LIST_CUSTOM
#endif

#define ARGTYPES_X_LIST                                                        \
  X(bool)                                                                      \
  X(char)                                                                      \
  X(short)                                                                     \
  X(int)                                                                       \
  X(long)                                                                      \
  X(float)                                                                     \
  X(double)                                                                    \
  X(string)                                                                    \
  X(filepath)                                                                  \
  X(dirpath)                                                                   \
  ARGTYPES_X_LIST_CUSTOM

#define _ARGFMT(TYPE) _##TYPE##_fmt
#define _bool_fmt "%x"
#define _char_fmt "%c"
#define _short_fmt "%d"
#define _int_fmt "%d"
#define _long_fmt "%ld"
#define _float_fmt "%g"
#define _double_fmt "%g"
#define _string_fmt "%s"
#define _filepath_fmt "%s"
#define _dirpath_fmt "%s"

#define _ARGTYPE(TYPE) _##TYPE##_type
typedef enum argtype_t
{
#define X(TYPE) _ARGTYPE(TYPE),
  ARGTYPES_X_LIST
#undef X
} argtype_t;

const char *_argtype_names[] = {
#define X(TYPE) #TYPE,
  ARGTYPES_X_LIST
#undef X
};

const int _argtype_sizes[] = {
#define X(TYPE) sizeof(TYPE),
  ARGTYPES_X_LIST
#undef X
};

// -----------------------------------------------------------------------------

typedef struct arg_t
{
  const char *name;
  argtype_t type;
  const char *message;
  void *value;
  void *defaultValue;
  bool isUsed;
  bool isFlag;
  bool isRequired;
  bool isSingleLetter;
  bool isPopped;
} arg_t;

typedef struct args_t
{
  char *exe;
  arg_t args[ARGS_MAX_ARGC];
  int argc;
  int argr;
} *args_t;

// -----------------------------------------------------------------------------

#define args_new() _args_new_impl()

#define args_default(args_t)                                                     \
  do                                                                           \
  {                                                                            \
    args_flag(args_t, help, "Display this help message and exit");               \
  } while (0)

#define _args_common(args_t, NAME, TYPE, ISFLAG, ISPOPPED, ...)                  \
  _args_arg_impl(args_t, NAME, TYPE, ISFLAG, ISPOPPED, 0, ""__VA_ARGS__, 0, 0,   \
                 0, 0)

#define args_arg(args_t, NAME, TYPE, ...)                                        \
  ((TYPE *) _args_common(args_t, #NAME, _ARGTYPE(TYPE), false, false,            \
                         __VA_ARGS__))

#define args_flag(args_t, NAME, ...)                                             \
  ((bool *) _args_common(args_t, #NAME, _bool_type, true, false, __VA_ARGS__))

#define args_pop(args_t, NAME, TYPE, ...)                                        \
  ((TYPE *) _args_common(args_t, #NAME, _ARGTYPE(TYPE), false, true, __VA_ARGS__))

extern args_t _args_new_impl();

extern void *_args_arg_impl(args_t args, const char *name, argtype_t type,
                            bool isFlag, bool isPopped, int _, ...);

extern bool _args_get_bool(char *value);
extern char _args_get_char(char *value);
extern short _args_get_short(char *value);
extern int _args_get_int(char *value);
extern long _args_get_long(char *value);
extern float _args_get_float(char *value);
extern double _args_get_double(char *value);
extern string _args_get_str(char *value);
extern bool _args_set_value(arg_t *arg, char *value);

extern void args_help(FILE *const stream, args_t args);
extern void _args_parse(args_t args, int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif // _ARGS_H

// -----------------------------------------------------------------------------

#ifdef ARGS_IMPLEMENTATION
#ifndef _ARGS_C
#define _ARGS_C

args_t _args_new_impl()
{
  args_t args  = (args_t) malloc(sizeof(struct args_t));
  args->argc = 0;
  memset(args->args, 0, sizeof(arg_t) * ARGS_MAX_ARGC); // initialize to zero
  return args;
}

void *_args_arg_impl(args_t args, const char *name, argtype_t type, bool isFlag,
                     bool isPopped, int _, ...)
{
  va_list varg;
  va_start(varg, _);
  char *message      = va_arg(varg, char *);
  void *defaultValue = va_arg(varg, void *);
  bool isRequired    = va_arg(varg, int);
  va_end(varg);
  if (!name || strchr(name, ' ') != NULL)
  {
    fprintf(stderr, "ERROR: Invalid argument name '%s'\n", name);
    exit(1);
  }
  int tlen = _argtype_sizes[type];
  args->args[args->argc] =
    (arg_t){name,  type,   message,    malloc(tlen),      (void *) defaultValue,
          false, isFlag, isRequired, strlen(name) == 1, isPopped};
  *((void **) args->args[args->argc].value) = (void *) defaultValue;
  args->argc += 1;
  return args->args[args->argc - 1].value;
};

bool _args_get_bool(char *value)
{
  if (!value)
  {
    return false;
  }
  for (int i = 0; value[i]; i++)
  {
    value[i] = tolower((unsigned char) value[i]);
  }
  return strcmp(value, "y") == 0 || strcmp(value, "t") == 0 ||
         strcmp(value, "1") == 0 || strcmp(value, "yes") == 0 ||
         strcmp(value, "true") == 0 || strcmp(value, "okay") == 0 ||
         strcmp(value, "ok") == 0;
}

char _args_get_char(char *value)
{
  return value ? value[0] : 0;
}

short _args_get_short(char *value)
{
  return (short) atoi(value);
}

int _args_get_int(char *value)
{
  return atoi(value);
}

long _args_get_long(char *value)
{
  return atol(value);
}

float _args_get_float(char *value)
{
  return (float) atof(value);
}

double _args_get_double(char *value)
{
  return atof(value);
}

string _args_get_string(char *value)
{
  return value;
}

filepath _args_get_filepath(char *value)
{
  struct stat buffer;
  if (stat(value, &buffer) != 0 || (buffer.st_mode & S_IFMT) != S_IFREG)
  {
    fprintf(stderr, "ERROR: Value '%s' is not a valid filepath\n", value);
    exit(1);
  }
  return strdup(value); // Return a copy of the string
}

dirpath _args_get_dirpath(char *value)
{
  struct stat buffer;
  if (stat(value, &buffer) != 0 || (buffer.st_mode & S_IFMT) != S_IFDIR)
  {
    fprintf(stderr, "ERROR: Value '%s' is not a valid directory\n", value);
    exit(1);
  }
  return strdup(value); // Return a copy of the string
}

bool _args_set_value(arg_t *a, char *xval)
{
  switch (a->type)
  {
    default:
    {
      *((void **) a->value) = NULL;
      return false;
    }
#define X(TYPE)                                                                \
  case _ARGTYPE(TYPE):                                                         \
  {                                                                            \
    *((TYPE *) a->value) = _args_get_##TYPE(xval);                             \
    return true;                                                               \
  }
      ARGTYPES_X_LIST
#undef X
  }
}

void _arg_help(FILE *const s, arg_t a)
{
  fprintf(s, "    -%s", a.name);
  if (!a.isFlag)
  {
    if (!a.isRequired)
    {
      fprintf(s, "[");
    }
    fprintf(s, "=%s", _argtype_names[a.type]);
    if (!a.isRequired)
    {
      fprintf(s, "]");
    }
  }
  if (a.message != NULL && strlen(a.message) != 0)
  {
    fprintf(s, ": %s", a.message);
    switch (a.type)
    {
#define X(TYPE)                                                                \
  case _ARGTYPE(TYPE):                                                         \
    if (a.defaultValue != 0)                                                   \
    {                                                                          \
      printf(" (default: "_ARGFMT(TYPE) ")",                                   \
             ((TYPE) ((uintptr_t) a.defaultValue)));                           \
    }                                                                          \
    break;
      ARGTYPES_X_LIST
    }
  }
  fprintf(s, "\n");
}

void args_help(FILE *const s, args_t args)
{
  fprintf(s, "usage: %s", args->exe);
  bool singleLetter = false;
  for (int i = 0; i < args->argc; ++i)
  {
    arg_t a = args->args[i];
    if (!a.isRequired)
    {
      continue;
    }
    if (a.isSingleLetter)
    {
      if (!singleLetter)
      {
        fprintf(s, " -");
      }
      singleLetter = true;
      fprintf(s, "%s", a.name);
    }
    else
    {
      fprintf(s, " -%s", a.name);
    }
  }
  fprintf(s, " [options]");
  for (int i = 0; i < args->argc; ++i)
  {
    arg_t a = args->args[i];
    if (a.isPopped)
    {
      fprintf(s, " %s", a.name);
    }
  }
  fprintf(s, " ...\n");
  bool hasRequired = false;
  for (int i = 0; i < args->argc; ++i)
  {
    arg_t a = args->args[i];
    if (!a.isRequired || a.isPopped)
    {
      continue;
    }
    if (!hasRequired)
    {
      hasRequired = true;
      fprintf(s, "  required:\n");
    }
    _arg_help(s, a);
  }
  bool hasOptions = false;
  for (int i = 0; i < args->argc; ++i)
  {
    arg_t a = args->args[i];
    if (a.isRequired || a.isPopped)
    {
      continue;
    }
    if (!hasOptions)
    {
      hasOptions = true;
      fprintf(s, "  options:\n");
    }
    _arg_help(s, a);
  }
  bool hasPopped = false;
  for (int i = 0; i < args->argc; ++i)
  {
    arg_t a = args->args[i];
    if (!a.isPopped)
    {
      continue;
    }
    if (!hasPopped)
    {
      hasPopped = true;
      fprintf(s, "  arguments:\n");
    }
    _arg_help(s, a);
  }
}

#define args_parse(args_t, ARGC, ARGV)                                           \
  do                                                                           \
  {                                                                            \
    _args_parse(args_t, ARGC, ARGV);                                             \
    (ARGC) -= (args_t)->argr;                                                    \
    (ARGV) += (args_t)->argr - 1;                                                \
  } while (0)

void _args_parse(args_t args, int argc, char **argv)
{
  char *exe = strrchr(argv[0], ARGS_PATHSEP);
  exe       = exe ? exe + 1 : argv[0];
  int len   = strrchr(exe, '.') - exe;
  len       = (__args_win32 || len <= 0) ? strlen(exe) : len;
  args->exe = malloc(len + 1);
  strncpy(args->exe, exe, len);
  args->exe[len] = '\0';
  argc -= 1;
  argv += 1;
  for (int i = 0; i < argc; ++i)
  {
    char *arg  = argv[i];
    char *xval = strrchr(arg, '=');
    if (xval != NULL)
    {
      xval += 1;
    }
    else if (i + 1 < argc && argv[i + 1][0] != '-')
    {
      xval = argv[i + 1];
    }
    if (arg[0] == '-')
    {
      bool found = false;
      arg += 1;
      int arglen = strlen(arg);
      for (int j = 0; j < args->argc; ++j)
      {
        arg_t *a = &args->args[j];
        if (!a->isSingleLetter && arg[0] == '-')
        {
          arg += 1;
          arglen = strlen(arg);
        }
        int namelen = strrchr(arg, '=') - arg;
        if (namelen <= 0)
        {
          namelen = strlen(arg);
        }
        if (!strncmp(arg, a->name, namelen))
        {
          if (!a->isFlag)
          {
            if (xval == NULL)
            {
              fprintf(stderr, "ERROR: Expected value for argument '%s'\n",
                      a->name);
              args_help(stderr, args);
              exit(1);
            }
            if (!_args_set_value(a, xval))
            {
              fprintf(stderr, "ERROR: Couldn't determine argument '%s' type\n",
                      xval);
              args_help(stderr, args);
              exit(1);
            }
            args->argr += (i + 1 < argc && xval == argv[i + 1]) ? 2 : 1;
          }
          else
          {
            if (xval != NULL)
            {
              fprintf(stderr,
                      "ERROR: Unexpected value '%s' for argument '%s'\n", xval,
                      a->name);
              args_help(stderr, args);
              exit(1);
            }
            *((bool *) a->value) = true;
          }
          found     = true;
          a->isUsed = true;
          args->argr += 1;
        }
      }
      if (!found)
      {
        int i = 0;
        for (int j = 0; j < args->argc; ++j)
        {
          arg_t *a = &(args->args)[j];
          if (!a->isFlag && !a->isSingleLetter)
          {
            continue;
          }
          if (arg[i] == a->name[0])
          {
            *((bool *) a->value) = true;
            a->isUsed            = true;
            i++;
            args->argr += 1;
          }
        }
        if (i != arglen)
        {
          fprintf(stderr, "ERROR: Unexpected argument '%s'\n", arg);
          args_help(stderr, args);
          exit(1);
        }
      }
    }
  }
  for (int j = 0; j < args->argc; ++j)
  {
    arg_t a = args->args[j];
    if (a.isRequired && !a.isUsed)
    {
      fprintf(stderr, "ERROR: Missing required argument '%s'\n", a.name);
      args_help(stderr, args);
      exit(1);
    }
  }
  for (int i = args->argr; i < argc; ++i)
  {
    for (int j = 0; j < args->argc; ++j)
    {
      arg_t a = args->args[j];
      if (a.isPopped)
      {
        _args_set_value(&a, argv[i]);
        a.isUsed = true;
        args->argr += 1;
      }
    }
  }
  for (int j = 0; j < args->argc; ++j)
  {
    arg_t a = args->args[j];
    if (a.isPopped && !a.isUsed)
    {
      fprintf(stderr, "ERROR: Missing required argument '%s'\n", a.name);
      args_help(stderr, args);
      exit(1);
    }
  }
}

#endif
#endif

/*
--------------------------------------------------------------------------------
This software is served under two licenses - pick which you prefer.
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
