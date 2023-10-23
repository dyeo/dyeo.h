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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONCAT(A, B) _CONCAT(A, B)
#define _CONCAT(A, B) A##B

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
  X(nop, void)        /*noop*/                                                 \
  X(ldv, oword, word) /*load val to reg*/                                      \
  X(ldm, oword, word) /*load mem to reg*/                                      \
  X(ldr, word, word)  /*load reg to reg*/                                      \
  X(ldsp, word)       /*load stack ptr to reg*/                                \
  X(ldip, word)       /*load inst ptr to reg*/                                 \
  X(stm, word, oword) /*store reg to mem*/                                     \
  X(pop, word)        /*pop stack to reg*/                                     \
  X(peek, word)       /*peek stack to reg*/                                    \
  X(push, word)       /*push reg to stack*/                                    \
  X(add, word, word)  /*reg1 = reg1 + reg0*/                                   \
  X(sub, word, word)  /*reg1 = reg1 - reg0*/                                   \
  X(mul, word, word)  /*reg1 = reg1 * reg0*/                                   \
  X(div, word, word)  /*reg1 = reg1 / reg0*/                                   \
  X(mod, word, word)  /*reg1 = reg1 % reg0*/                                   \
  X(not, word)        /*reg0 = ~reg0*/                                         \
  X(lt, word, word)   /*reg1 = reg1 < reg0*/                                   \
  X(le, word, word)   /*reg1 = reg1 <= reg0*/                                  \
  X(eq, word, word)   /*reg1 = reg1 == reg0*/                                  \
  X(ne, word, word)   /*reg1 = reg1 != reg0*/                                  \
  X(ge, word, word)   /*reg1 = reg1 >= reg0*/                                  \
  X(gt, word, word)   /*reg1 = reg1 > reg0*/                                   \
  X(jp, oword)        /*jump inst ptr to addr*/                                \
  X(jpz, word, oword) /*jump if reg == 0 to addr*/                             \
  X(jpn, word, oword) /*jump if reg != 0 to addr*/                             \
  X(sys, word)        /*call system oper */                                    \
  X(hlt, void)        /*end the execution*/

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
#define void 0
#define word 1
#define dword 2
#define qword 4
#define oword 8
#define X(NAME, ...) {__VA_ARGS__, 0},
  _OPS_X_LIST
#undef X
#undef void
#undef word
#undef dword
#undef qword
#undef oword
};

const char *_op_toks[] = {
#define X(NAME, ...) #NAME,
  _OPS_X_LIST
#undef X
};

typedef enum cpu_sysop
{
  sys_putc,
  sys_puti,
  CPU_SYSOPS_COUNT
} cpu_sysop;

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

static inline char *_strsub(const char *str, size_t i, size_t len)
{    if (!str) {
        return NULL;
    }
    size_t slen = strlen(str);
    size_t sublen = (i + len > slen) ? (slen - i) : len;
    char *substr = (char *) malloc(sublen + 1);
    if (!substr) {
        return NULL;
    }
    strncpy(substr, str + i, sublen);
    substr[sublen] = '\0';
    return substr;
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
        *(words + *wlen) = (cpu_op) i;
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
          char *endp = NULL;
          oword val  = strtoll(atok, &endp, 0);
          if (!isdigit(atok[0]) && isascii(atok[0]))
          {
            val = (oword) atok[0];
          }
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

void cpu_run(cpu c)
{
  while (c->ip != c->memc)
  {
    cpu_step(c);
  }
}

void cpu_step(cpu c)
{
  word op = cpu_pop(c, word);
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
  oword val   = cpu_pop(c, oword);
  word reg    = cpu_pop(c, word);
  c->reg[reg] = val;
}

void _op_ldm(cpu c)
{
  oword addr  = cpu_pop(c, oword);
  word reg    = cpu_pop(c, word);
  c->reg[reg] = c->mem[addr];
}

void _op_ldr(cpu c)
{
  word sReg    = cpu_pop(c, word);
  word dReg    = cpu_pop(c, word);
  c->reg[dReg] = c->reg[sReg];
}

void _op_ldsp(cpu c)
{
  word reg    = cpu_pop(c, word);
  c->reg[reg] = c->sp;
}

void _op_ldip(cpu c)
{
  word reg    = cpu_pop(c, word);
  c->reg[reg] = c->ip;
}

void _op_stm(cpu c)
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
  c->stk[c->sp++] = c->reg[reg];
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

void _op_not(cpu c)
{
  word reg    = cpu_pop(c, word);
  c->reg[reg] = ~c->reg[reg];
}

void _op_jp(cpu c)
{
  oword addr = cpu_pop(c, oword);
  c->ip      = addr;
}

void _op_jpz(cpu c)
{
  word reg   = cpu_pop(c, word);
  oword addr = cpu_pop(c, oword);
  c->ip      = c->reg[reg] == 0 ? addr : c->ip;
}

void _op_jpn(cpu c)
{
  word reg   = cpu_pop(c, word);
  oword addr = cpu_pop(c, oword);
  c->ip      = c->reg[reg] == 1 ? addr : c->ip;
}

void _op_sys(cpu c)
{
  word call = cpu_pop(c, word);
  switch (call)
  {
    default:
      fprintf(stderr, "ERROR: Invalid sys call '%u'", call);
      exit(1);
      break;
    case sys_putc:
      putc((char) c->reg[0], stdout);
      break;
    case sys_puti:
      fprintf(stdout, "%llu", c->reg[0]);
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