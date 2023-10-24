#define CPU_IMPLEMENTATION
#include "../cpu.h"

#define MEM_SIZE 1024

const char *assembly =
  "set r0 10 "
  "mov r1 r0 "
  "set r0 1 "
  "hlt";

int main()
{
  u64 memc = 0;
  u8 *mem  = asm_compile(assembly, &memc);
  printf("%llu: ", memc);
  for (int i = 0; i < memc; ++i)
  {
    printf("%x ", mem[i]);
  }

  cpu c = cpu_new(memc, mem);
  cpu_run(c);
  free(c);
  free(mem);
  return 0;
}