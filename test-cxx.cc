#include "sfh.h"
#include <cstring>

static void
foo(void)
{
    int* invalid = NULL;
    memset(invalid, 0, 4096);
}

static void
bar(void)
{
    try {
        foo();
    } catch (sfh::signal_exception ex) {
        puts("It works with C++ try/catch");
    }
    foo();
}

int
main(int argc, char* argv[])
{
    sfh_init_cxx_throw(SIGSEGV);
    __sfh_try {
        bar();
    } __sfh_catch {
        puts("It works with sfh try/catch");
    }
    bar();

    return 0;
}
