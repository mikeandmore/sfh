/* -*- mode: c++ -*- */
#ifndef _SFH_H_
#define _SFH_H_

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ucontext.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sfh_exception
{
    ucontext_t ctx;
    siginfo_t  siginfo;
};

void sfh_init_sjlj       (int signal);
void sfh_init_cxx_throw  (int signal);

#ifdef __cplusplus
}

#include <stdexcept>
#include <string>

namespace sfh {
struct signal_exception : public std::exception
{
    signal_exception(siginfo_t* siginfo, ucontext_t* ctx);
    virtual ~signal_exception() throw();

    sfh_exception exception() const;
private:
    sfh_exception sfh_except_;
    char*         msg_;
};
}

/* __try and __catch for C++ */
#define __sfh_try try
#define __sfh_catch catch (sfh::signal_exception __sfh_cxx_signal_exception)
#define sfh_get_exception __sfh_cxx_signal_exception.exception

#else
/* __try and __catch for C */
#include <setjmp.h>

struct jbstack
{
    jmp_buf         env;
    struct jbstack* next;
};

extern __thread struct jbstack* __jbhead;
extern __thread struct sfh_exception __jbexception;

int     __sfh_push_jbstack (void);
int     __sfh_pop_jbstack  (void);

#define __sfh_try                               \
    if (__sfh_push_jbstack()                    \
        && setjmp(__jbhead->env) == 0)          \

#define __sfh_catch                             \
    else if (__sfh_pop_jbstack())               \

#define sfh_get_exception __sfh_get_exception
#endif

#endif /* _SFH_H_ */
