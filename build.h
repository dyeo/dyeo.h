#ifndef _BUILD_H
#define _BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Platforms
// ---------------------------------------------------------------------------------------------------------------------

// Windows
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#define __WINDOWS__ 1
#else
#define __WINDOWS__ 0
#endif

// Unix
#if defined(__unix__)
#define __UNIX__ 1
#else
#define __UNIX__ 0
#endif

// Linux
#if defined(__linux__)
#define __LINUX__ 1
#else
#define __LINUX__ 0
#endif

// BSD
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) ||     \
  defined(__bsdi__) || defined(__DragonFly__)
#define __BSD__ 1
#else
#define __BSD__ 0
#endif

// Solaris
#if defined(__sun)
#define __SOLARIS__ 1
#else
#define __SOLARIS__ 0
#endif

// Android
#if defined(__ANDROID__)
#define __ANDROID__ 1
#else
#define __ANDROID__ 0
#endif

// MacOS / iOS
#if defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC == 1
#define __MACOS__ 1
#define __IOS__ 0
#elif (TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1)
#define __MACOS__ 0
#define __IOS__ 1
#else
#define __MACOS__ 0
#define __IOS__ 0
#error "Unsupported apple platform"
#endif
#else
#define __MACOS__ 0
#define __IOS__ 0
#endif

#if __LINUX__ || __BSD__ || __SOLARIS__ || __ANDROID__ || __UNIX__ ||          \
  __MACOS__ || __IOS__
#define __POSIX__ 1
#else
#define __POSIX__ 0
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Compilers
// ---------------------------------------------------------------------------------------------------------------------

// GCC
#if defined(__GNUC__) && !defined(__clang__)
#define __GCC__ 1
#define __COMPILER__ "gcc"
// Clang
#elif defined(__clang__)
#define __CLANG__ 1
#define __COMPILER__ "clang"
// MSVC
#elif defined(_MSC_VER)
#define __MSVC__ 1
#define __COMPILER__ "cl.exe"
// Intel
#elif defined(__INTEL_COMPILER) || defined(__ICC)
#define __INTEL__ 1
#define __COMPILER__ "intel"
// IBM
#elif defined(__xlC__) || defined(__IBMC__)
#define __IBM__ 1
#define __COMPILER__ "ibm"
// ARM
#elif defined(__CC_ARM) || defined(__ARMCC__)
#define __ARM__ 1
#define __COMPILER__ "arm"
// Oracle Solaris (Sun Studio)
#elif defined(__SUNPRO_C)
#define __SUNPRO__ 1
#define __COMPILER__ "sunpro"
// HP aCC
#elif defined(__HP_cc) || defined(__HP_aCC)
#define __HPACC__ 1
#define __COMPILER__ "hpacc"
// Portland Group (PGI)
#elif defined(__PGI)
#define __PGI__ 1
#define __COMPILER__ "pgi"
// Digital Mars
#elif defined(__DMC__)
#define __DMC__ 1
#define __COMPILER__ "dmc"
#else
#define __COMPILER__ NULL
#error "Invalid compiler"
#endif

#ifndef __GCC__
#define __GCC__ 0
#endif
#ifndef __CLANG__
#define __CLANG__ 0
#endif
#ifndef __MSVC__
#define __MSVC__ 0
#endif
#ifndef __INTEL__
#define __INTEL__ 0
#endif
#ifndef __IBM__
#define __IBM__ 0
#endif
#ifndef __ARM__
#define __ARM__ 0
#endif
#ifndef __SUNPRO__
#define __SUNPRO__ 0
#endif
#ifndef __HPACC__
#define __HPACC__ 0
#endif
#ifndef __PGI__
#define __PGI__ 0
#endif
#ifndef __DMC__
#define __DMC__ 0
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Configurations
// ---------------------------------------------------------------------------------------------------------------------

#if (__SIZEOF_POINTER__ == 16)
#define __128_BIT__ 1
#define __64_BIT__ 0
#define __32_BIT__ 0
#define __16_BIT__ 0
#elif defined(__aarch64__) || defined(__arm64__) || defined(__x86_64__) ||     \
  defined(__x86_64) || defined(__ppc64__) || defined(__powerpc64__) ||         \
  defined(_LP64) || defined(__LP64__) || defined(__amd64__) ||                 \
  defined(__amd64) || defined(_M_AMD64) || defined(_M_AMD_64) ||               \
  defined(_M_X64) || defined(_M_IX64) || defined(_WIN64) ||                    \
  defined(__mips64__) || defined(__sparc64__) || defined(__alpha__) ||         \
  defined(__ia64__) || defined(__s390x__) || (__SIZEOF_POINTER__ == 8)
#define __128_BIT__ 0
#define __64_BIT__ 1
#define __32_BIT__ 0
#define __16_BIT__ 0
#elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) ||             \
  defined(__THUMB__) || defined(_M_ARM) || defined(_M_ARMT) ||                 \
  defined(__mips__) || defined(__powerpc__) || defined(__sparc__) ||           \
  defined(__s390__) || defined(__sh__) || defined(__MIPSEL__) ||               \
  defined(_WIN32) || (__SIZEOF_POINTER__ == 4)
#define __128_BIT__ 0
#define __64_BIT__ 0
#define __32_BIT__ 1
#define __16_BIT__ 0
#elif defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__) ||             \
  defined(M_I86) || defined(_M_I86) || defined(__TINYC__) ||                   \
  (__SIZEOF_POINTER__ == 2)
#define __128_BIT__ 0
#define __64_BIT__ 0
#define __32_BIT__ 0
#define __16_BIT__ 1
#else
#define __128_BIT__ 0
#define __64_BIT__ 0
#define __32_BIT__ 0
#define __16_BIT__ 0
#error "Unable to determine bit-width"
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Endianness
// ---------------------------------------------------------------------------------------------------------------------

#if __WINDOWS__ || defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) ||         \
  defined(__THUMBEL__) || defined(__AARCH64EL__) ||                            \
  (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define __LITTLE_ENDIAN__ 1
#define __BIG_ENDIAN__ 0
#elif defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || \
  defined(__AARCH64EB__) ||                                                    \
  (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define __LITTLE_ENDIAN__ 0
#define __BIG_ENDIAN__ 1
#else
#define __LITTLE_ENDIAN__ 0
#define __BIG_ENDIAN__ 0
#error "Unable to determine endianness"
#endif

// ---------------------------------------------------------------------------------------------------------------------
// CPU
// ---------------------------------------------------------------------------------------------------------------------

#if defined(_M_AMD64) || defined(_M_AMD_64)
#define __CPU_AMD__ 1
#define __CPU_INTEL__ 1
#elif defined(__i386__) || defined(__x86_64__)
#define __CPU_INTEL__ 1
#elif defined(__arm__) || defined(__aarch64__)
#define __CPU_ARM__ 1
#elif defined(__mips__) || defined(__mips64__)
#define __CPU_MIPS__ 1
#elif defined(__powerpc__) || defined(__powerpc64__)
#define __CPU_POWERPC__ 1
#elif defined(__sparc__) || defined(__sparc64__)
#define __CPU_SPARC__ 1
#elif defined(__alpha__)
#define __CPU_ALPHA__ 1
#elif defined(__ia64__)
#define __CPU_ITANIUM__ 1
#elif defined(__s390__) || defined(__s390x__)
#define __CPU_S390__ 1
#elif defined(__sh__)
#define __CPU_SUPERH__ 1
#elif defined(__riscv)
#define __CPU_RISCV__ 1
#else
#error "Invalid CPU."
#endif

#ifndef __CPU_AMD__
#define __CPU_AMD__ 0
#endif
#ifndef __CPU_INTEL__
#define __CPU_INTEL__ 0
#endif
#ifndef __CPU_ARM__
#define __CPU_ARM__ 0
#endif
#ifndef __CPU_MIPS__
#define __CPU_MIPS__ 0
#endif
#ifndef __CPU_POWERPC__
#define __CPU_POWERPC__ 0
#endif
#ifndef __CPU_SPARC__
#define __CPU_SPARC__ 0
#endif
#ifndef __CPU_ALPHA__
#define __CPU_ALPHA__ 0
#endif
#ifndef __CPU_ITANIUM__
#define __CPU_ITANIUM__ 0
#endif
#ifndef __CPU_S390__
#define __CPU_S390__ 0
#endif
#ifndef __CPU_SUPERH__
#define __CPU_SUPERH__ 0
#endif
#ifndef __CPU_RISCV__
#define __CPU_RISCV__ 0
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Platform-specific tidbits
// ---------------------------------------------------------------------------------------------------------------------

#if __WINDOWS__
#define strdup _strdup
#define PATHSEP_CHR '\\'
#define PATHSEP_ALT_CHR '/'
#define PATHSEP "\\"
#define PATHSEP_ALT "/"
#define NEWLINE "\r\n"
#if defined(BUILD_H_WINDOWS_MAXPATH)
#undef MAX_PATH
#define MAX_PATH BUILD_H_WINDOWS_MAXPATH
#endif
#elif __POSIX__
#define PATHSEP_CHR '/'
#define PATHSEP_ALT_CHR '\\'
#define PATHSEP "/"
#define PATHSEP_ALT "\\"
#define NEWLINE "\n"
#if __128_BIT__ || __64_BIT__
#define MAX_PATH 4096
#elif __32_BIT__
#define MAX_PATH 2048
#else
#define MAX_PATH 1024
#endif
#endif

// ---------------------------------------------------------------------------------------------------------------------
// Misc utils
// ---------------------------------------------------------------------------------------------------------------------

// get printf format string
#define format_str(V)                                                          \
  _Generic((V),                                                                \
    int: "%d",                                                                 \
    unsigned int: "%u",                                                        \
    long: "%ld",                                                               \
    unsigned long: "%lu",                                                      \
    long long: "%lld",                                                         \
    unsigned long long: "%llu",                                                \
    float: "%f",                                                               \
    double: "%lf",                                                             \
    long double: "%Lf",                                                        \
    char: "%c",                                                                \
    bool: ((V) ? "true" : "false"),                                            \
    default: "%p")

// ---------------------------------------------------------------------------------------------------------------------

// dynamically allocated ptr
#define _malloc

// ---------------------------------------------------------------------------------------------------------------------

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __WINDOWS__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if __POSIX__
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

// ---------------------------------------------------------------------------------------------------------------------

#if defined(BUILD_H_DISABLE_COLORS)
#define _TermColStr(CodeStr) ""
#else
#define _TermColStr(CodeStr) "\x01b[" CodeStr "m"
#endif

#define _TermCol0 _TermColStr("0;0")
#define _TermColHint _TermColStr("3;90")
#define _TermColInfo _TermColStr("0;90")
#define _TermColPass _TermColStr("0;32")
#define _TermColWarn _TermColStr("0;33")
#define _TermColErr _TermColStr("0;31")
#define _TermColPing _TermColStr("0;35")
#define _TermColCmd _TermColStr("0;93")

// ---------------------------------------------------------------------------------------------------------------------

#define _log_full(STDOUT, PREFIX, FORMAT, ...)                                 \
  fprintf(STDOUT, PREFIX " " _TermCol0 FORMAT NEWLINE, __VA_ARGS__)

// log info message
#define log_info(FORMAT, ...)                                                  \
  _log_full(stdout, _TermColInfo " ", FORMAT, __VA_ARGS__)

// log warning
#define log_warning(FORMAT, ...)                                               \
  _log_full(stdout, _TermColWarn "?", FORMAT, __VA_ARGS__)

// log a success or test pass
#define log_pass(FORMAT, ...)                                                  \
  _log_full(stdout, _TermColPass "O", FORMAT, __VA_ARGS__)

// log a failure or test fail
#define log_error(FORMAT, ...)                                                 \
  _log_full(stderr, _TermColErr "X", FORMAT, __VA_ARGS__)

// log a command
#define log_cmd(CMD, args_t)                                                   \
  _log_full(stdout, _TermColPing ">", _TermColCmd "%s" _TermCol0 " %s", CMD,   \
            args_t)

// ---------------------------------------------------------------------------------------------------------------------

#if __WINDOWS__
_malloc char *_build_h_errmsg(DWORD errorMessageID)
{
  if (errorMessageID == 0)
  {
    return NULL;
  }
  LPSTR messageBuffer = NULL;
  size_t size         = FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPSTR) &messageBuffer, 0, NULL);
  if (size == 0)
  {
    return NULL;
  }
  char *message = (char *) malloc(size);
  if (message == NULL)
  {
    LocalFree(messageBuffer);
    return NULL;
  }
  memcpy(message, messageBuffer, size - 1);
  message[size - 1] = '\0';
  LocalFree(messageBuffer);
  return message;
}
#elif __POSIX__
_malloc char *_build_h_errmsg(int errorNumber)
{
  if (errorNumber == 0)
  {
    return NULL;
  }
  size_t size   = 1024;
  char *message = (char *) malloc(size);
  if (message == NULL)
  {
    return NULL;
  }
  if (strerror_r(errorNumber, message, size) != 0)
  {
    free(message);
    return NULL;
  }
  return message;
}

#endif

// ---------------------------------------------------------------------------------------------------------------------

// compose a path from N elements
#define path(...) _build_h_path(PATHSEP, __VA_ARGS__, NULL)
_malloc char *_build_h_path(const char *pathsep, ...)
{
  size_t seplen = strlen(pathsep);
  va_list args;

  char *result = (char *) malloc(MAX_PATH);
  if (!result)
  {
    return NULL;
  }

  char *curr = result;

  va_start(args, pathsep);
  const char *part = NULL;
  size_t i         = 0;
  while ((part = va_arg(args, const char *)) != NULL)
  {
    if (i > 0 && part[0] != pathsep[0] && part[0] != PATHSEP_ALT_CHR)
    {
      memcpy(curr, pathsep, seplen);
      curr += seplen;
    }

    size_t partlen = strlen(part);

#if __WINDOWS__
    if (i == 0 && partlen == 1 &&
        ((part[0] >= 'a' && part[0] <= 'z') ||
         (part[0] >= 'A' && part[0] <= 'Z')))
    {
      *curr++ = toupper(part[0]);
      *curr++ = ':';
    }
    else
#endif
    {
      if (i == 0 && part[0] != '.' && part[0] != pathsep[0] &&
          part[0] != PATHSEP_ALT_CHR)
      {
        *curr++ = '.';
        memcpy(curr, PATHSEP, seplen);
        curr += seplen;
      }
      for (size_t j = 0; j < partlen; ++j)
      {
        char partc = part[j];
        if (partc == PATHSEP_ALT_CHR)
        {
          partc = PATHSEP_CHR;
        }
        *(curr + j) = partc;
      }
      curr += partlen;
    }

    i++;
  }
  va_end(args);

  *curr  = '\0';
  result = realloc(result, curr - result);

  return result;
}

// ---------------------------------------------------------------------------------------------------------------------

// get absolute path
#define abspath(...) _build_h_abspath(path(__VA_ARGS__))
_malloc char *_build_h_abspath(_malloc char *relpath)
{
  if (relpath == NULL)
  {
    log_error("%s", "Path is NULL");
    return NULL;
  }

  char *abspath = (char *) malloc(MAX_PATH);
#if __WINDOWS__
  if (abspath == NULL)
  {
    log_error("%s", "Failed to allocate memory");
    free(relpath);
    return NULL;
  }

  DWORD len;
  if ((len = GetFullPathName(relpath, MAX_PATH, abspath, NULL)) == 0)
  {
    log_error("%s", "Abspath invalid");
    free(abspath);
    free(relpath);
    return NULL;
  }
#elif __POSIX__
  if (abspath == NULL)
  {
    log_error("%s", "Failed to allocate memory");
    free(relpath);
    return NULL;
  }

  if (realpath(relpath, abspath) == NULL)
  {
    log_error("%s", "Abspath invalid");
    free(abspath);
    free(relpath);
    return NULL;
  }
  size_t len = strlen(abspath);
#endif
  abspath      = realloc(abspath, len + 1);
  abspath[len] = '\0';
  free(relpath);
  return abspath;
}

// ---------------------------------------------------------------------------------------------------------------------

// get file path (no file name)
#define filepath(...) _build_h_filepath(path(__VA_ARGS__))
_malloc char *_build_h_filepath(_malloc char *filepath)
{
  if (!filepath)
  {
    log_error("%s", "Path is NULL");
    return NULL;
  }

  char *last_slash = strrchr(filepath, PATHSEP_CHR);

  if (!last_slash)
  {
    free((void *) filepath);
    return NULL;
  }

  size_t length = last_slash - filepath;
  char *result  = (char *) malloc(length + 1);
  if (!result)
  {
    free((void *) filepath);
    return NULL;
  }

  strncpy(result, filepath, length);
  result[length] = '\0';
  free((void *) filepath);
  return result;
}

// ---------------------------------------------------------------------------------------------------------------------

// get file name
#define filename(...) _build_h_filename(path(__VA_ARGS__))
_malloc char *_build_h_filename(_malloc char *filepath)
{
  if (!filepath)
  {
    log_error("%s", "Path is NULL");
    return NULL;
  }

  const char *filename = strrchr(filepath, PATHSEP_CHR);

  if (!filename)
    filename = filepath;
  else
    filename++;

  char *result = strdup(filename);
  free((void *) filepath);
  return result;
}

// ---------------------------------------------------------------------------------------------------------------------

// get file extension
#define fileext(...) _build_h_fileext(path(__VA_ARGS__))
_malloc char *_build_h_fileext(_malloc char *filepath)
{
  if (!filepath)
  {
    log_error("%s", "Path is NULL");
    return NULL;
  }

  size_t i = 0;
  while (*(filepath + i) == '.')
    i++;

  const char *dot = strrchr((filepath + i), '.');
  if (!dot || dot == filepath)
    return NULL;

  char *result = strdup(dot + 1);
  free((void *) filepath);
  return result;
}

// ---------------------------------------------------------------------------------------------------------------------

#define mkdir(...) cmdcall("mkdir", "-p", path(__VA_ARGS__))

// ---------------------------------------------------------------------------------------------------------------------

// change directory
// use this over a cd command
#define cd(...) _build_h_cd(path(__VA_ARGS__))
#if __WINDOWS__
bool _build_h_cd(_malloc char *dirpath)
{
  if (!dirpath)
  {
    log_error("%s", "Path is NULL");
    return NULL;
  }

  log_cmd("cd", dirpath);
  bool result = SetCurrentDirectory(dirpath) != 0;
  if (!result)
  {
    char *msg = _build_h_errmsg(GetLastError());
    log_error("%s", msg);
    free(msg);
  }
  free(dirpath);
  return result;
}
#elif __POSIX__
// change directory
// use this over a cd command
bool _build_h_cd(_malloc char *dirpath)
{
  char *a = abspath(dirpath);
  log_cmd("cd", a);
  free(a);
  bool result = chdir(dirpath) == -1;
  if (result)
  {
    char *msg = _build_h_errmsg(errno);
    log_error("%s", msg);
    free(msg);
  }
  free(dirpath);
  return result;
}
#endif

// ---------------------------------------------------------------------------------------------------------------------

// get current directory
static inline _malloc char *cwd()
{
  char *res = (char *) malloc(MAX_PATH);
#if __WINDOWS__
  GetCurrentDirectory(MAX_PATH, res);
#else
  getcwd(res, MAX_PATH);
#endif
  size_t len = strlen(res);
  res        = realloc(res, len + 1);
  res[len]   = '\0';
  return res;
}

// ---------------------------------------------------------------------------------------------------------------------

typedef struct command_t
{
  char *cmd;
  char *args;
  size_t length;
  size_t capacity;
} *command_t;

static void _build_h_cmd_add_arg(command_t cmd, const char *arg)
{
  if (arg == NULL)
  {
    return;
  }
  size_t arg_length = strlen(arg);
  size_t new_length = cmd->length + arg_length + 3;

  if (new_length > cmd->capacity)
  {
    cmd->capacity = new_length * 2;
    cmd->args     = (char *) realloc(cmd->args, cmd->capacity);
    if (!cmd->args)
    {
      log_error("%s", "Failed to allocate memory for command");
      return;
    }
  }

  if (cmd->length > 0)
  {
    strcat(cmd->args, " ");
  }
  bool has_spaces = strchr(arg, ' ');
  if (has_spaces)
  {
    strcat(cmd->args, "\"");
  }
  strcat(cmd->args, arg);
  if (has_spaces)
  {
    strcat(cmd->args, "\"");
  }
  cmd->length = new_length - 1;
}

static inline void _build_h_cmdarg_va_list(command_t cmd, va_list args)
{
  const char *arg = NULL;
  while ((arg = va_arg(args, char *)) != NULL)
  {
    _build_h_cmd_add_arg(cmd, arg);
  }
}

// add N arguments to command
#define cmdarg(CMDPTR, ...) _build_h_cmdarg(CMDPTR, __VA_ARGS__, NULL)
static inline void _build_h_cmdarg(command_t cmd, ...)
{
  va_list args;
  va_start(args, cmd);
  _build_h_cmdarg_va_list(cmd, args);
  va_end(args);
}

#define cmdflag(CMD, BOOLFLAG)                                                 \
  ((BOOLFLAG) ? cmdarg(CMD, "-" #BOOLFLAG) : (void) (BOOLFLAG))

// create new command with N arguments
// free with cmdfree
#define cmdnew(...) _build_h_cmdnew(__VA_ARGS__, NULL)
static command_t _build_h_cmdnew(const char *command, ...)
{
  command_t cmd = (command_t) malloc(sizeof(struct command_t));
  if (!cmd)
  {
    log_error("%s", "Failed to allocate memory for command");
    return NULL;
  }
  cmd->cmd      = strdup(command);
  cmd->length   = 0;
  cmd->capacity = 64;
  cmd->args     = (char *) malloc(cmd->capacity);
  memset(cmd->args, 0, cmd->capacity);
  if (!cmd->args)
  {
    log_error("%s", "Failed to allocate memory for command");
    free(cmd);
    return NULL;
  }

  va_list args;
  va_start(args, command);
  _build_h_cmdarg_va_list(cmd, args);
  va_end(args);

  return cmd;
}

// get full command str
// free with free
static _malloc char *cmdstr(command_t cmd)
{
  if (cmd == NULL)
  {
    log_error("%s", "Command is NULL");
    return NULL;
  }

  if (cmd->cmd == NULL || cmd->args == NULL)
  {
    log_error("%s", "Command or arguments are NULL");
    return NULL;
  }

  size_t cmdlen = strlen(cmd->cmd);
  size_t arglen = strlen(cmd->args);

  char *res = (char *) malloc(cmdlen + arglen + 2);

  if (res == NULL)
  {
    log_error("%s", "Memory allocation failed");
    return NULL;
  }

  strcpy(res, cmd->cmd);
  res[cmdlen] = ' ';
  strcpy(res + cmdlen + 1, cmd->args);
  res[cmdlen + arglen + 1] = '\0';

  return res;
}

// free a command
static void cmdfree(command_t cmd)
{
  if (cmd == NULL)
  {
    log_error("%s", "Command is NULL");
    return;
  }
  free(cmd->cmd);
  free(cmd->args);
  free(cmd);
}

#define cmdcall(...)                                                           \
  do                                                                           \
  {                                                                            \
    command_t __thisCmdPtr = cmdnew(__VA_ARGS__);                              \
    if (__thisCmdPtr)                                                          \
    {                                                                          \
      cmdcallf(__thisCmdPtr);                                                  \
      cmdfree(__thisCmdPtr);                                                   \
    }                                                                          \
  } while (0)

// call the command
// returns true on success, false on failure
static bool cmdcallf(command_t c)
{
#if __WINDOWS__
  if (c == NULL)
  {
    log_error("%s", "Command is NULL");

    return false;
  }
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  log_cmd(c->cmd, c->args);
  printf(_TermColHint);
  char *cres = cmdstr(c);
  if (!CreateProcess(NULL, cres, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
  {
    free(cres);
    DWORD error = GetLastError();
    char *msg   = _build_h_errmsg(error);
    if (msg != NULL)
    {
      log_error("%s", msg);
      free(msg);
    }
    return false;
  }
  free(cres);

  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code;
  if (!GetExitCodeProcess(pi.hProcess, &exit_code))
  {
    log_error("%s", "Failed to get exit code");
    exit_code = EXIT_FAILURE;
  }

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  printf(_TermCol0);
  return exit_code == EXIT_SUCCESS;
#elif __POSIX__
  if (c == NULL)
  {
    log_error("%s", "Command is NULL");
    return false;
  }

  pid_t pid;
  char *shell = getenv("SHELL");
  if (!shell)
  {
    shell = "/bin/sh";
  }

  char *cres   = cmdstr(c);
  char *argv[] = {shell, "-c", cres, NULL};
  free(cres);
  int status;

  cmd_print(c);
  printf(_TermColHint);

  pid = fork();
  if (pid == -1)
  {
    char *msg = _build_h_errmsg(errno);
    log_error("%s: %s\n", "fork", msg);
    return false;
  }
  else if (pid == 0)
  {
    if (execvp(shell, argv) == -1)
    {
      char *msg = _build_h_errmsg(errno);
      log_error("%s: %s\n", "execvp", msg);
      return false;
    }
  }
  else
  {
    if (waitpid(pid, &status, 0) == -1)
    {
      char *msg = _build_h_errmsg(errno);
      log_error("%s: %s\n", "waitpid", msg);
      return false;
    }

    if (WIFEXITED(status))
    {
      printf(_TermCol0);
      return WEXITSTATUS(status) == EXIT_SUCCESS;
    }
    else
    {
      return false;
    }
  }
  return false;
#endif
}

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
