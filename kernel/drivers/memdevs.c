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
#include "globals.h"

#include "util/string.h"
#include "util/debug.h"

#include "mm/mm.h"
#include "mm/page.h"
#include "mm/mmobj.h"
#include "mm/kmalloc.h"
#include "mm/pframe.h"

#include "drivers/bytedev.h"

#include "vm/anon.h"

#include "fs/vnode.h"

static int null_read(bytedev_t *dev, int offset, void *buf, int count);
static int null_write(bytedev_t *dev, int offset, const void *buf, int count);

static int zero_read(bytedev_t *dev, int offset, void *buf, int count);
static int zero_mmap(vnode_t *file, vmarea_t *vma, mmobj_t **ret);

bytedev_ops_t null_dev_ops = {
        null_read,
        null_write,
        NULL,
        NULL,
        NULL,
        NULL
};

bytedev_ops_t zero_dev_ops = {
        zero_read,
        null_write,
        zero_mmap,
        NULL,
        NULL,
        NULL
};

/*
 * The byte device code needs to know about these mem devices, so create
 * bytedev_t's for null and zero, fill them in, and register them.
 */
void
memdevs_init()
{
        NOT_YET_IMPLEMENTED("DRIVERS: memdevs_init");
}

/**
 * Reads a given number of bytes from the null device into a
 * buffer. Any read performed on the null device should read 0 bytes.
 *
 * @param dev the null device
 * @param offset the offset to read from. Should be ignored
 * @param buf the buffer to read into
 * @param count the maximum number of bytes to read
 * @return the number of bytes read, which should be 0
 */
static int
null_read(bytedev_t *dev, int offset, void *buf, int count)
{
        NOT_YET_IMPLEMENTED("DRIVERS: null_read");
        return -ENOMEM;
}

/**
 * Writes a given number of bytes to the null device from a
 * buffer. Writing to the null device should _ALWAYS_ be successful
 * and write the maximum number of bytes.
 *
 * @param dev the null device
 * @param offset the offset to write to. Should be ignored
 * @param buf buffer to read from
 * @param count the maximum number of bytes to write
 * @return the number of bytes written, which should be count
 */
static int
null_write(bytedev_t *dev, int offset, const void *buf, int count)
{
        NOT_YET_IMPLEMENTED("DRIVERS: null_write");
        return -ENOMEM;
}

/**
 * Reads a given number of bytes from the zero device into a
 * buffer. Any read from the zero device should be a series of zeros.
 *
 * @param dev the zero device
 * @param offset the offset to read from. Should be ignored
 * @param buf the buffer to write to
 * @param count the maximum number of bytes to read
 * @return the number of bytes read. Should always read the maximum
 * number of bytes
 */
static int
zero_read(bytedev_t *dev, int offset, void *buf, int count)
{
        NOT_YET_IMPLEMENTED("DRIVERS: zero_read");
        return 0;
}

/* Don't worry about these until VM. Once you're there, they shouldn't be hard. */

static int
zero_mmap(vnode_t *file, vmarea_t *vma, mmobj_t **ret)
{
        NOT_YET_IMPLEMENTED("DRIVERS: zero_mmap");
        return -1;
}
