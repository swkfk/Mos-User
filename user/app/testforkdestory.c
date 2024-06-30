#include <lib.h>

int main(int argc, char **argv) {
    // debugf("Unfortunately, this test will fail in this version's kernel.\n");
    // debugf("Press any key to continue...");
    char temp;
    // read(0, &temp, 1);
    for (int i = 0; i < 100; i++) {
        int r = fork();
        if (r < 0) {
            user_panic("Fork Fail!");
        }
        if (r == 0) {
            syscall_env_destroy(0);
        } else {
            const volatile struct Env *e;

            e = &envs[ENVX(r)];
            debugf("e=%d\n", e);
            while (e->env_id == r && e->env_status != ENV_FREE) {
                debugf("Parent Yield\n");
                syscall_yield();
                debugf("Parent Yield Over\n");
            }
        }
    }
    exit();
}