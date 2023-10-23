#define CPU_IMPLEMENTATION
#include "../cpu.h"

#define MEM_SIZE 1024

const char *assembly =
  " ldv h 0"
  " sys 0"
  " ldv i 0"
  " sys 0"
  " ldv ! 0"
  " sys 0"
  " ldv 10 0"
  " sys 0"
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