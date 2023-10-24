#define CPU_IMPLEMENTATION
#include "../cpu.h"

#define MEM_SIZE 1024

const char *assembly =
  "set r0 1000 "
  "set r1 1000 ";

int main()
{
  u64 memc = 0;
  u8 *mem  = asm_compile(assembly, &memc);
  printf("%llu: ", memc);
  for (int i = 0; i < memc; ++i)
  {
    printf("%x ", mem[i]);
  }
  printf("\n");

  cpu c = cpu_new(memc, mem);
  cpu_run(c);
  printf("%llu\n", c->r[0]);
  printf("%llu\n", c->r[1]);
  free(c);
  free(mem);
  return 0;
}