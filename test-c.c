#include "sfh.h"
#include <stdlib.h>
#include <string.h>

static void
foo(void)
{
    int* invalid = NULL;
    memset(invalid, 0, 4096);
}

static void
bar(void)
{
    __sfh_try {
        foo();
    } __sfh_catch {
        puts("It works");
    }
    foo();
}

int
main(int argc, char* argv[])
{
    sfh_init_sjlj(SIGSEGV);
    __sfh_try {
        bar();
    } __sfh_catch {
        puts("It works with nested");
    }
    bar();
    return 0;
}
