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

#include "util/list.h"

#define VMMAP_DIR_LOHI 1
#define VMMAP_DIR_HILO 2

struct mmobj;
struct proc;
struct vnode;

typedef struct vmmap {
        list_t       vmm_list;
        struct proc *vmm_proc;
} vmmap_t;

/* make sure you understand why mapping boundaries are in terms of frame
 * numbers and not addresses */
typedef struct vmarea {
        uint32_t       vma_start;    /* [starting vfn, */
        uint32_t       vma_end;      /*  ending vfn) */
        uint32_t       vma_off;      /* offset from beginning of vma_obj in pages */

        int            vma_prot;     /* permissions on mapping */
        int            vma_flags;    /* either MAP_SHARED or MAP_PRIVATE */

        struct vmmap  *vma_vmmap;    /* address space that this area belongs to */
        struct mmobj  *vma_obj;      /* the vm object to read pages from */
        list_link_t    vma_plink;    /* link on process vmmap maps list */
        list_link_t    vma_olink;    /* link on the list of all vm_areas
                                      * having the same vm_object at the
                                      * bottom of their chain */
} vmarea_t;

void vmmap_init(void);

size_t vmmap_mapping_info(const void *map, char *buf, size_t size);

vmmap_t *vmmap_create(void);
void vmmap_destroy(vmmap_t *map);

vmarea_t *vmmap_lookup(vmmap_t *map, uint32_t vfn);
int vmmap_map(vmmap_t *map, struct vnode *file, uint32_t lopage, uint32_t npages, int prot, int flags, off_t off, int dir, vmarea_t **new);
int vmmap_remove(vmmap_t *map, uint32_t lopage, uint32_t npages);
int vmmap_is_range_empty(vmmap_t *map, uint32_t startvfn, uint32_t npages);
int vmmap_find_range(vmmap_t *map, uint32_t npages, int dir);

int vmmap_read(vmmap_t *map, const void *vaddr, void *buf, size_t count);
int vmmap_write(vmmap_t *map, void *vaddr, const void *buf, size_t count);

vmmap_t *vmmap_clone(vmmap_t *map);
