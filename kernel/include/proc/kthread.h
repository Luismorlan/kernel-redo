/******************************************************************************/
/* Important Fall 2017 CSCI 402 usage information:                            */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

/*
 *   FILE: kthread.h
 * AUTHOR: kma
 *  DESCR: a kthread structure
 */
#pragma once

#include "util/list.h"

#include "proc/sched.h"
#include "proc/context.h"

typedef context_func_t kthread_func_t;

struct proc;
typedef struct kthread {
        context_t       kt_ctx;         /* this thread's context */
        char           *kt_kstack;      /* the kernel stack */
        void           *kt_retval;      /* this thread's return value */
        int             kt_errno;       /* error no. of most recent syscall */
        struct proc    *kt_proc;        /* the thread's process */

        int             kt_cancelled;   /* 1 if this thread has been cancelled */
        ktqueue_t      *kt_wchan;       /* The queue that this thread is blocked on */
        int             kt_state;       /* this thread's state */
        list_link_t     kt_qlink;       /* link on ktqueue */
        list_link_t     kt_plink;       /* link on proc thread list */
#ifdef __MTP__
        int             kt_detached;    /* if the thread has been detached */
        ktqueue_t       kt_joinq;       /* thread waiting to join with this thread */
#endif
} kthread_t;

/* thread states */
#define KT_NO_STATE             0       /* illegal state */
#define KT_RUN                  1       /* currently running, or on runq */
#define KT_SLEEP                2       /* blocked for an indefinite amount of time */
#define KT_SLEEP_CANCELLABLE    3       /* blocked, but this sleep can be cancelled */
#define KT_EXITED               4       /* has exited, waiting to be joined */

void kthread_init(void);

/**
 * Free resources associated with a thread.
 *
 * @param t the thread to free
 */
void kthread_destroy(kthread_t *t);

/**
 * Allocates and initializes a kernel thread.
 *
 * @param p the process in which the thread will run
 * @param func the function that will be called when the newly created
 * thread starts executing
 * @param arg1 the first argument to func
 * @param arg2 the second argument to func
 * @return the newly created thread
 */
kthread_t *kthread_create(struct proc *p, kthread_func_t func, long arg1, void *arg2);

/**
 * Cancel a thread.
 *
 * @param kthr the thread to be cancelled
 * @param retval the return value for the thread
 */
void kthread_cancel(kthread_t *kthr, void *retval);

/**
 * Exits the current thread.
 *
 * @param retval the return value for the thread
 */
void kthread_exit(void *retval);

/**
 * Allocates a new thread that is a copy of a specified thread.
 *
 * @param thr the thread to clone
 * @return a cloned version of thr
 */
kthread_t *kthread_clone(kthread_t *thr);

#ifdef __MTP__
/**
 * Shuts down the reaper daemon.
 */
void kthread_reapd_shutdown(void);

/**
 * Put a thread in the detached state.
 *
 * @param kthr the thread to put in the detached state
 * @return 0 on sucess and <0 on error
 */
int kthread_detach(kthread_t *kthr);

/**
 * Wait for the termination of another thread.
 *
 * @param kthr the thread to wait for
 * @param retval if retval is not NULL, the return value for kthr is
 * stored in the location pointed to by retval
 * @return 0 on sucess and <0 on error
 */
int kthread_join(kthread_t *kthr, void **retval);
#endif
