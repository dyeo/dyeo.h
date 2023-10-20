#include "../build.h"

#define cflag(CMD, CFLAG) (CFLAG ? cmdarg(CMD, "-" #CFLAG) : (void)CFLAG)

bool Wall = true;
bool Wextra = true;
bool Werror = false;

void cflags(Cmd *c)
{
    cflag(c, Wall);
    cflag(c, Wextra);
    cflag(c, Werror);
}

void source(Cmd *c)
{
    cmdarg(c, path("../../tests", "build.c"));
}

void output(Cmd *c)
{
    cmdarg(c, "-o", "./build.exe");
}

int main(void)
{
    printf("%s\n", path("c","Windows/system32","drivers"));
    Cmd *cc = cmdnew(__COMPILER__);
    cmdcallf("echo", "Hello, world!");
    mkdir("out");
    cd("out");
    cflags(cc);
    source(cc);
    output(cc);
    bool status = cmdcall(cc);
    cmdfree(cc);
    return status ? 0 : 1;
}