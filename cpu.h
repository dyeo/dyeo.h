/*
# cpu.h - simple 8-register cpu vm
*/
#ifndef _CPU_H
#define _CPU_H

#ifdef __cplusplus
extern "C" {
#endif
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

#define CONCAT(A, B) _CONCAT(A, B)
#define _CONCAT(A, B) A##B

#if defined(__GNUC__) || defined(__clang__)
#define _ARGC(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define ARGC(...) _ARGC(_, ##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#else
#define _ARGC(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define ARGC(...) _ARGC(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#endif

#define F_TO_O(d) (*(oword *) &(d))
#define O_TO_F(i) (*(double *) &(i))

typedef uint8_t word;
typedef uint16_t dword;
typedef uint32_t qword;
typedef uint64_t oword;

#define CPU_REG_COUNT 16
#define CPU_STK_SIZE 2048

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
extern void cpu_run(cpu c);
extern void cpu_step(cpu c);

#define _OPS_X_LIST                                                            \
  X(nop) /* noop */                                                            \
  /**/                                                                         \
  X(ldv, oword, word) /* load val to reg */                                    \
  X(ldm, oword, word) /* load mem to reg */                                    \
  X(ldr, word, word)  /* load reg to reg */                                    \
  X(ldsp, word)       /* load stack ptr to reg */                              \
  X(ldip, word)       /* load inst ptr to reg */                               \
  X(stm, word, oword) /* store reg to mem */                                   \
  /**/                                                                         \
  X(pop, word)  /* pop stack to reg */                                         \
  X(peek, word) /* peek stack to reg */                                        \
  X(push, word) /* push reg to stack */                                        \
  /**/                                                                         \
  X(add, word, word) /* reg1 = reg1 + reg0 */                                  \
  X(sub, word, word) /* reg1 = reg1 - reg0 */                                  \
  X(mul, word, word) /* reg1 = reg1 * reg0 */                                  \
  X(div, word, word) /* reg1 = reg1 / reg0 */                                  \
  X(mod, word, word) /* reg1 = reg1 % reg0 */                                  \
  X(neg, word)       /* reg0 = -reg0 */                                        \
  /**/                                                                         \
  X(and, word, word) /* reg1 = reg1 & reg0 */                                  \
  X(or, word, word)  /* reg1 = reg1 | reg0 */                                  \
  X(xor, word, word) /* reg1 = reg1 ^ reg0 */                                  \
  X(not, word)       /* reg0 = ~reg0 */                                        \
  /**/                                                                         \
  X(lt, word, word) /* reg1 = reg1 < reg0 */                                   \
  X(le, word, word) /* reg1 = reg1 <= reg0 */                                  \
  X(eq, word, word) /* reg1 = reg1 == reg0 */                                  \
  X(ne, word, word) /* reg1 = reg1 != reg0 */                                  \
  X(ge, word, word) /* reg1 = reg1 >= reg0 */                                  \
  X(gt, word, word) /* reg1 = reg1 > reg0 */                                   \
  /**/                                                                         \
  X(jp, oword)        /* jump inst ptr to addr */                              \
  X(jpz, word, oword) /* jump if reg == 0 to addr */                           \
  X(jpn, word, oword) /* jump if reg != 0 to addr */                           \
  X(sys, word)        /* call system oper  */                                  \
  /**/                                                                         \
  X(hlt) /* end the execution */

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

word _op_lens[CPU_OPS_COUNT][4] = {
#define word 1
#define dword 2
#define qword 4
#define oword 8
#define X(NAME, ...) {ARGC(__VA_ARGS__), __VA_ARGS__},
  _OPS_X_LIST
#undef X
#undef word
#undef dword
#undef qword
#undef oword
};

#undef ARGC
#undef _ARGC

const char *_op_toks[] = {
#define X(NAME, ...) #NAME,
  _OPS_X_LIST
#undef X
};

typedef enum cpu_sysop
{
  sys_putc,
  sys_puti,
  sys_putf,
  CPU_SYSOPS_COUNT
} cpu_sysop;

#ifdef __cplusplus
}
#endif

#endif

#ifdef CPU_IMPLEMENTATION
#ifndef _CPU_C
#define _CPU_C

#define CPU_POP(CPU, TYPE) CONCAT(_CPU_POP_, TYPE)(CPU)
#define CPU_PEEK(CPU, TYPE) CONCAT(_CPU_PEEK_, TYPE)(CPU)

#define _CPU_POP_8 _CPU_POP_oword
#define _CPU_PEEK_8 _CPU_PEEK_oword
#define _CPU_POP_oword(CPU)                                                    \
  (((oword) (CPU)->mem[(CPU)->ip++] << 56) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 48) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 40) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 32) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 24) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 16) |                                   \
   ((oword) (CPU)->mem[(CPU)->ip++] << 8) |                                    \
   ((oword) (CPU)->mem[(CPU)->ip++] << 0))
#define _CPU_PEEK_oword(CPU)                                                   \
  (((oword) (CPU)->mem[(CPU)->ip] << 56) |                                     \
   ((oword) (CPU)->mem[(CPU)->ip + 1] << 48) |                                 \
   ((oword) (CPU)->mem[(CPU)->ip + 2] << 40) |                                 \
   ((oword) (CPU)->mem[(CPU)->ip + 3] << 32) |                                 \
   ((oword) (CPU)->mem[(CPU)->ip + 4] << 24) |                                 \
   ((oword) (CPU)->mem[(CPU)->ip + 5] << 16) |                                 \
   ((oword) (CPU)->mem[(CPU)->ip + 6] << 8) |                                  \
   ((oword) (CPU)->mem[(CPU)->ip + 7] << 0))

#define _CPU_POP_4 _CPU_POP_qword
#define _CPU_PEEK_4 _CPU_PEEK_qword
#define _CPU_POP_qword(CPU)                                                    \
  (((qword) (CPU)->mem[(CPU)->ip++] << 24) |                                   \
   ((qword) (CPU)->mem[(CPU)->ip++] << 16) |                                   \
   ((qword) (CPU)->mem[(CPU)->ip++] << 8) |                                    \
   ((qword) (CPU)->mem[(CPU)->ip++] << 0))
#define _CPU_PEEK_qword(CPU)                                                   \
  (((qword) (CPU)->mem[(CPU)->ip] << 24) |                                     \
   ((qword) (CPU)->mem[(CPU)->ip + 1] << 16) |                                 \
   ((qword) (CPU)->mem[(CPU)->ip + 2] << 8) |                                  \
   ((qword) (CPU)->mem[(CPU)->ip + 3] << 0))

#define _CPU_POP_2 _CPU_POP_dword
#define _CPU_PEEK_2 _CPU_PEEK_dword
#define _CPU_POP_dword(CPU)                                                    \
  (((dword) (CPU)->mem[(CPU)->ip++] << 8) |                                    \
   ((dword) (CPU)->mem[(CPU)->ip++] << 0))
#define _CPU_PEEK_dword(CPU)                                                   \
  (((dword) (CPU)->mem[(CPU)->ip] << 8) |                                      \
   ((dword) (CPU)->mem[(CPU)->ip + 1] << 0))

#define _CPU_POP_1 _CPU_POP_word
#define _CPU_PEEK_1 _CPU_PEEK_word
#define _CPU_POP_word(C) ((word) (C)->mem[(C)->ip++])
#define _CPU_PEEK_word(C) ((word) (C)->mem[(C)->ip])

#define _CPU_PUSH(ARR, INDEX, VALUE, LEN)                                      \
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

#define CPU_GET(TYPE, NAME) TYPE NAME = CPU_POP(c, TYPE)

static inline const char *strnchr(const char *s, const char c, size_t n)
{
  if (!s || n == 0)
  {
    return NULL;
  }
  size_t i = 0;
  if (!s[i] || s[i] != c)
  {
    return NULL;
  }
  while (s[i] && i < n && s[i] != c)
  {
    i++;
  }
  return s + i > s ? s + i : NULL;
}

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

typedef struct
{
  const char *start;
  oword length;
} _asmtok;

_asmtok _asm_next_tok(const char *code, oword *i)
{
  _asm_skip_ws(code, i);
  _asmtok tok = {&code[*i], 0};
  while (code[*i] && !isspace(code[*i]) && code[*i] != ';')
  {
    tok.length++;
    *i += 1;
  }
  return tok;
}

bool _is_tok_int(const char *start, const size_t length)
{
  size_t i = 0;
  while (i < length && isdigit(start[i]))
  {
    i++;
  }
  return i == length;
}

word *asm_to_words(const char *code, oword *wlen)
{
  if (!code || !wlen)
  {
    return NULL;
  }
  oword wordc = CPU_DEFAULT_BUFLEN;
  word *words = malloc(wordc * sizeof(word));
  oword a     = 0;
  while (code[a])
  {
    _asmtok tok = _asm_next_tok(code, &a);
    for (int i = 0; i < CPU_OPS_COUNT; ++i)
    {
      if (strncmp(tok.start, _op_toks[i], strlen(_op_toks[i])) == 0)
      {
        if (*wlen + 1 > wordc)
        {
          words = realloc(words, (wordc *= 2) * sizeof(word));
        }
        *(words + *wlen) = (word) i;
        *wlen += 1;
        word *arglens = _op_lens[i];
        int j         = 0;
        while (j < arglens[0])
        {
          if (arglens[j] == 0)
          {
            break;
          }
          oword alen = arglens[j + 1];
          if (*wlen + alen > wordc)
          {
            words = realloc(words, (wordc *= 2) * sizeof(word));
          }
          _asmtok atok = _asm_next_tok(code, &a);
          bool isint  = _is_tok_int(atok.start, atok.length);
          oword oval  = strtoll(atok.start, NULL, 0);
          char *endp  = NULL;
          double dval = strtold(atok.start, &endp);
          if (!isint)
          {
            if (atok.length == 1 && isascii(atok.start[0]))
            {
              oval = (oword) atok.start[0];
            }
            else if (endp != atok.start)
            {
              oval = F_TO_O(dval);
            }
            else
            {
              fprintf(stderr, "ERROR: Invalid token '%.*s'\n",
                      (int) atok.length, atok.start);
            }
          }
          _CPU_PUSH(words, *wlen, oval, alen);
          *wlen += alen;
          j++;
        }
      }
    }
  }
  words        = realloc(words, (*wlen + 1) * sizeof(word));
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

void cpu_run(cpu c)
{
  while (c->ip != c->memc)
  {
    cpu_step(c);
  }
}

void cpu_step(cpu c)
{
  CPU_GET(word, op);
  _op_fptr[op](c);
}

void _op_nop(cpu c)
{
  (void) c;
}

void _op_hlt(cpu c)
{
  c->ip = c->memc;
}

void _op_ldv(cpu c)
{
  CPU_GET(oword, val);
  CPU_GET(word, reg);
  c->reg[reg] = val;
}

void _op_ldm(cpu c)
{
  CPU_GET(oword, addr);
  CPU_GET(word, reg);
  c->reg[reg] = c->mem[addr];
}

void _op_ldr(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[sReg];
}

void _op_ldsp(cpu c)
{
  CPU_GET(word, reg);
  c->reg[reg] = c->sp;
}

void _op_ldip(cpu c)
{
  CPU_GET(word, reg);
  c->reg[reg] = c->ip;
}

void _op_stm(cpu c)
{
  CPU_GET(word, reg);
  CPU_GET(oword, addr);
  c->mem[addr] = c->reg[reg];
}

void _op_pop(cpu c)
{
  CPU_GET(word, reg);
  c->reg[reg] = c->stk[c->sp--];
}

void _op_peek(cpu c)
{
  CPU_GET(word, reg);
  c->reg[reg] = c->stk[c->sp];
}

void _op_push(cpu c)
{
  CPU_GET(word, reg);
  c->stk[c->sp++] = c->reg[reg];
}

void _op_add(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] + c->reg[sReg];
}

void _op_sub(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] - c->reg[sReg];
}

void _op_mul(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] * c->reg[sReg];
}

void _op_div(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] / c->reg[sReg];
}

void _op_mod(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] % c->reg[sReg];
}

void _op_neg(cpu c)
{
  CPU_GET(word, reg);
  c->reg[reg] = -c->reg[reg];
}

void _op_lt(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] < c->reg[sReg];
}

void _op_le(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] <= c->reg[sReg];
}

void _op_eq(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] == c->reg[sReg];
}

void _op_ne(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] != c->reg[sReg];
}

void _op_ge(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] >= c->reg[sReg];
}

void _op_gt(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] > c->reg[sReg];
}

void _op_and(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] & c->reg[sReg];
}

void _op_or(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] | c->reg[sReg];
}

void _op_xor(cpu c)
{
  CPU_GET(word, sReg);
  CPU_GET(word, dReg);
  c->reg[dReg] = c->reg[dReg] ^ c->reg[sReg];
}

void _op_not(cpu c)
{
  CPU_GET(word, reg);
  c->reg[reg] = ~c->reg[reg];
}

void _op_jp(cpu c)
{
  CPU_GET(oword, addr);
  c->ip = addr;
}

void _op_jpz(cpu c)
{
  CPU_GET(word, reg);
  CPU_GET(oword, addr);
  c->ip = c->reg[reg] == 0 ? addr : c->ip;
}

void _op_jpn(cpu c)
{
  CPU_GET(word, reg);
  CPU_GET(oword, addr);
  c->ip = c->reg[reg] == 1 ? addr : c->ip;
}

void _op_sys(cpu c)
{
  CPU_GET(word, call);
  oword reg0 = c->reg[0];
  switch (call)
  {
    default:
      fprintf(stderr, "ERROR: Invalid sys call '%u'", call);
      exit(1);
      break;
    case sys_putc:
      fprintf(stdout, "%c", (char) reg0);
      break;
    case sys_puti:
      fprintf(stdout, "%llu", reg0);
      break;
  }
  fflush(stdout);
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