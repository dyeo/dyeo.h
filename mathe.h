#ifndef _MATHE_H
#define _MATHE_H

// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MATHE_PRECISION
#define MATHE_PRECISION double
#endif

#define REAL MATHE_PRECISION

// -----------------------------------------------------------------------------

extern REAL mathe(const char *expression);

// -----------------------------------------------------------------------------

#undef REAL

#ifdef __cplusplus
}
#endif // _MATHE_H

// -----------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------

#ifdef MATHE_IMPLEMENTATION
#ifndef _MATHE_C
#define _MATHE_C

#ifndef MATHE_PRECISION
#define MATHE_PRECISION double
#endif

#define REAL MATHE_PRECISION

// -----------------------------------------------------------------------------

#define X_OPS_LIST                                                             \
  X(lpr)                                                                       \
  X(rpr)                                                                       \
  X(min)                                                                       \
  X(fun)                                                                       \
  X(exp)                                                                       \
  X(mlt)                                                                       \
  X(div)                                                                       \
  X(mod)                                                                       \
  X(add)                                                                       \
  X(sub)

#define X(V) me_##V,
typedef enum _me_op
{
  X_OPS_LIST
} _me_op;
#undef X

#define X(V) #V,
const char *_op_names[] = {X_OPS_LIST};
#undef X

typedef struct _me_tok
{
  bool isop;
  union
  {
    struct
    {
      _me_op op;
      REAL (*fun)(REAL);
    };
    REAL val;
  };
} _me_tok;

#define RESIZE(ARRAY, LENGTH, CAPACITY, AMOUNT)                                \
  if (LENGTH + AMOUNT > CAPACITY)                                              \
  {                                                                            \
    CAPACITY *= 2;                                                             \
    ARRAY = realloc(tokens, CAPACITY * sizeof(_me_tok));                       \
  }

#define PUSH_VAL(VAL, LEN)                                                     \
  do                                                                           \
  {                                                                            \
    RESIZE(tokens, *toklen, tokcap, 1);                                        \
    if (!tokens)                                                               \
    {                                                                          \
      return NULL;                                                             \
    }                                                                          \
    tokens[*toklen].isop = false;                                              \
    tokens[*toklen].val  = VAL;                                                \
    *toklen += 1;                                                              \
    i += LEN;                                                                  \
  } while (0)

#define PUSH_OP(OP)                                                            \
  do                                                                           \
  {                                                                            \
    RESIZE(tokens, *toklen, tokcap, 1);                                        \
    if (!tokens)                                                               \
    {                                                                          \
      return NULL;                                                             \
    }                                                                          \
    tokens[*toklen].isop = true;                                               \
    tokens[*toklen].op   = OP;                                                 \
    *toklen += 1;                                                              \
    i += 1;                                                                    \
  } while (0)

#define PUSH_FUN(FPTR, LEN)                                                    \
  do                                                                           \
  {                                                                            \
    RESIZE(tokens, *toklen, tokcap, 1);                                        \
    if (!tokens)                                                               \
    {                                                                          \
      return NULL;                                                             \
    }                                                                          \
    tokens[*toklen].isop = true;                                               \
    tokens[*toklen].op   = me_fun;                                             \
    tokens[*toklen].fun  = FPTR;                                               \
    *toklen += 1;                                                              \
    i += LEN;                                                                  \
  } while (0)

#define _strncmp(StrA, StrB, StrN)                                             \
  (StrN == strlen(StrB) && !memcmp(StrA, StrB, StrN))

_me_tok *_me_tokenize(const char *expr, size_t *toklen)
{
  size_t tokcap   = 32;
  _me_tok *tokens = malloc(tokcap * sizeof(_me_tok));
  *toklen         = 0;
  size_t i        = 0;
  while (expr[i])
  {
    while (isspace(expr[i]))
    {
      i++;
    }
    if (expr[i] == '\0')
      break;
    _me_tok tok = {false, {.val = 0.0}};
    if (isalpha(expr[i]))
    {
      const char *token = expr + i;
      size_t tlen       = 0;
      while (token[tlen] && isalpha(token[tlen]))
      {
        tlen += 1;
      }
      if (_strncmp(token, "pi", tlen))
      {
        PUSH_VAL(3.141592653589793, tlen);
        continue;
      }
      if (_strncmp(token, "sin", tlen))
      {
        PUSH_FUN(sin, tlen);
        continue;
      }
      if (_strncmp(token, "cos", tlen))
      {
        PUSH_FUN(sin, tlen);
        continue;
      }
      if (_strncmp(token, "tan", tlen))
      {
        PUSH_FUN(sin, tlen);
        continue;
      }
      if (_strncmp(token, "sqrt", tlen))
      {
        PUSH_FUN(sqrt, tlen);
        continue;
      }
    }
    if (!tok.isop)
    {
      switch (expr[i])
      {
        case '(':
          PUSH_OP(me_lpr);
          continue;
        case ')':
          PUSH_OP(me_rpr);
          continue;
        case '^':
          PUSH_OP(me_exp);
          continue;
        case '*':
          PUSH_OP(me_mlt);
          continue;
        case '/':
          PUSH_OP(me_div);
          continue;
        case '%':
          PUSH_OP(me_mod);
          continue;
        case '+':
          PUSH_OP(me_add);
          continue;
        case '-':
          if (i == 0 || (i != 0 && (tokens[*toklen - 1].isop &&
                                    tokens[*toklen - 1].op != me_rpr)))
          {
            PUSH_OP(me_min);
          }
          else
          {
            PUSH_OP(me_sub);
          }
          continue;
      }
    }
    if (!tok.isop)
    {
      char *end  = NULL;
      double val = strtod(expr + i, &end);
      PUSH_VAL(val, end - (expr + i));
      continue;
    }
    i++;
  }
  tokens = realloc(tokens, *toklen * sizeof(_me_tok));
  return tokens;
}

_me_tok *_me_shuntingyard(_me_tok *tokens, const size_t toklen, size_t *outlen)
{
  size_t slen = 0, olen = 0;
  _me_tok *stack  = malloc(toklen * sizeof(_me_tok));
  _me_tok *output = malloc(toklen * sizeof(_me_tok));
  size_t i        = 0;
  while (i < toklen)
  {
    if (!tokens[i].isop)
    {
      output[olen++] = tokens[i++];
    }
    else if (tokens[i].op == me_min || tokens[i].op == me_lpr ||
             tokens[i].op == me_fun)
    {
      stack[slen++] = tokens[i++];
    }
    else if (tokens[i].op == me_rpr)
    {
      while (slen && stack[slen - 1].op != me_lpr)
      {
        output[olen++] = stack[--slen];
      }
      slen--;
      i++;
      if (slen && stack[slen - 1].op == me_fun)
      {
        output[olen++] = stack[--slen];
      }
    }
    else
    {
      while (slen && stack[slen - 1].op != me_lpr &&
             stack[slen - 1].op <= tokens[i].op)
      {
        output[olen++] = stack[--slen];
      }
      stack[slen++] = tokens[i++];
    }
  }
  while (slen)
  {
    output[olen++] = stack[--slen];
  }
  free(stack);
  _me_tok *resized_output = realloc(output, olen * sizeof(_me_tok));
  if (!resized_output)
  {
    free(output);
    return NULL;
  }
  if (outlen)
  {
    *outlen = olen;
  }
  free(tokens);
  return resized_output;
}

REAL mathe(const char *expr)
{
  size_t count    = 0;
  _me_tok *tokens = _me_tokenize(expr, &count);
  tokens          = _me_shuntingyard(tokens, count, &count);
  size_t slen     = 0;
  REAL *stack     = calloc(count, sizeof(REAL));
  for (size_t i = 0; i < count; ++i)
  {
    if (!tokens[i].isop)
    {
      stack[slen++] = tokens[i].val;
    }
    else
    {
      switch (tokens[i].op)
      {
        default:
          break;
        case me_fun:
          if (slen < 1)
          {
            free(stack);
            free(tokens);
            return NAN;
          }
          stack[slen - 1] = tokens[i].fun(stack[slen - 1]);
          continue;
        case me_min:
          if (slen < 1)
          {
            free(stack);
            free(tokens);
            return NAN;
          }
          stack[slen - 1] = -stack[slen - 1];
          continue;
      }

      if (slen < 2)
      {
        free(stack);
        free(tokens);
        return NAN;
      }
      REAL r = stack[--slen];
      REAL l = stack[--slen];
      switch (tokens[i].op)
      {
        default:
          stack[slen++] = 0;
          break;
        case me_exp:
          stack[slen++] = pow(l, r);
          break;
        case me_mlt:
          stack[slen++] = l * r;
          break;
        case me_div:
          stack[slen++] = l / r;
          break;
        case me_mod:
          stack[slen++] = fmod(l, r);
          break;
        case me_add:
          stack[slen++] = l + r;
          break;
        case me_sub:
          stack[slen++] = l - r;
          break;
      }
    }
  }
  REAL result = stack[0];
  free(stack);
  free(tokens);
  return result;
}

// -----------------------------------------------------------------------------

#undef REAL

// -----------------------------------------------------------------------------

#endif // _MATHE_C
#endif // MATHE_IMPLEMENTATION
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

The author of this software and associated documentation files (the
"Software") dedicates any and all copyright interest in the Software to the
public domain. The author makes this dedication for the benefit of the public
at large and to the detriment of the author's heirs and successors. The author
intends this dedication to be an overt act of relinquishment in perpetuity of
all present and future rights to the Software under copyright law.

Any person obtaining a copy of the Software and associated documentation files
is free to to deal in the Software without restriction, including without
limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to no conditions.
--------------------------------------------------------------------------------
*/
