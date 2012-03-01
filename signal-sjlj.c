#include "sfh.h"
#include <string.h>

__thread struct jbstack* __jbhead = NULL;
__thread struct sfh_exception __jbexception;

int
__sfh_push_jbstack(void)
{
    struct jbstack* node = malloc(sizeof(struct jbstack));
    node->next = __jbhead;
    __jbhead = node;
    return 1;
}

int
__sfh_pop_jbstack(void)
{
    struct jbstack* next = __jbhead->next;
    free(__jbhead);
    __jbhead = next;
    return 1;
}

static void
__sfh_sjlj_sigaction(int sig, siginfo_t* info, void* data)
{
    ucontext_t* uc = data; /* TODO: secret data, Linux only? */
    sigset_t sigset;
    int sigmask_ret = -1;
    sigemptyset(&sigset);
    sigaddset(&sigset, sig);

    if (__jbhead == NULL) {
        /* nested execute default handler */
        SIG_DFL(sig);
        return;
    }
    /* fill up the exeception */
    memcpy(&__jbexception.ctx, uc, sizeof(ucontext_t));
    memcpy(&__jbexception.siginfo, info, sizeof(siginfo_t));
    if (pthread_sigmask(SIG_UNBLOCK, &sigset, NULL) < 0) {
        fprintf(stderr, "Cannot restore signal mask!\n");
        exit(-1); /* quit the whole process! */
    }
    longjmp(__jbhead->env, 1);
}

void
sfh_init_sjlj(int signal)
{
    struct sigaction sa;
    sa.sa_sigaction = __sfh_sjlj_sigaction;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(signal, &sa, NULL);
}
