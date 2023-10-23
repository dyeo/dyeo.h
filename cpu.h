/*
# cpu.h - simple 8-register cpu vm
*/
#ifndef _CPU_H
#define _CPU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONCAT(A, B) _CONCAT(A, B)
#define _CONCAT(A, B) A##B

#define ARGC(...) _ARGC(__VA_ARGS__, 5, 4, 3, 2, 1)
#define _ARGC(_1, _2, _3, _4, _5, N, ...) N

#define ARGLENS(...) _ARGLENS(ARGC(__VA_ARGS__), __VA_ARGS__)
#define _ARGLENS(N, ...) CONCAT(_ARGLENS_, N)(__VA_ARGS__), 0

#define _ARGLENS_5(_0, _1, _2, _3, _4)                                         \
  sizeof(_0), sizeof(_1), sizeof(_2), sizeof(_3), sizeof(_4)
#define _ARGLENS_4(_0, _1, _2, _3)                                             \
  sizeof(_0), sizeof(_1), sizeof(_2), sizeof(_3)
#define _ARGLENS_3(_0, _1, _2) sizeof(_0), sizeof(_1), sizeof(_2)
#define _ARGLENS_2(_0, _1) sizeof(_0), sizeof(_1)
#define _ARGLENS_1(_0) sizeof(_0)

typedef uint8_t word;
typedef uint16_t dword;
typedef uint32_t qword;
typedef uint64_t oword;

#define CPU_REG_COUNT 8
#define CPU_STK_SIZE 1024

typedef struct cpu
{
  // registers
  oword reg[CPU_REG_COUNT];

  // stack
  oword stk[CPU_STK_SIZE];

  // instruction pointer
  oword ip;

  // stack pointer
  oword sp;

  // memory byte count
  oword memc;

  // memory
  word *mem;
} *cpu;

word *asm_to_words(const char *code, oword *wlen);

extern cpu cpu_new(oword memc, word *mem);
extern void cpu_step(cpu c);

#define _OPS_X_LIST                                                            \
  X(lodv, oword, word)                                                         \
  X(lodm, oword, word)                                                         \
  X(lodr, word, word)                                                          \
  X(strm, word, oword)                                                         \
  X(pop, word)                                                                 \
  X(peek, word)                                                                \
  X(push, word)                                                                \
  X(add, word, word)                                                           \
  X(sub, word, word)                                                           \
  X(mul, word, word)                                                           \
  X(div, word, word)                                                           \
  X(mod, word, word)                                                           \
  X(lt, word, word)                                                            \
  X(le, word, word)                                                            \
  X(eq, word, word)                                                            \
  X(ne, word, word)                                                            \
  X(ge, word, word)                                                            \
  X(gt, word, word)                                                            \
  X(jmp, oword)                                                                \
  X(jmpz, word, oword)                                                         \
  X(jmpo, word, oword)                                                         \
  X(hlt, oword)

#define CPU_DEFAULT_BUFLEN 32

typedef enum cpu_op
{
#define X(NAME, ...) op_##NAME,
  _OPS_X_LIST
#undef X
    CPU_OPS_COUNT
} cpu_op;

#define X(NAME, ...) extern void _op_##NAME(cpu c);
_OPS_X_LIST
#undef X

void (*_op_fptr[])(cpu) = {
#define X(NAME, ...) _op_##NAME,
  _OPS_X_LIST
#undef X
};

word _op_lens[CPU_OPS_COUNT][3] = {
#define X(NAME, ...) {ARGLENS(__VA_ARGS__)},
  _OPS_X_LIST
#undef X
};

const char *_op_toks[] = {
#define X(NAME, ...) #NAME,
  _OPS_X_LIST
#undef X
};

#ifdef __cplusplus
}
#endif

#endif

#ifdef CPU_IMPLEMENTATION
#ifndef _CPU_C
#define _CPU_C

#define cpu_pop(CPU, TYPE) CONCAT(_cpu_pop_, TYPE)(CPU)

#define _cpu_pop_8 _cpu_pop_oword
#define _cpu_pop_oword(CPU)                                                    \
  (((oword) (CPU)->mem[(CPU)->ip++] << 56) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 48) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 40) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 32) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 24) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 16) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 8) |                                    \
   ((oword) (CPU)->mem[(CPU)->ip++] << 0))

#define _cpu_pop_4 _cpu_pop_qword
#define _cpu_pop_qword(CPU)                                                    \
  (((qword) (CPU)->mem[(CPU)->ip++] << 24) |                                   \
   ((qword) (CPU)->mem[(CPU)->ip++] << 16) |                                   \
   ((qword) (CPU)->mem[(CPU)->ip++] << 8) |                                    \
   ((qword) (CPU)->mem[(CPU)->ip++] << 0))

#define _cpu_pop_2 _cpu_pop_dword
#define _cpu_pop_dword(CPU)                                                    \
  (((dword) (CPU)->mem[(CPU)->ip++] << 8) |                                    \
   ((dword) (CPU)->mem[(CPU)->ip++] << 0))

#define _cpu_pop_1 _cpu_pop_word
#define _cpu_pop_word(C) ((word) (C)->mem[(C)->ip++])

#define _cpu_push(ARR, INDEX, VALUE, LEN)                                      \
  do                                                                           \
  {                                                                            \
    for (oword i = 0; i < LEN; i++)                                            \
    {                                                                          \
      (ARR)[INDEX + i] = (word) ((VALUE) >> (8 * (LEN - 1 - i)));              \
    }                                                                          \
  } while (0)

#define int_8_t oword
#define int_4_t qword
#define int_2_t dword
#define int_1_t word

void _asm_skip_comment(const char *code, oword *i)
{
  if (code[*i] == ';')
  {
    while (code[*i] != '\n')
    {
      *i += 1;
    }
  }
}

void _asm_skip_ws(const char *code, oword *i)
{
  _asm_skip_comment(code, i);
  while (code[*i] && isspace(code[*i]))
  {
    *i += 1;
  }
  _asm_skip_comment(code, i);
}

char *_asm_next_tok(const char *code, oword *i)
{
  _asm_skip_ws(code, i);
  if (!code || !*code || !i)
  {
    fprintf(stderr, "ERROR: couldn't get asm argument");
    return NULL;
  }
  oword j = *i;
  while (code[j] && !isspace(code[j]) && code[j] != ';')
  {
    j++;
  }
  oword rlen = j - *i;
  char *res  = malloc(rlen + 1);
  strncpy(res, &(code)[*i], rlen);
  res[rlen] = '\0';
  *i += rlen;
  return res;
}

word *asm_to_words(const char *code, oword *wlen)
{
  if (!code || !wlen)
  {
    return NULL;
  }
  oword wordc = CPU_DEFAULT_BUFLEN;
  word *words = malloc(wordc);
  oword a     = 0;
  while (code[a])
  {
    char *tok = _asm_next_tok(code, &a);
    for (int i = 0; i < CPU_OPS_COUNT; ++i)
    {
      if (strcmp(tok, _op_toks[i]) == 0)
      {
        if (*wlen + 1 > wordc)
        {
          words = realloc(words, (wordc *= 2));
        }
        memcpy(words + *wlen, &i, 1);
        *wlen += 1;
        word *arglens = _op_lens[i];
        int j         = 0;
        while (arglens[j] != 0)
        {
          oword alen = arglens[j];
          if (*wlen + alen > wordc)
          {
            words = realloc(words, (wordc *= 2));
          }
          char *atok = _asm_next_tok(code, &a);
          oword val  = atoll(atok);
          _cpu_push(words, *wlen, val, alen);
          *wlen += alen;
          free(atok);
          j++;
        }
      }
    }
    free(tok);
  }
  words        = realloc(words, *wlen + 1);
  words[*wlen] = 0;
  return words;
}

cpu cpu_new(oword memc, word *mem)
{
  if (memc == 0 || !mem)
  {
    fprintf(stderr, "ERROR: CPU requires memory to function\n");
    exit(1);
  }
  cpu c = malloc(sizeof(struct cpu));
  memset(c->reg, 0, sizeof(oword) * CPU_REG_COUNT);
  memset(c->stk, 0, sizeof(oword) * CPU_STK_SIZE);
  c->ip   = 0;
  c->sp   = 0;
  c->memc = memc;
  c->mem  = mem;
  return c;
}

#define _pop_word

void cpu_step(cpu c)
{
  word op = cpu_pop(c, word);
  _op_fptr[op](c);
}

void _op_lodv(cpu c)
{
  oword val   = cpu_pop(c, oword);
  word reg    = cpu_pop(c, word);
  c->reg[reg] = val;
}

void _op_lodm(cpu c)
{
  oword addr  = cpu_pop(c, oword);
  word reg    = cpu_pop(c, word);
  c->reg[reg] = c->mem[addr];
}

void _op_lodr(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[sReg];
}

void _op_strm(cpu c)
{
  word reg     = cpu_pop(c, word);
  oword addr   = cpu_pop(c, oword);
  c->mem[addr] = c->reg[reg];
}

void _op_pop(cpu c)
{
  word reg    = cpu_pop(c, word);
  c->reg[reg] = c->stk[c->sp--];
}

void _op_peek(cpu c)
{
  word reg    = cpu_pop(c, word);
  c->reg[reg] = c->stk[c->sp];
}

void _op_push(cpu c)
{
  word reg        = cpu_pop(c, word);
  c->stk[++c->sp] = c->reg[reg];
}

void _op_add(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] + c->reg[sReg];
}

void _op_sub(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] - c->reg[sReg];
}

void _op_mul(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] * c->reg[sReg];
}

void _op_div(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] / c->reg[sReg];
}

void _op_mod(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] % c->reg[sReg];
}

void _op_lt(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] < c->reg[sReg];
}

void _op_le(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] <= c->reg[sReg];
}

void _op_eq(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] == c->reg[sReg];
}

void _op_ne(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] != c->reg[sReg];
}

void _op_ge(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] >= c->reg[sReg];
}

void _op_gt(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[dReg] > c->reg[sReg];
}

void _op_jmp(cpu c)
{
  oword addr = cpu_pop(c, oword);
  c->ip      = addr;
}

void _op_jmpz(cpu c)
{
  word reg   = cpu_pop(c, word);
  oword addr = cpu_pop(c, oword);
  c->ip      = c->reg[reg] == 0 ? addr : c->ip;
}

void _op_jmpo(cpu c)
{
  word reg   = cpu_pop(c, word);
  oword addr = cpu_pop(c, oword);
  c->ip      = c->reg[reg] == 1 ? addr : c->ip;
}

void _op_hlt(cpu c)
{
  oword len = cpu_pop(c, oword);
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