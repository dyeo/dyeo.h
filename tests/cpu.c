#define _CRT_SECURE_NO_WARNINGS
#define CPU_IMPLEMENTATION
#include "../cpu.h"

#define MEM_SIZE 1024

const char *assembly =
  "lodv 42 0 "
  "lodv 10 1 "
  "add 0 1 "
  "hlt 0";

int main()
{
  // word mem[] = {
  //   op_lodv, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, // lodv 42
  //   0 op_lodv, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x01, // lodv
  //   10 1 op_add,  0x00, 0x01,                                           //
  //   add 0 1 op_hlt                                                         //
  //   hlt
  // };
  // oword memc = sizeof(mem) / sizeof(word);

  oword memc = 0;
  word *mem  = asm_to_words(assembly, &memc);
  printf("%llu\n", memc);

  for (oword i = 0; i < memc; ++i)
  {
    printf("%X", mem[i]);
  }

  cpu c = cpu_new(memc, mem);
  while (c->ip < memc)
  {
    cpu_step(c);
  }
  printf("\n");
  printf("%llu\n", c->reg[1]);
  free(c);
  return 0;
}