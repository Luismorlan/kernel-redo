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

#include "errno.h"

#include "fs/file.h"
#include "fs/fcntl.h"
#include "fs/vfs_syscall.h"

#include "util/init.h"
#include "util/debug.h"
#include "util/list.h"

#include "mm/kmalloc.h"

#include "api/binfmt.h"

struct binfmt {
        const char        *bf_id;
        binfmt_load_func_t bf_load;
        list_link_t        bf_link;
};

static list_t binfmt_list;

static __attribute__((unused)) void binfmt_init()
{
        list_init(&binfmt_list);
}
init_func(binfmt_init);

int binfmt_add(const char *id, binfmt_load_func_t loadfunc)
{
        struct binfmt *fmt;
        if (NULL == (fmt = kmalloc(sizeof(*fmt)))) {
                return -ENOMEM;
        }

        dbg(DBG_EXEC, "Registering binary loader %s\n", id);

        fmt->bf_id = id;
        fmt->bf_load = loadfunc;
        list_insert_head(&binfmt_list, &fmt->bf_link);

        return 0;
}
int binfmt_load(const char *filename, char *const *argv, char *const *envp, uint32_t *eip, uint32_t *esp)
{
        int err, fd = -1;
        if (0 > (fd = do_open(filename, O_RDONLY))) {
                dbg(DBG_EXEC, "ERROR: exec failed to open file %s\n", filename);
                return fd;
        }

        file_t *file = fget(fd);
        KASSERT(NULL != file);
        if (S_ISDIR(file->f_vnode->vn_mode)) {
                err = -EISDIR;
                goto cleanup;
        }
        if (!S_ISREG(file->f_vnode->vn_mode)) {
                err = -EACCES;
                goto cleanup;
        }
        fput(file);
        file = NULL;

        struct binfmt *fmt;
        list_iterate_begin(&binfmt_list, fmt, struct binfmt, bf_link) {
                dbg(DBG_EXEC, "Trying to exec %s using binary loader %s\n", filename, fmt->bf_id);

                /* ENOEXE indicates that the given loader is unable to load
                 * the given file, any other error indicates that the file
                 * was recognized, but some other error existed which should
                 * be returned to the user, only if all loaders specify ENOEXEC
                 * do we actually return ENOEXEC */
                if (-ENOEXEC != (err = fmt->bf_load(filename, fd, argv, envp, eip, esp))) {
                        goto cleanup;
                }
        } list_iterate_end();

cleanup:
        if (NULL != file) {
                fput(file);
        }
        if (0 <= fd) {
                do_close(fd);
        }
        return err;
}
