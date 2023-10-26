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

#ifdef MATHE_RELEASE
#define MATHE_RELEASE_MAIN main
#else
#define MATHE_RELEASE_MAIN mathe_main
#endif

// -----------------------------------------------------------------------------

REAL fsign(const REAL v)
{
  return (0 < v) - (v < 0);
}

#define mathe(...) mathe_eval(__VA_ARGS__, NULL, 0.0)
extern REAL mathe_eval(const char *expr, ...);
extern int MATHE_RELEASE_MAIN(int argc, char **argv);

// -----------------------------------------------------------------------------

#undef REAL

#ifdef __cplusplus
}
#endif // _MATHE_H

// -----------------------------------------------------------------------------

#endif

// -----------------------------------------------------------------------------

#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
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

#ifdef MATHE_RELEASE
#define MATHE_RELEASE_MAIN main
#else
#define MATHE_RELEASE_MAIN mathe_main
#endif

#define REAL MATHE_PRECISION

#ifdef MATHE_DEBUG
#if defined(__gnuc__) || defined(__clang__)
#define _me_log(FMT, ...) printf(FMT, ##__VA_ARGS__)
#else
#define _me_log(FMT, ...) printf(FMT, __VA_ARGS__)
#endif
#else
#define _me_log(FMT, ...)
#endif

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

#define X_CONSTS_LIST                                                          \
  X(pi, 3.141592653589793)                                                     \
  X(e, 2.718281828459045)                                                      \
  X(phi, 1.618033988749895)                                                    \
  X(sqrt2, 1.4142135623730951)                                                 \
  X(ln2, 0.6931471805599453)                                                   \
  X(log2e, 1.4426950408889634)                                                 \
  X(log10e, 0.4342944819032518)                                                \
  X(gamma, 0.5772156649015329)

#define X(V, ...) #V,
const char *_me_const_names[] = {X_CONSTS_LIST NULL};
#undef X
#define X(V, ...) sizeof(#V),
const size_t _me_const_namelens[] = {X_CONSTS_LIST 0};
#undef X
#define X(_, V, ...) V,
const double _me_const_vals[] = {X_CONSTS_LIST 0.0};
#undef X

#define X_FUNCS_LIST                                                           \
  X(sin, sin)                                                                  \
  X(cos, cos)                                                                  \
  X(tan, tan)                                                                  \
  X(sqrt, sqrt)                                                                \
  X(asin, asin)   /* Arcsine */                                                \
  X(acos, acos)   /* Arccosine */                                              \
  X(atan, atan)   /* Arctangent */                                             \
  X(sinh, sinh)   /* Hyperbolic Sine */                                        \
  X(cosh, cosh)   /* Hyperbolic Cosine */                                      \
  X(tanh, tanh)   /* Hyperbolic Tangent */                                     \
  X(abs, fabs)    /* Absolute Value */                                         \
  X(log, log)     /* Natural Logarithm (Base e) */                             \
  X(log10, log10) /* Common Logarithm (Base 10) */                             \
  X(exp, exp)     /* Exponential (Base e) */                                   \
  X(ceil, ceil)   /* Smallest integer not less than the argument */            \
  X(floor, floor) /* Largest integer not greater than the argument */          \
  X(round, round) /* Round to nearest integer */                               \
  X(cbrt, cbrt)   /* Cube root */                                              \
  X(sign, fsign)

#define X(V, ...) #V,
const char *_me_func_names[] = {X_FUNCS_LIST NULL};
#undef X
#define X(V, ...) sizeof(#V),
const size_t _me_func_namelens[] = {X_FUNCS_LIST 0};
#undef X
#define X(V, P, ...) P,
REAL (*const _me_func_ptrs[])(REAL) = {X_FUNCS_LIST NULL};
#undef X

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

#define streq(StrA, StrB, StrAN)                                               \
  (StrAN == strlen(StrB) && !memcmp(StrA, StrB, StrAN))

typedef struct _mathv
{
  char *token;
  double val;
} _mathv;

void _me_print_toks(_me_tok *tokens, size_t count)
{
  if (!tokens || count == 0)
  {
    _me_log("Tokens are empty or null.");
    return;
  }

  for (size_t i = 0; i < count; ++i)
  {
    if (tokens[i].isop)
    {
      if (tokens[i].op == me_fun)
      {
        for (int f = 0; _me_func_names[f] != NULL; ++f)
        {
          if (_me_func_ptrs[f] == tokens[i].fun)
          {
            _me_log("%s ", _me_func_names[f]);
            break;
          }
        }
      }
      else
      {
        _me_log("%s ", _op_names[tokens[i].op]);
      }
    }
    else
    {
      _me_log("%f ", tokens[i].val);
    }
  }
  _me_log("\n"); // Newline at the end
}

_me_tok *_me_tokenize(const char *expr, size_t *toklen, size_t varsn,
                      _mathv *vars)
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
      size_t tlen       = 1;
      while (token[tlen] && isalpha(token[tlen]) || isdigit(token[tlen]) ||
             token[tlen] == '_')
      {
        tlen += 1;
      }
      for (int f = 0; _me_func_names[f] != NULL; ++f)
      {
        if (streq(token, _me_func_names[f], tlen))
        {
          PUSH_FUN(_me_func_ptrs[f], tlen);
          goto done;
        }
      }
      for (int c = 0; _me_const_names[c] != NULL; ++c)
      {
        if (streq(token, _me_const_names[c], tlen))
        {
          PUSH_VAL(_me_const_vals[c], tlen);
          goto done;
        }
      }
      for (int v = 0; v < varsn; ++v)
      {
        if (streq(token, vars[v].token, tlen))
        {
          PUSH_VAL(vars[v].val, tlen);
          goto done;
        }
      }
    done:
      continue;
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

REAL mathe_eval(const char *expr, ...)
{
  size_t varsc = 32, varsn = 0;
  _mathv *vars = malloc(varsc * sizeof(_mathv));
  va_list args;
  va_start(args, expr);
  char *vtok  = NULL;
  double vval = 0.0;
  while ((vtok = va_arg(args, char *)) != NULL)
  {
    vval = va_arg(args, double);
    if (varsn >= varsc)
    {
      vars = realloc(vars, (varsc *= 2) * sizeof(_mathv));
      if (!vars)
      {
        fprintf(stderr, "ERROR: Memory allocation failure");
        return 0.0;
      }
    }
    vars[varsn].token = vtok;
    vars[varsn].val   = vval;
    varsn++;
  }
  va_end(args);
  vars  = realloc(vars, varsn * sizeof(_mathv));
  varsc = varsn;

  size_t count    = 0;
  _me_tok *tokens = _me_tokenize(expr, &count, varsn, vars);
  _me_print_toks(tokens, count);
  tokens = _me_shuntingyard(tokens, count, &count);
  _me_print_toks(tokens, count);
  size_t slen = 0;
  REAL *stack = calloc(count, sizeof(REAL));
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

int MATHE_RELEASE_MAIN(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s expr\n", argv[0]);
    return 1;
  }
  int len = 0;
  for (int i = 1; i < argc; i++)
  {
    len += strlen(argv[i]);
    if (i < argc - 1)
    {
      len++;
    }
  }
  len++;
  char *expr = (char *) malloc(len * sizeof(char));
  if (!expr)
  {
    perror("Failed to allocate memory");
    return 1;
  }
  strcpy(expr, argv[1]);
  for (int i = 2; i < argc; i++)
  {
    strcat(expr, " ");
    strcat(expr, argv[i]);
  }
  double result = mathe_eval(expr);
  printf("%f\n", result);
  free(expr);
  return 0;
}

// -----------------------------------------------------------------------------

#undef MATHE_RELEASE_MAIN
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
