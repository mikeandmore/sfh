#include "sfh.h"
#include <cstring>
#include <sstream>

namespace sfh {

signal_exception::signal_exception(siginfo_t* siginfo, ucontext_t* ctx)
{
    msg_ = NULL;
    memcpy(&sfh_except_.ctx, ctx, sizeof(ucontext_t));
    memcpy(&sfh_except_.siginfo, siginfo, sizeof(siginfo_t));
}

signal_exception::~signal_exception() throw()
{
    free(msg_);
}

sfh_exception
signal_exception::exception() const
{
    return sfh_except_;
}

}

static void
__sfh_cxx_sigaction(int signal, siginfo_t* info, void* data)
{
    ucontext_t* uc = (ucontext_t*) data;
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, signal);
    // restore the sigmask
    if (pthread_sigmask(SIG_UNBLOCK, &sigset, NULL) < 0) {
        fprintf(stderr, "Cannot restore signal mask!\n");
        exit(-1);
    }
    throw sfh::signal_exception(info, uc);
}

extern "C" void
sfh_init_cxx_throw(int signal)
{
    struct sigaction sa;
    sa.sa_sigaction = __sfh_cxx_sigaction;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(signal, &sa, NULL);
}
