#include "../build.h"
#include "../test.h"

#define cflag(CMD, CFLAG) (CFLAG ? cmdarg(CMD, "-" #CFLAG) : (void)CFLAG)

bool Wall = true;
bool Wextra = false;
bool Werror = false;

void cflags(Cmd *c) {
  cflag(c, Wall);
  cflag(c, Wextra);
  cflag(c, Werror);
}

void source(Cmd *c) { cmdarg(c, path("../../tests", "build.c")); }

void output(Cmd *c) { cmdarg(c, "-o", "./build.exe"); }

int main(void) {
  test_group(build, {
    printf("%s\n", path("c", "Windows/system32", "drivers"));
    Cmd *cc = cmdnew(__COMPILER__);
    cmdcallf("echo", "Hello, world!");
    mkdir("out");
    cd("out");
    cflags(cc);
    source(cc);
    output(cc);
    bool status = cmdcall(cc);
    cmdfree(cc);
    test_expr(status, bool, true);
  });
  return 0;
}