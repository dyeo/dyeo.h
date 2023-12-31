#include "../cor.h"
#include "../test.h"

double f = 10.0;

cor_1_t(name, int *);
cor_1_func(name_0, int *)
{
  cor_start();
  printf("0: %d\n", *arg0);
  cor_yield(1, true);
  printf("1: %d\n", *arg0);
  cor_yield(2, true);
  printf("2: %d\n", *arg0);
  cor_yield(3, true);
  printf("3: %d\n", *arg0);
  cor_return(true);
  cor_end();
}

cor_name_fptr_t name_sig;

int main(void)
{
  test_group(cor, {
    name_sig           = name_0;
    int a              = 1;
    cor_t(name) crt    = cor_init(&a);
    corresult_t result = {0};
    result             = cor_1_call(name_sig, crt);
    test_expr(result.status, int, cor_yielded);
    result = cor_1_call(name_sig, crt);
    test_expr(result.status, int, cor_yielded);
    result = cor_1_call(name_sig, crt);
    test_expr(result.status, int, cor_yielded);
    result = cor_1_call(name_sig, crt);
    test_expr(result.status, int, cor_success);
  });
  return 0;
}