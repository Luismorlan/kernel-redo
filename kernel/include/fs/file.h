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

#pragma once

#include "types.h"

#define FMODE_READ    1
#define FMODE_WRITE   2
#define FMODE_APPEND  4

struct vnode;

typedef struct file {
        /*
         * The current position in the file. Can be modified by system calls
         * like lseek(2), read(2), and write(2) (and possibly others) as
         * described in the man pages of those calls.
         */
        off_t                   f_pos;

        /*
         * The mode in which this file was opened. This is a mask of the flags
         * FMODE_READ, FMODE_WRITE, and FMODE_APPEND. It is set when the file
         * is first opened, and use to restrict the operations that can be
         * performed on the underlying vnode.
         */
        int                     f_mode;

        /*
         * The number of references to this struct. This is nearly the same as
         * the vnode's vn_refcount member.
         */
        int                     f_refcount;

        /*
         * The vnode which corresponds to this file.
         */
        struct vnode            *f_vnode;
} file_t;

/*
 * Returns the file_t assiciated with the given file descriptor for the
 * current process. If there is no associated file_t, returns NULL.
 *
 * If the passed fd == -1, a new file_t, not associated with any process
 * or file descriptor, is created and returned with its refcount set to
 * 1.
 */
struct file *fget(int fd);

/*
 * fref() increments the reference count on the given file.
 */
void fref(file_t *f);

/*
 * fput() decrements the reference count on the given file.
 *
 * If the refcount reaches 0, the storage for the given file_t will be
 * released (f won't point to a valid memory address anymore), and the
 * refcount on the associated vnode (if any) will be decremented.
 */
void fput(file_t *f);