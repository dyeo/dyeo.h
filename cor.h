/*
# cor.h

## Description

The `cor.h` library provides a set of macros and types that assist in writing
and managing coroutines in C. These utilities allow for writing coroutines in a
manner that closely resembles a state machine, making it easier to understand
and maintain asynchronous logic.

## API Documentation

### Enums and Typedefs

- `corstatus_t`:
  - An enumeration representing the status of a coroutine's execution.
  - Values:
    - `cor_yielded`: The coroutine yielded execution.
    - `cor_success`: The coroutine completed successfully.
    - `cor_failure`: The coroutine completed with a failure.

- `corstate_t`:
  - A struct that maintains the state of a coroutine, including its yield point
and the number of runs.

- `corresult_t`:
  - A struct representing the result of a coroutine's execution.
  - Fields:
    - `corstatus_t status`: Status of the coroutine's execution.
    - `size_t yield`: Current yield point.
    - `void *result`: Pointer to the result data.

### Coroutine Control Macros

- `cor_start()`: Marks the start of a coroutine.
- `cor_yield(...)`: Causes the coroutine to yield its execution.
- `cor_return(Result)`: Returns from the coroutine with a success status and the
provided result.
- `cor_break(Result)`: Returns from the coroutine with a failure status and the
provided result.
- `cor_end()`: Marks the end of the coroutine.

### Utility Macros

- `cor_reset(CorT)`: Resets the coroutine's state.
- `cor_t(CorName)`: Defines a type for a coroutine's state structure.
- `cor_init(...)`: Initializes a coroutine's state.

### Coroutine Function Definitions

- `cor_0_t(CorName, T0)`: Defines a coroutine that takes no arguments.
- `cor_1_t(CorName, T0)`: Defines a coroutine that takes one argument.
- `cor_2_t(CorName, T0, T1)`: Defines a coroutine that takes two arguments.
- `cor_3_t(CorName, T0, T1, T2)`: Defines a coroutine that takes three
arguments.
- `cor_4_t(CorName, T0, T1, T2, T3)`: Defines a coroutine that takes four
arguments.

### Miscellaneous

- Several internal macros are also defined (`COR_EXPAND`, `COR_CONCAT`, etc.) to
assist with macro expansion, argument counting, and other utility operations.
These are primarily used to simplify the implementation of the above APIs and
are not typically used directly.

## Usage

To use the coroutine utilities, include the `cor.h` header, and define your
coroutines using the provided macros. When the coroutine yields, you can capture
its state and resume it later.
*/
#ifndef _COR_H
#define _COR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define COR_EXPAND(X) X

#define COR_CONCAT(X, Y) COR_CONCAT_IMPL(X, Y)
#define COR_CONCAT_IMPL(X, Y) X##Y

#define COR_ARGC(...) COR_ARGC_IMPL(__VA_ARGS__, 4, 3, 2, 1, 0)
#define COR_ARGC_IMPL(_4, _3, _2, _1, N, ...) N

#define COR_BRACES(...)                                                        \
  COR_CONCAT(COR_EXPAND(COR_BRACES_), COR_ARGC(__VA_ARGS__))(__VA_ARGS__)
#define COR_BRACES_0()
#define COR_BRACES_1(X) (X)
#define COR_BRACES_2(X, Y) (X), (Y)
#define COR_BRACES_3(X, Y, Z) (X), (Y), (Z)
#define COR_BRACES_4(X, Y, Z, W) (X), (Y), (Z), (W)

typedef enum
{
  cor_yielded,
  cor_success,
  cor_failure
} corstatus_t;

typedef struct
{
  size_t yield;
  size_t runs;
} corstate_t;

typedef struct
{
  corstatus_t status;
  size_t yield;
  void *result;
} corresult_t;

#define core_state_n_t(InnerStruct)                                            \
  struct                                                                       \
  {                                                                            \
    corstate_t state;                                                          \
    struct InnerStruct;                                                        \
  }

#define cor_start()                                                            \
  {                                                                            \
    if ((cor_state) != NULL)                                                   \
    {                                                                          \
      switch ((cor_state)->yield)                                              \
      {                                                                        \
        case 0:

#if __has_attribute(__fallthrough__)
#define cor_fallthrough                                                        \
  do                                                                           \
  {                                                                            \
  } while (0);                                                                 \
  __attribute__((__fallthrough__))
#else
#define cor_fallthrough                                                        \
  do                                                                           \
  {                                                                            \
  } while (0);                                                                 \
// fall through
#endif

#define cor_yield(/*StateVal, Condition, Result*/...)                          \
  cor_yield_impl(__VA_ARGS__, NULL)
#define cor_yield_impl(StateVal, Condition, Result, ...)                       \
  do                                                                           \
  {                                                                            \
    if ((Condition))                                                           \
    {                                                                          \
      corresult_t cor_result = {cor_yielded, (size_t) (StateVal),              \
                                (void *) (Result)};                            \
      return cor_result;                                                       \
    }                                                                          \
  } while (0);                                                                 \
  cor_fallthrough;                                                             \
  case (StateVal):

#define cor_return(Result) cor_return_impl(Result, (void) 0)
#define cor_return_impl(Result, ...)                                           \
  do                                                                           \
  {                                                                            \
    __VA_ARGS__;                                                               \
    corresult_t cor_result = {cor_success, 0llu, (void *) (Result)};           \
    return cor_result;                                                         \
  } while (0)

#define cor_break(Result) cor_break_impl(Result, (void) 0)
#define cor_break_impl(Result, ...)                                            \
  do                                                                           \
  {                                                                            \
    __VA_ARGS__;                                                               \
    corresult_t cor_result = {cor_failure, 0llu, (void *) (Result)};           \
    return cor_result;                                                         \
  } while (0)

#define cor_end()                                                              \
  }                                                                            \
  cor_break(NULL);                                                             \
  }                                                                            \
  cor_break(NULL);                                                             \
  }

#define cor_reset(CorT) ((CorT).yield = 0, (CorT).runs += 1)

#define cor_t(CorName) cor_##CorName##_state_t

static inline corresult_t _cor_apply_result(corstate_t *state,
                                            corresult_t result)
{
  if (result.status != cor_yielded)
  {
    state->runs += 1;
  }
  state->yield = result.yield;
  return result;
}

#define _cor_call_impl(CorName, CorT, CorCall)                                 \
  _cor_apply_result(&((CorT).state), CorCall(CorName, CorT))
#define _cor_runsync_impl(CorPtr, CorT, CorCall)                               \
  do                                                                           \
  {                                                                            \
    corresult_t cor_result;                                                    \
    do                                                                         \
    {                                                                          \
      cor_result = CorCall(CorPtr, CorT);                                      \
    } while (cor_result.status == cor_yielded);                                \
  } while (0)

#define cor_init(...)                                                          \
  {                                                                            \
    (0llu), (0llu), COR_BRACES(__VA_ARGS__)                                    \
  }

#define cor_0_t(CorName, T0)                                                   \
  typedef core_state_n_t({}) cor_t(CorName);                                   \
  typedef corresult_t (*cor_##CorName##_fptr_t)(const corstate_t *cor_state)
#define cor_0_call_impl(CorName, CorT) ((CorPtr) CorName(&((CorT).state)))
#define cor_0_call(CorName, CorT) _cor_call_impl(CorName, CorT, cor_0_call_impl)
#define cor_0_runs(CorPtr, CorT) _cor_runsync_impl(CorPtr, CorT, cor_0_call)
#define cor_0_fptr(CorName) corresult_t CorName(const corstate_t *cor_state)
#define cor_0_func(CorName) corresult_t (*CorName)(const corstate_t *cor_state)

#define cor_1_t(CorName, T0)                                                   \
  typedef core_state_n_t({ T0 arg0; }) cor_t(CorName);                         \
  typedef corresult_t (*cor_##CorName##_fptr_t)(const corstate_t *cor_state,   \
                                                T0 arg0)
#define cor_1_call_impl(CorPtr, CorT) ((CorPtr) (&((CorT).state), (CorT).arg0))
#define cor_1_call(CorName, CorT) _cor_call_impl(CorName, CorT, cor_1_call_impl)
#define cor_1_runs(CorPtr, CorT) _cor_runsync_impl(CorPtr, CorT, cor_1_call)
#define cor_1_func(CorName, T0)                                                \
  corresult_t(CorName)(const corstate_t *cor_state, T0 arg0)

#define cor_2_t(CorName, T0, T1)                                               \
  typedef core_state_n_t({                                                     \
    T0 arg0;                                                                   \
    T1 arg1;                                                                   \
  }) cor_t(CorName);                                                           \
  typedef corresult_t (*cor_##CorName##_fptr_t)(const corstate_t *cor_state,   \
                                                T0 arg0, T1 arg1)
#define cor_2_call_impl(CorPtr, CorT)                                          \
  ((CorPtr) (&((CorT).state), (CorT).arg0, (CorT).arg1))
#define cor_2_call(CorPtr, CorT) _cor_call_impl(CorName, CorT, cor_2_call_impl)
#define cor_2_runs(CorPtr, CorT) _cor_runsync_impl(CorPtr, CorT, cor_2_call)
#define cor_2_func(CorName, T0, T1)                                            \
  corresult_t(CorName)(const corstate_t *cor_state, T0 arg0, T1 arg1)

#define cor_3_t(CorName, T0, T1, T2)                                           \
  typedef core_state_n_t({                                                     \
    T0 arg0;                                                                   \
    T1 arg1;                                                                   \
    T2 arg2;                                                                   \
  }) cor_t(CorName);                                                           \
  typedef corresult_t (*cor_##CorName##_fptr_t)(const corstate_t *cor_state,   \
                                                T0 arg0, T1 arg1, T2 arg2)
#define cor_3_call_impl(CorPtr, CorT)                                          \
  ((CorPtr) (&((CorT).state), (CorT).arg0, (CorT).arg1, (CorT).arg2))
#define cor_3_call(CorPtr, CorT) _cor_call_impl(CorName, CorT, cor_3_call_impl)
#define cor_3_runs(CorPtr, CorT) _cor_runsync_impl(CorPtr, CorT, cor_3_call)
#define cor_3_func(CorName, T0, T1, T2)                                        \
  corresult_t(CorName)(const corstate_t *cor_state, T0 arg0, T1 arg1, T2 arg2)

#define cor_4_t(CorName, T0, T1, T2, T3)                                       \
  typedef core_state_n_t({                                                     \
    T0 arg0;                                                                   \
    T1 arg1;                                                                   \
    T2 arg2;                                                                   \
    T3 arg3;                                                                   \
  })                                                                           \
    cor_t(CorName) typedef corresult_t (*cor_##CorName##_fptr_t)(              \
      const corstate_t *cor_state, T0 arg0, T1 arg1, T2 arg2, T3 arg3)
#define cor_4_call_impl(CorPtr, CorT)                                          \
  ((CorPtr) (&((CorT).state), (CorT).arg0, (CorT).arg1, (CorT).arg2,           \
             (CorT).arg3))
#define cor_4_call(CorPtr, CorT) _cor_call_impl(CorName, CorT, cor_4_call_impl)
#define cor_4_runs(CorPtr, CorT) _cor_runsync_impl(CorPtr, CorT, cor_4_call)
#define cor_4_func(CorName, T0, T1, T2, T3)                                    \
  corresult_t(CorName)(const corstate_t *cor_state, T0 arg0, T1 arg1, T2 arg2, \
                       T3 arg3)

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
dedicates any and all copyright interest in the Software to the public domain.
The author makes this dedication for the benefit of the public at large and to
the detriment of the author's heirs and successors. The author intends this
dedication to be an overt act of relinquishment in perpetuity of all present and
future rights to the Software under copyright law.

Any person obtaining a copy of the Software and associated documentation files
is free to to deal in the Software without restriction, including without
limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to no conditions.
--------------------------------------------------------------------------------
*/
