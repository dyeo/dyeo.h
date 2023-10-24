#define CPU_DEBUG
#define CPU_IMPLEMENTATION
#include "../cpu.h"

#include <stdio.h>

#define MEM_SIZE 1024

const char *assembly =
  "set r0 0 "
  "set r1 420 "
  "set r2 69 "
  "set i0 4 "
  "swp i1 r1 "
  "sys r0 "
  "swp i1 r2 "
  "sys r0 "
  "hlt ";

int main()
{
  u64 memc = 0;
  u8 *mem  = asm_compile(assembly, &memc);

  cpu c = cpu_new(memc, mem);
  cpu_run(c);
  printf("%llu\n", c->r[0]);
  printf("%llu\n", c->r[1]);
  free(c);
  free(mem);
  return 0;
}