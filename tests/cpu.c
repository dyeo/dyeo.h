#define CPU_IMPLEMENTATION
#include "../cpu.h"

#define MEM_SIZE 1024

const char *assembly =
  "set r0 0 "
  "set i0 4 "
  "set i1 69 "
  "sys r0 "
  "sys r0 "
  "hlt ";

int main()
{
  u64 memc = 0;
  u8 *mem  = asm_compile(assembly, &memc);
  printf("\n");
  for (int i = 0; i < memc;)
  {
    u8 op = mem[i++];
    printf("%x", op);
    for (int j = 0; j < _op_argc[op]; ++j)
    {
      switch (_op_lens[op][j])
      {
        case _u8_siz:
        {
          printf(" "_u8_fmt, mem[i]);
          i += _u8_siz;
        }
        break;
        case _u16_siz:
        {
          printf(" "_u16_fmt, *((u16 *) (mem + i)));
          i += _u16_siz;
        }
        break;
        case _u32_siz:
        {
          printf(" "_u32_fmt, *((u32 *) (mem + i)));
          i += _u32_siz;
        }
        break;
        case _u64_siz:
        {
          printf(" "_u64_fmt, *((u64 *) (mem + i)));
          i += _u64_siz;
        }
        break;
        default:
          break;
      }
    }
    printf("\n");
  }
  printf("\n");

  cpu c = cpu_new(memc, mem);
  cpu_run(c);
  printf("\n");
  printf("%llu\n", c->r[0]);
  printf("%llu\n", c->r[1]);
  free(c);
  free(mem);
  return 0;
}