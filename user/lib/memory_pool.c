#include <env.h>
#include <lib.h>
#include <mmu.h>

int create_mem_pool(void *va, u_int len, u_int writeable) {
    if ((u_int) va % PAGE_SIZE != 0) {
        return -E_INVAL;
    }
    int perm = PTE_POOL;
    if (writeable) {
        perm |= PTE_D;
    }
    return syscall_create_shared_pool(va, len, perm);
}

int bind_mem_pool(void *va, u_int id, u_int writeable) {
    if ((u_int) va % PAGE_SIZE != 0) {
        return -E_INVAL;
    }
    int perm = PTE_POOL;
    if (writeable) {
        perm |= PTE_D;
    }
    return syscall_bind_shared_pool(va, id, perm);
}

int try_bind_mem_pool(void *va, u_int id, u_int writeable) {
    int r;
    while ((r = bind_mem_pool(va, id, writeable)) < 0) {
        syscall_yield();
    }
    return r;
}
