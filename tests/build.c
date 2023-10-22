#include "../build.h"
#include "../test.h"

bool Wall   = true;
bool Wextra = false;
bool Werror = false;

void cflags(command_t c)
{
  cmdflag(c, Wall);
  cmdflag(c, Wextra);
  cmdflag(c, Werror);
}

void source(command_t c)
{
  cmdarg(c, path("../../tests", "build.c"));
}

void output(command_t c)
{
  cmdarg(c, "-o", "./build.exe");
}

int main(void)
{
  test_group(build, {
    printf("%s\n", path("c", "Windows/system32", "drivers"));
    command_t cc = cmdnew(__COMPILER__);
    cmdcall("echo", "Hello, world!");
    mkdir("out");
    cd("out");
    cflags(cc);
    source(cc);
    output(cc);
    bool status = cmdcallf(cc);
    cmdfree(cc);
    test_expr(status, bool, true);
  });
  return 0;
}