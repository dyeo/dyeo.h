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
  /* memory manipulation */                                                    \
  X(set, u8, u64) /* reg[$0] = val */                                          \
  X(mov, u8, u8)  /* reg[$0] = reg[$1] */                                      \
  X(lod, u8, u64) /* reg[$0] = mem[$1] */                                      \
  X(str, u64, u8) /* mem[$0] = reg[$1] */                                      \
  X(swp, u8, u8)  /* swap mem[$0] and reg[$1] */                               \
  /* stack manipulation */                                                     \
  X(psh, u8) /* push reg to stack */                                           \
  X(pek, u8) /* peek stack to reg */                                           \
  X(pop, u8) /* pop stack to reg */                                            \
  /**/                                                                         \
  X(sys, u8) /* system calls */                                                \
  X(hlt)     /* end the execution */

typedef enum sys_op
{
  sys_print,
} sys_op;

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

#define _OPTARGS_0(...)
#define _OPTARGS_1(...) , __VA_ARGS__
#define OPTARGS(...) CONCAT(_OPTARGS_, HASARGS(__VA_ARGS__))(__VA_ARGS__)

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

#define _u8_fmt "%u"
#define _u16_fmt "%u"
#define _u32_fmt "%u"
#define _u64_fmt "%llu"

#define _u8_siz 1
#define _u16_siz 2
#define _u32_siz 4
#define _u64_siz 8

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

u8 *asm_comps(const char *code, u64 *out_length);

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

#define Y(V) , V
#define X(NAME, ...) extern void _op_##NAME(cpu c YARGS(__VA_ARGS__));
X_LIST_OPS
#undef X
#undef Y

#define X(NAME, ...) ARGC(__VA_ARGS__),
u8 _op_argc[] = {X_LIST_OPS};
#undef X

u8 _op_lens[][4] = {
#define Y(V) _##V##_siz,
#define X(NAME, ...) {YARGS(__VA_ARGS__) 0},
  X_LIST_OPS
#undef X
#undef Y
};

u8 _op_xlens[] = {
#define Y(V) +_##V##_siz
#define X(NAME, ...) 0 YARGS(__VA_ARGS__),
  X_LIST_OPS
#undef X
#undef Y
};
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

#ifdef CPU_DEBUG
#define _log(FMT, ...) printf(FMT OPTARGS(__VA_ARGS__))
#define _logl(FMT, ...) printf(FMT "\n" OPTARGS(__VA_ARGS__))
#define _error(FMT, ...)                                                       \
  fprintf(stderr, "ERROR: " FMT "\n" OPTARGS(__VA_ARGS__))
#else
#define _log(FMT, ...)
#define _logl(FMT, ...)
#define _error(FMT, ...)
#endif

#define _popv(CPU, TYPE) CONCAT(__popv_, TYPE)(CPU)

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

bool _notcode(const char c)
{
  return isspace(c) || c == ';';
}

size_t _asm_count_tokens(const char *s)
{
  size_t length      = strlen(s);
  size_t token_count = 0;
  size_t i           = 0;
  while (i < length)
  {
    if (s[i] == ';')
    {
      while (s[i] != '\n' && i < length)
      {
        i++;
      }
    }
    else if (_notcode(s[i]))
    {
      i++;
    }
    else
    {
      token_count++;
      while (i < length && !_notcode(s[i]))
      {
        i++;
      }
    }
  }
  return token_count;
}

char **_asm_tokenize(const char *s, size_t *num_tokens)
{
  int i = 0, j = 0, start, end, token_length;
  size_t length      = strlen(s);
  size_t token_count = _asm_count_tokens(s);
  char **tokens      = malloc(sizeof(char *) * token_count);
  int token_index    = 0;
  i                  = 0;
  while (i < length)
  {
    if (s[i] == ';')
    {
      while (s[i] != '\n' && i < length)
      {
        i++;
      }
    }
    else if (_notcode(s[i]))
    {
      i++;
      continue;
    }
    else
    {
      start = i;
      while (!_notcode(s[i]) && i < length)
      {
        i++;
      }
      end                 = i;
      token_length        = end - start;
      tokens[token_index] = malloc(token_length + 1);
      for (j = 0; j < token_length; j++)
      {
        tokens[token_index][j] = s[start + j];
      }
      tokens[token_index++][token_length] = '\0';
      while (isspace(s[i]))
      {
        i++;
      }
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
  _error("Expected opcode, got '%s", token);
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

u8 *asm_compf(const char *filepath, u64 *out_length)
{
  FILE *file = fopen(filepath, "rb");
  if (file == NULL)
  {
    _error("Could not read file '%s'\n", filepath);
  }
  fseek(file, 0, SEEK_END);
  size_t len = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *data = malloc(len + 1);
  if (data == NULL)
  {
    fclose(file);
    return NULL;
  }
  size_t read = fread(data, 1, len, file);
  data[len]   = '\0';
  fclose(file);
  if (read != len)
  {
    _error("Could not read entire file '%s'\n", filepath);
  }
  u8 *result = asm_comps(data, out_length);
  free(data);
  return result;
}

u8 *asm_comps(const char *code, u64 *out_length)
{
  if (!code || !out_length)
  {
    return NULL;
  }
  u64 bytec = 32;
  u8 *bytes = calloc(bytec, sizeof(u8));
  if (bytes == NULL)
  {
    _error("Memory allocation failure");
    exit(1);
  }
  _logl("Compiling...");
  size_t ntokens = 0;
  char **tokens  = _asm_tokenize(code, &ntokens);
  u64 i = 0;
  while (i < ntokens)
  {
    u8 op = _asm_op(tokens[i++]);
    _log("+ %s", _op_toks[op]);
    if (*out_length + 1 > bytec)
    {
      u8 *tbytes = realloc(bytes, bytec *= 2);
      if (tbytes == NULL)
      {
        _error("Memory allocation failure");
        exit(1);
      }
      bytes = tbytes;
    }
    _pushv(bytes, *out_length, op, 1);
    *out_length += 1;
    for (int j = 0; j < _op_argc[op]; ++j)
    {
      if (i >= ntokens)
      {
        _error("Expected argument, got EOF");
        exit(1);
      }
      u8 alen = _op_lens[op][j];
      if (*out_length + alen > bytec)
      {
        u8 *tbytes = realloc(bytes, bytec *= 2);
        if (tbytes == NULL)
        {
          _error("Memory allocation failure");
          exit(1);
        }
        bytes = tbytes;
      }
      _log(" %s", tokens[i]);
      u64 arg = _asm_arg(tokens[i++]);
      _pushv(bytes, *out_length, arg, alen);
      *out_length += alen;
    }
    _log("\n");
  }
  free(tokens);
  bytes              = realloc(bytes, (*out_length + 1) * sizeof(u8));
  bytes[*out_length] = 0;
  _log("\n");
  return bytes;
}

cpu cpu_new(u64 memc, u8 *mem)
{
  if (memc == 0 || !mem)
  {
    _error("CPU requires memory to function");
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
  _logl("Running...");
  while (c->ip < c->memc)
  {
    cpu_step(c);
  }
  _log("\n");
}

#define Y(V) " "_##V##_fmt
#define E(N, V) CPU_GET(V, arg##N);
#define _X_0(NAME, ...) _op_##NAME(c)
#define _X_1(NAME, ...) _op_##NAME(c, GEN_ARGS(ARGC(__VA_ARGS__)))
#define _PF_0(...) _op_toks[op]
#define _PF_1(...) _op_toks[op], GEN_ARGS(ARGC(__VA_ARGS__))
#define X(NAME, ...)                                                           \
  case op_##NAME:                                                              \
  {                                                                            \
    EARGS(__VA_ARGS__)                                                         \
    _log("> %s" YARGS(__VA_ARGS__) " \t; ",                                    \
         CONCAT(_PF_, HASARGS(__VA_ARGS__))(__VA_ARGS__));                     \
    CONCAT(_X_, HASARGS(__VA_ARGS__))(NAME, __VA_ARGS__);                      \
    _log("\n");                                                                \
    break;                                                                     \
  }
void cpu_step(cpu c)
{
  CPU_GET(u8, op);
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

//

void _op_nop(cpu c)
{
  (void) c;
}

void _op_hlt(cpu c)
{
  c->ip = c->memc;
}

//

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

void _op_swp(cpu c, u8 r0, u8 r1)
{
  u64 t      = c->reg[r0];
  c->reg[r0] = c->reg[r1];
  c->reg[r1] = t;
}

void _op_psh(cpu c, u8 reg)
{
  c->stk[c->sp++] = c->reg[reg];
}

void _op_pek(cpu c, u8 reg)
{
  c->reg[reg] = c->stk[c->sp];
}

void _op_pop(cpu c, u8 reg)
{
  c->reg[reg] = c->stk[c->sp -= 1];
}

void _op_sys(cpu c, u8 reg)
{
  u64 sysop = c->reg[reg];

  switch (sysop)
  {
    case sys_print:
      u8 type  = c->i0;
      u64 data = c->i1;
      switch (type)
      {
        case 0:
          _log("%c", (char) data);
          break;
        case 1:
          _log(_u8_fmt, (u8) data);
          break;
        case 2:
          _log(_u16_fmt, (u16) data);
          break;
        case 3:
          _log(_u32_fmt, (u32) data);
          break;
        case 4:
          _log(_u64_fmt, (u64) data);
          break;
        default:
          _error(
            "Invalid print type: '"_u8_fmt
            "'",
            type);
          exit(1);
          break;
      }
      break;

    default:
      _error(
        "Invalid sys op: '"_u64_fmt
        "'",
        sysop);
      exit(1);
      break;
  }
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