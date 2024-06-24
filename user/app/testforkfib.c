#include <lib.h>

unsigned long fib(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return fib(n - 1) + fib(n - 2);
}

#define CHILDN 20
int main(int argc, char **argv) {

    int r[CHILDN];
    for (int i = 0; i < CHILDN; i++) {
        r[i] = fork();
        if (r[i] < 0) {
            user_panic("Fork Fail!");
        }
        if (r[i] == 0) {
            debugf("child%d: fib[%d]=%lu\n", i, CHILDN * 2 - i, fib(CHILDN * 2 - i));
            exit();
        }
    }
    for (int i = 0; i < CHILDN; i++) {
        wait(r[i]);
    }
}