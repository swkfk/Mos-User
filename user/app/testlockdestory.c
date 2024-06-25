#include <lib.h>

char mem[8192];

int main() {
    int id = create_mem_pool(mem, 8192, 1);
    int r = fork();
    if (r < 0 || id < 0) {
        user_halt("failed");
    }

    if (r == 0) {
        mem_pool_lock(id);
        debugf("child: get the lock successfully\n");
        for (int i = 0; i < 100; i++)
            syscall_yield();
        // Without Unlock
        exit();
    } else {
        debugf("parent: wait the child\n");
        wait(r);
        mem_pool_lock(id);
        debugf("parent: get the lock successfully\n");
    }
}