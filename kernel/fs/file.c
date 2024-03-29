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
 *  FILE: file.c
 *  AUTH: mcc | mahrens
 *  DESC:
 *  DATE: Fri Apr  3 20:38:30 1998
 */

#include "kernel.h"
#include "util/init.h"
#include "util/debug.h"
#include "util/string.h"
#include "util/printf.h"
#include "globals.h"
#include "util/list.h"
#include "fs/file.h"
#include "fs/vfs.h"
#include "fs/vnode.h"
#include "proc/proc.h"
#include "mm/slab.h"
#include "config.h"

static slab_allocator_t *file_allocator;

static __attribute__((unused)) void
file_init(void)
{
        file_allocator = slab_allocator_create("file", sizeof(file_t));
}
init_func(file_init);

void
fref(file_t *f)
{
        KASSERT(f->f_mode >= 0 && f->f_mode < 8);
        KASSERT(f->f_pos >= -1);
        KASSERT(f->f_refcount >= 0);
        if (f->f_refcount != 0) KASSERT(f->f_vnode);

        f->f_refcount++;

        if (f->f_vnode) {
                dbg(DBG_FREF, "fref: 0x%p, 0x%p ino %lu, up to %d (vnode refcount = %d, nrespages = %d)\n",
                    f, f->f_vnode->vn_fs, (unsigned long) f->f_vnode->vn_vno,
                    f->f_refcount, f->f_vnode->vn_refcount, f->f_vnode->vn_nrespages);
        } else {
                dbg(DBG_FREF, "fref: 0x%p up to %d\n", f, f->f_refcount);
        }
}

/* Look in process fd table and return the file*. */
file_t *
fget(int fd)
{
        file_t *f;

        if (fd == -1) {
                f = slab_obj_alloc(file_allocator);
                if (f) memset(f, 0, sizeof(file_t));
        } else {
                if (fd < 0 || fd >= NFILES)
                        return NULL;
                f = curproc->p_files[fd];
        }
        if (f) fref(f);

        return f;
}

/* - Decrement f_count.
 * - If f_count == 0, call vput() and free it. */
void
fput(file_t *f)
{
        KASSERT(f);
        KASSERT(f->f_mode >= 0 && f->f_mode < 8);
        KASSERT(f->f_pos >= -1);
        KASSERT(f->f_refcount > 0);
        if (f->f_refcount != 1) KASSERT(f->f_vnode);

        f->f_refcount--;

        if (f->f_vnode) {
                dbg(DBG_FREF, "fput: 0x%p, 0x%p ino %lu, down to %d (vnode refcount = %d, nrespages = %d)\n",
                    f, f->f_vnode->vn_fs, (unsigned long) f->f_vnode->vn_vno,
                    f->f_refcount, f->f_vnode->vn_refcount, f->f_vnode->vn_nrespages);
        } else {
                dbg(DBG_FREF, "fput: 0x%p down to %d\n", f, f->f_refcount);
        }

        if (f->f_refcount == 0) {
                if (f->f_vnode) vput(f->f_vnode);
                slab_obj_free(file_allocator, f);
        }
}
