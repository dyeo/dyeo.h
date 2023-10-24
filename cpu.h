/*
# cpu.h - simple 8-register cpu vm
*/
#ifndef _CPU_H
#define _CPU_H

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define strdup _strdup
#endif

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef clamp
#define clamp(x, MIN, MAX) max(MIN, min(x, MAX))
#endif

// -----------------------------------------------------------------------------

#define X_LIST_REGISTERS                                                       \
  X(ip)                                                                        \
  X(sp)                                                                        \
  X(r0)                                                                        \
  X(r1)                                                                        \
  X(r2)                                                                        \
  X(r3)                                                                        \
  X(r4)                                                                        \
  X(r5)                                                                        \
  X(r6)                                                                        \
  X(r7)                                                                        \
  X(r8)                                                                        \
  X(r9)                                                                        \
  X(i0)                                                                        \
  X(i1)                                                                        \
  X(o0)                                                                        \
  X(o1)

#define X_LIST_GREGISTERS                                                      \
  X(r0)                                                                        \
  X(r1)                                                                        \
  X(r2)                                                                        \
  X(r3)                                                                        \
  X(r4)                                                                        \
  X(r5)                                                                        \
  X(r6)                                                                        \
  X(r7)                                                                        \
  X(r8)                                                                        \
  X(r9)

#define X_LIST_OPS                                                             \
  X(nop) /* noop */                                                            \
  /**/                                                                         \
  X(set, u8, u64) /* reg[$0] = val */                                          \
  X(mov, u8, u8)  /* reg[$0] = reg[$1] */                                      \
  X(lod, u8, u64) /* reg[$0] = mem[$1] */                                      \
  X(str, u64, u8) /* mem[$0] = reg[$1] */                                      \
  /**/                                                                         \
  X(hlt) /* end the execution */

// -----------------------------------------------------------------------------

#define CONCAT(A, B) _CONCAT(A, B)
#define _CONCAT(A, B) A##B

#if defined(__GNUC__) || defined(__clang__)
#define _ARGC(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define ARGC(...) _ARGC(_, ##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define HASARGS(...) _ARGC(_, ##__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#else
#define _ARGC(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define ARGC(...) _ARGC(_, __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define HASARGS(...) _ARGC(_, __VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#endif

#define _YARGS_0(...)
#define _YARGS_1(_1, ...) Y(_1)
#define _YARGS_2(_1, _2, ...) Y(_1) Y(_2)
#define _YARGS_3(_1, _2, _3, ...) Y(_1) Y(_2) Y(_3)
#define _YARGS_4(_1, _2, _3, _4, ...) Y(_1) Y(_2) Y(_3) Y(_4)
#define _YARGS_5(_1, _2, _3, _4, _5, ...) Y(_1) Y(_2) Y(_3) Y(_4) Y(_5)
#define _YARGS_6(_1, _2, _3, _4, _5, _6, ...)                                  \
  Y(_1) Y(_2) Y(_3) Y(_4) Y(_5) Y(_6)
#define _YARGS_7(_1, _2, _3, _4, _5, _6, _7, ...)                              \
  Y(_1) Y(_2) Y(_3) Y(_4) Y(_5) Y(_6) Y(_7)
#define _YARGS_8(_1, _2, _3, _4, _5, _6, _7, _8, ...)                          \
  Y(_1) Y(_2) Y(_3) Y(_4) Y(_5) Y(_6) Y(_7) Y(_8)
#define _YARGS_9(_1, _2, _3, _4, _5, _6, _7, _8, _9, ...)                      \
  Y(_1) Y(_2) Y(_3) Y(_4) Y(_5) Y(_6) Y(_7) Y(_8) Y(_9)
#define _YARGS_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...)                \
  Y(_1) Y(_2) Y(_3) Y(_4) Y(_5) Y(_6) Y(_7) Y(_8) Y(_9) Y(_10)

#define YARGS_N(N, ...) CONCAT(_YARGS_, N)(__VA_ARGS__)
#define YARGS(...) YARGS_N(ARGC(__VA_ARGS__), __VA_ARGS__)

#define _EARGS_0(...)
#define _EARGS_1(_1, ...) E(0, _1)
#define _EARGS_2(_1, _2, ...) E(0, _1) E(1, _2)
#define _EARGS_3(_1, _2, _3, ...) E(0, _1) E(1, _2) E(2, _3)
#define _EARGS_4(_1, _2, _3, _4, ...) E(0, _1) E(1, _2) E(2, _3) E(3, _4)
#define _EARGS_5(_1, _2, _3, _4, _5, ...)                                      \
  E(0, _1) E(1, _2) E(2, _3) E(3, _4) E(4, _5)
#define _EARGS_6(_1, _2, _3, _4, _5, _6, ...)                                  \
  E(0, _1) E(1, _2) E(2, _3) E(3, _4) E(4, _5) E(5, _6)
#define _EARGS_7(_1, _2, _3, _4, _5, _6, _7, ...)                              \
  E(0, _1) E(1, _2) E(2, _3) E(3, _4) E(4, _5) E(5, _6) E(6, _7)
#define _EARGS_8(_1, _2, _3, _4, _5, _6, _7, _8, ...)                          \
  E(0, _1) E(1, _2) E(2, _3) E(3, _4) E(4, _5) E(5, _6) E(6, _7) E(7, _8)
#define _EARGS_9(_1, _2, _3, _4, _5, _6, _7, _8, _9, ...)                      \
  E(0, _1)                                                                     \
  E(1, _2) E(2, _3) E(3, _4) E(4, _5) E(5, _6) E(6, _7) E(7, _8) E(8, _9)
#define _EARGS_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...)                \
  E(0, _1)                                                                     \
  E(1, _2)                                                                     \
  E(2, _3) E(3, _4) E(4, _5) E(5, _6) E(6, _7) E(7, _8) E(8, _9) E(9, _10)

#define EARGS_N(N, ...) CONCAT(_EARGS_, N)(__VA_ARGS__)
#define EARGS(...) EARGS_N(ARGC(__VA_ARGS__), __VA_ARGS__)

#define _GEN_ARGS_0()
#define _GEN_ARGS_1() arg0
#define _GEN_ARGS_2() arg0, arg1
#define _GEN_ARGS_3() arg0, arg1, arg2
#define _GEN_ARGS_4() arg0, arg1, arg2, arg3
#define _GEN_ARGS_5() arg0, arg1, arg2, arg3, arg4
#define _GEN_ARGS_6() arg0, arg1, arg2, arg3, arg4, arg5
#define _GEN_ARGS_7() arg0, arg1, arg2, arg3, arg4, arg5, arg6
#define _GEN_ARGS_8() arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7
#define _GEN_ARGS_9() arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8
#define _GEN_ARGS_10()                                                         \
  arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9

#define GEN_ARGS(N) CONCAT(_GEN_ARGS_, N)()

// -----------------------------------------------------------------------------

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define _FtoU(d) (*(u64 *) &(d))
#define _UtoF(i) (*(double *) &(i))

// -----------------------------------------------------------------------------

typedef enum cpu_reg
{
#define X(NAME, ...) op_##NAME,
  X_LIST_REGISTERS
#undef X
    CPU_REGISTER_COUNT
} cpu_reg;

const char *_reg_toks[] = {
#define X(NAME, ...) #NAME,
  X_LIST_REGISTERS
#undef X
};

#define CPU_STACK_SIZE 2048

typedef struct cpu
{
  union
  {
    u64 reg[CPU_REGISTER_COUNT];
    struct
    {
      u64 ip;
      u64 sp;
      u64 r[CPU_REGISTER_COUNT - 6];
      u64 i0;
      u64 i1;
      u64 o0;
      u64 o1;
    };
  };
  u64 stk[CPU_STACK_SIZE];
  u64 memc;
  u8 *mem;
} *cpu;

u8 *asm_compile(const char *code, u64 *out_length);

extern cpu cpu_new(u64 memc, u8 *mem);
extern void cpu_run(cpu c);
extern void cpu_step(cpu c);

// -----------------------------------------------------------------------------

typedef enum cpu_op
{
#define X(NAME, ...) op_##NAME,
  X_LIST_OPS
#undef X
    CPU_OP_COUNT
} cpu_op;

#define Z(...) CONCAT(Z_, HASARGS(__VA_ARGS__))(__VA_ARGS__)
#define Z_0(...)
#define Z_1(...) , __VA_ARGS__
#define X(NAME, ...) extern void _op_##NAME(cpu c Z(__VA_ARGS__));
X_LIST_OPS

#undef X
#undef Z_1
#undef Z_0
#undef Z

#define X(NAME, ...) ARGC(__VA_ARGS__),
u8 _op_argc[] = {X_LIST_OPS};
#undef X

#define Z(...) CONCAT(Z_, HASARGS(__VA_ARGS__))(__VA_ARGS__)
#define Z_0(...) 0
#define Z_1(...) __VA_ARGS__
u8 _op_lens[][4] = {
#define u8 1
#define u16 2
#define u32 4
#define u64 8
#define X(NAME, ...) {Z(__VA_ARGS__)},
  X_LIST_OPS
#undef X

#undef u8
#undef u16
#undef u32
#undef u64
};
#undef Z_1
#undef Z_0
#undef Z

const char *_op_toks[] = {
#define X(NAME, ...) #NAME,
  X_LIST_OPS
#undef X

};

// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif

// -----------------------------------------------------------------------------

#ifdef CPU_IMPLEMENTATION
#ifndef _CPU_C
#define _CPU_C

#define _popv(CPU, TYPE) CONCAT(__popv_, TYPE)(CPU)
#define _peekv(CPU, TYPE) CONCAT(__peekv_, TYPE)(CPU)

#define __popv_8 __popv_u64
#define __popv_u64(CPU)                                                        \
  (((u64) (CPU)->mem[(CPU)->ip++] << 56) |                                     \
   ((u64) (CPU)->mem[(CPU)->ip++] << 48) |                                     \
   ((u64) (CPU)->mem[(CPU)->ip++] << 40) |                                     \
   ((u64) (CPU)->mem[(CPU)->ip++] << 32) |                                     \
   ((u64) (CPU)->mem[(CPU)->ip++] << 24) |                                     \
   ((u64) (CPU)->mem[(CPU)->ip++] << 16) |                                     \
   ((u64) (CPU)->mem[(CPU)->ip++] << 8) |                                      \
   ((u64) (CPU)->mem[(CPU)->ip++] << 0))

#define __popv_4 __popv_u32
#define __popv_u32(CPU)                                                        \
  (((u32) (CPU)->mem[(CPU)->ip++] << 24) |                                     \
   ((u32) (CPU)->mem[(CPU)->ip++] << 16) |                                     \
   ((u32) (CPU)->mem[(CPU)->ip++] << 8) |                                      \
   ((u32) (CPU)->mem[(CPU)->ip++] << 0))

#define __popv_2 __popv_u16
#define __popv_u16(CPU)                                                        \
  (((u16) (CPU)->mem[(CPU)->ip++] << 8) | ((u16) (CPU)->mem[(CPU)->ip++] << 0))

#define __popv_1 _popv_u8
#define __popv_u8(C) ((u8) (C)->mem[(C)->ip++])

#define _pushv(ARR, INDEX, VALUE, LEN)                                         \
  do                                                                           \
  {                                                                            \
    for (u64 i = 0; i < LEN; i++)                                              \
    {                                                                          \
      (ARR)[INDEX + i] = (u8) ((VALUE) >> (8 * (LEN - 1 - i)));                \
    }                                                                          \
  } while (0)

#define int_8_t u64
#define int_4_t u32
#define int_2_t u16
#define int_1_t u8

#define CPU_GET(TYPE, NAME) TYPE NAME = _popv(c, TYPE)

bool _isdigit(const char *start, const size_t length)
{
  size_t i = 0;
  while (i < length && isdigit(start[i]))
  {
    i++;
  }
  return i == length;
}

char **_asm_tokenize(const char *s, size_t *num_tokens)
{
  int i, j, length, start, end, token_length, token_count = 0;
  length = strlen(s);
  for (i = 0; i < length; i++)
  {
    if (!isspace(s[i]) && (i == 0 || isspace(s[i - 1])))
    {
      token_count++;
    }
  }
  char **tokens   = (char **) malloc(sizeof(char *) * token_count);
  int token_index = 0;
  for (i = 0; i < length; i++)
  {
    if (!isspace(s[i]) && (i == 0 || isspace(s[i - 1])))
    {
      start = i;
      while (i < length && !isspace(s[i]))
      {
        i++;
      }
      end = i;

      token_length        = end - start;
      tokens[token_index] = (char *) malloc(sizeof(char) * (token_length + 1));
      for (j = 0; j < token_length; j++)
      {
        tokens[token_index][j] = s[start + j];
      }
      tokens[token_index][token_length] = '\0';

      token_index++;
    }
  }
  *num_tokens = token_count;
  return tokens;
}

u8 _asm_op(const char *token)
{
  for (u8 i = 0; i < CPU_OP_COUNT; ++i)
  {
    if (strcmp(_op_toks[i], token) == 0)
    {
      return i;
    }
  }
  fprintf(stderr, "ERROR: Expected opcode, got '%s\n", token);
  exit(1);
  return 0;
}

u64 _asm_arg(const char *token)
{
  for (u8 i = 0; i < CPU_REGISTER_COUNT; ++i)
  {
    if (strcmp(_reg_toks[i], token) == 0)
    {
      return i;
    }
  }
  return strtoull(token, NULL, 0);
}

u8 *asm_compile(const char *code, u64 *out_length)
{
  if (!code || !out_length)
  {
    return NULL;
  }
  u64 bytec      = 32;
  u8 *bytes      = calloc(bytec, sizeof(u8));
  size_t ntokens = 0;
  char **tokens  = _asm_tokenize(code, &ntokens);
  u64 i          = 0;
  while (i < ntokens)
  {
    u8 op              = _asm_op(tokens[i++]);
    bytes[*out_length] = op;
    *out_length += 1;
    for (int j = 0; j < _op_argc[op]; ++j)
    {
      if (i >= ntokens)
      {
        fprintf(stderr, "ERROR: Expected argument, got EOF\n");
        exit(1);
      }
      u8 alen = _op_lens[op][j];
      u64 arg = _asm_arg(tokens[i++]);
      _pushv(bytes, *out_length, arg, alen);
      *out_length += alen;
    }
  }
  free(tokens);
  bytes              = realloc(bytes, (*out_length + 1) * sizeof(u8));
  bytes[*out_length] = 0;
  return bytes;
}

cpu cpu_new(u64 memc, u8 *mem)
{
  if (memc == 0 || !mem)
  {
    fprintf(stderr, "ERROR: CPU requires memory to function\n");
    exit(1);
  }
  cpu c = malloc(sizeof(struct cpu));
  memset(c->reg, 0, sizeof(u64) * CPU_REGISTER_COUNT);
  memset(c->stk, 0, sizeof(u64) * CPU_STACK_SIZE);
  c->ip   = 0;
  c->sp   = 0;
  c->memc = memc;
  c->mem  = mem;
  return c;
}

void cpu_run(cpu c)
{
  while (c->ip < c->memc)
  {
    cpu_step(c);
  }
}

#define E(N, V) CPU_GET(V, arg##N);
#define _X_0(NAME, ...) _op_##NAME(c)
#define _X_1(NAME, ...) _op_##NAME(c, GEN_ARGS(ARGC(__VA_ARGS__)))
#define X(NAME, ...)                                                           \
  case op_##NAME:                                                              \
  {                                                                            \
    EARGS(__VA_ARGS__)                                                         \
    CONCAT(_X_, HASARGS(__VA_ARGS__))(NAME, __VA_ARGS__);                      \
    break;                                                                     \
  }
void cpu_step(cpu c)
{
  u8 op = _popv(c, u8);
  switch (op)
  {
    X_LIST_OPS
    default:
      break;
  }
}
#undef X
#undef _X_0
#undef _X_1
#undef Y

void _op_hlt(cpu c)
{
  c->ip = c->memc;
}

void _op_nop(cpu c)
{
  (void) c;
}

void _op_mov(cpu c, u8 dst, u8 src)
{
  c->reg[dst] = c->reg[src];
}

void _op_set(cpu c, u8 dst, u64 src)
{
  c->reg[dst] = src;
}

void _op_lod(cpu c, u8 dst, u64 src)
{
  c->reg[dst] = c->mem[src];
}

void _op_str(cpu c, u64 dst, u8 src)
{
  c->mem[dst] = c->reg[src];
}

#endif
#endif
/*
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