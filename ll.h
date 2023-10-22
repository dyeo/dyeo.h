/*
# ll.h

## Description

The **LL** (Library Loader) utility is designed for runtime dynamic linking of
shared libraries (such as `.dll` on Windows or `.so` on Linux). It provides
macros and functions to load shared libraries, retrieve function pointers, and
handle dynamic symbols.

## Usage

To use this header-only library:

1. Include `ll.h` in your source code.
2. Before including, define `LL_IMPLEMENTATION` in one of your files to generate
the implementation.

```c
#define LL_IMPLEMENTATION
#include "ll.h"
```

## API Documentation

### Type Definitions

- `libptr_t`: Type representing a handle to a loaded shared library.
- `libfptr_t`: Type representing a pointer to a function loaded from a shared
library.
- `libvfptr_t`: Type representing a pointer to a function that retrieves a
variable's value.

### Macros

- `LL_LIB_EXT`: Represents the file extension for libraries (`.dll` for Windows,
`.so` for Linux).

### Functions

- `ll_lib(LibName, LibTarget, SymBlock)`: Define a shared library object with
its symbols.
- `ll_val(VarName, TValue)`: Define a variable symbol with its type.
- `ll_func(FuncName, TReturn, ...)`: Define a function symbol with its return
type and arguments.
- `ll_load_lib(LibName)`: Load a shared library.
- `ll_load_func(LibName, FuncName)`: Load a function from a shared library.
- `ll_load_var(LibName, VarName)`: Load a variable from a shared library.
- `ll_has_func(LibName, FuncName)`: Check if a function exists in a loaded
library.
- `ll_close_lib(LibName)`: Close a loaded shared library.

## Usage Examples

### 1. Loading a Library

```c
ll_lib(MyLib, "path_to_my_lib", {
    ll_func(my_function, int, const char*);
    ll_val(my_variable, int);
});
```

### 2. Using Loaded Functions and Variables

```c
if(ll_load_lib(MyLib)) {
    ll_load_func(MyLib, my_function);
    ll_load_var(MyLib, my_variable);

    int result = MyLib.my_function("test");
    int value = *(MyLib.my_variable);
}
```

## Note

- Errors in loading libraries, functions, or variables trigger the
`_ll_load_err()` function. This function prints relevant error messages to
`stderr`, aiding in debugging issues related to dynamic linking.
- Remember to close loaded libraries using `ll_close_lib(LibName)` when they are
no longer needed to free up resources.
*/
#ifndef _LL_H
#define _LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#ifdef _WIN32
#define LL_LIB_EXT ".dll"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <stdlib.h>
typedef HMODULE libptr_t;
typedef FARPROC libfptr_t;
typedef void *(*libvfptr_t)(void);
#else
#define LL_LIB_EXT ".so"
#include <dlfcn.h>
typedef void *libptr_t;
typedef void *libfptr_t;
typedef void *(*libvfptr_t)(void);
#endif

#define LL_STR(X) LL_STR_IMPL(X)
#define LL_STR_IMPL(X) #X

#define LL_CONCAT(A, B) LL_CONCAT_IMPL(A, B)
#define LL_CONCAT_IMPL(A, B) A##B

#define _ll_lib(LibName) _ll_lib_##LibName
#define _ll_handle(LibName) _ll_handle_##LibName
#define _ll_path(LibName) _ll_path_##LibName
#define _ll_vptr(LibName, VarName) LibName.VarName
#define _ll_fptr(LibName, FuncName) LibName.FuncName

#define ll_lib(LibName, LibTarget, SymBlock)                                   \
  typedef struct SymBlock _ll_lib(LibName);                                    \
  libptr_t _ll_handle(LibName)  = 0;                                           \
  const char *_ll_path(LibName) = LL_STR(LibTarget) LL_LIB_EXT;                \
  _ll_lib(LibName) LibName      = {0};

#define ll_val(VarName, TValue, ...) TValue *VarName
#define ll_func(FuncName, TReturn, ...) TReturn (*FuncName)(__VA_ARGS__)

extern void _ll_load_err(const char *symtype, const char *lib);
extern int _ll_load_lib(libptr_t *handle, const char *lib, void *alt, ...);
extern int _ll_has_func(libptr_t *handle, const char *func);
extern int _ll_load_func(libptr_t *handle, const char *func, void **fptr,
                         void **alt, ...);
extern int _ll_close_lib(libptr_t *handle);

#ifndef LL_STATIC
#define LL_PICK_STATIC
#define LL_EXCLUDE(X) X
#else
#define LL_PICK_STATIC NULL,
#define LL_EXCLUDE(X) LL_EXCLUDE_IMPL(LL_PICK_STATIC X, NULL)
#define LL_EXCLUDE_IMPL(Y, X, ...) X
#endif

#define ll_load_lib(LibName)                                                   \
  _ll_load_lib(&_ll_handle(LibName), _ll_path(LibName), NULL)

#define ll_load_func(LibName, FuncName)                                        \
  _ll_load_func(&_ll_handle(LibName), LL_STR(FuncName),                        \
                (void *) (&_ll_fptr(LibName, FuncName)),                       \
                LL_EXCLUDE((void *) &(FuncName)))

#define ll_load_var(LibName, VarName)                                          \
  _ll_load_var(&_ll_handle(LibName), LL_STR(VarName),                          \
               LL_EXCLUDE((void *) (&_ll_vptr(LibName, VarName))))

#define ll_has_func(LibName, FuncName)                                         \
  _ll_has_func(&_ll_handle(LibName), LL_STR(FuncName))

#define ll_close_lib(LibName) _ll_close_lib(&_ll_handle(LibName))

#ifdef __cplusplus
}
#endif

#endif

#ifdef LL_IMPLEMENTATION

#ifdef _WIN32
#define ll_load_lib_impl LoadLibraryA
#define ll_load_func_impl GetProcAddress
#define ll_close_lib_impl FreeLibrary

void _ll_load_err(const char *symtype, const char *lib)
{
  DWORD err    = GetLastError();
  LPSTR errmsg = NULL;
  if (err == 126)
  {
    fprintf(stderr, "ERROR: Failed to open %s: %s not found (%lu)\n", symtype,
            lib, err);
  }
  else
  {
    size_t elen = FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &errmsg, 0,
      NULL);
    if (elen == 0)
    {
      fprintf(stderr, "ERROR: Failed to open %s %s: %s (%lu)\n", symtype, lib,
              errmsg, err);
    }
    else
    {
      fprintf(stderr, "ERROR: Failed to open %s %s (%lu)\n", symtype, lib, err);
    }
  }
  LocalFree((HLOCAL) errmsg);
}

#else
#define ll_load_lib_impl dlopen
#define ll_load_func_impl dlsym
#define ll_close_lib_impl dlclose

void _ll_load_err(const char *symtype, const char *lib)
{
  int err = errno;
  if (err == 126)
  {
    fprintf(stderr, "ERROR: Failed to open %s %s: %s not found (%lu)\n",
            symtype, lib, err);
  }
  else
  {
    const char *errmsg = dlerror(err);
    if (errmsg != NULL)
    {
      fprintf(stderr, "ERROR: Failed to open %s %s: %s (%d)\n", symtype, lib,
              errmsg, err);
    }
    else
    {
      fprintf(stderr, "ERROR: Failed to open %s %s (%d)\n", symtype, lib, err);
    }
  }
}

#endif

int _ll_load_lib(libptr_t *handle, const char *lib, void *alt, ...)
{
  libptr_t result = ll_load_lib_impl(lib);
  if (result)
  {
    *handle = result;
    return 1;
  }
  if (result == 0)
  {
    _ll_load_err("library", lib);
  }
  *handle = alt;
  return 0;
}

int _ll_has_func(libptr_t *handle, const char *func)
{
  return ll_load_func_impl(*handle, func) != 0;
}

int _ll_load_var(libptr_t *handle, const char *var, void **vptr, ...)
{
  libvfptr_t result = (libvfptr_t) ll_load_func_impl(*handle, var);
  if (result)
  {
    *vptr = result();
    return 1;
  }
  _ll_load_err("variable", var);
  return 0;
}

int _ll_load_func(libptr_t *handle, const char *func, void **fptr, void **alt,
                  ...)
{
  libfptr_t result = ll_load_func_impl(*handle, func);
  if (result)
  {
    *fptr = (void *) result;
  }
  if (alt == 0)
  {
    _ll_load_err("function", func);
    return 0;
  }
  *fptr = alt;
  return 1;
}

int _ll_close_lib(libptr_t *handle)
{
  return ll_close_lib_impl(*handle) != 0;
}

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
