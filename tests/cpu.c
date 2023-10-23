#define CPU_IMPLEMENTATION
#include "../cpu.h"

#define MEM_SIZE 1024

const char *assembly =
  " ldv 1.0 0"
  " ldv 1.0 1"
  " addf 1 0"
  " sys 1"
  " hlt";

int main()
{
  oword memc = 0;
  word *mem  = asm_to_words(assembly, &memc);

  cpu c = cpu_new(memc, mem);
  cpu_run(c);
  free(c);
  return 0;
}