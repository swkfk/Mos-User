#include <lib.h>

char m[4096];
char n[4096];
int count[1024];
int number[1024];

int main(int argc, char **argv) {
    debugf("=== Test Memory Pool ===\n");
    int idm = create_mem_pool(m, 4096, 1);
    debugf("Return pool id: %d\n", idm);

    debugf("Fork: ...\n");
    int r = fork();
    if (r < 0) {
        user_panic("fork: %d", r);
    }

    if (r == 0) {
        debugf("child: write...\n");
        for (int i = 0; i < 4096; i++) {
            m[i] = n[i] = i * 37 % 128;
        }
        debugf("child: exit\n");
        exit();
    } else {
        wait(r);
        debugf("parent: read and check...\n");
        for (int i = 0; i < 4096; i++) {
            if (n[i] != 0 || m[i] != i * 37 % 128) {
                user_panic("Failed: n[%d] = %d (0 is right), m[%d] = %d (%d is right)", i, n[i], i, m[i], i * 37 % 128);
            }
        }
        debugf("parent: passed\n");
    }

    debugf("=== Test Data Race ===\n");
    int idn = create_mem_pool(count, 4096, 1);
    debugf("Return pool id: %d\n", idn);

    debugf("Write Race Will Happen...\n");
    int rc[3];
    for (int k = 0; k < 3; k++) {
        rc[k] = fork();
        if (rc[k] < 0) {
            user_panic("fork: %d", rc[k]);
        }

        if (rc[k] == 0) {
            debugf("child%d: increase 10000...\n", k);
            for (int i = 0; i < 10000; i++) {
                int ans = count[0];
                ans = ans + 1;
                for (int j = 0; j < 333; j++)
                    ;
                count[0] = ans;
            }
            debugf("child: exit\n");
            exit();
        }
    }
    debugf("parent: increase 10000...\n");
    for (int i = 0; i < 10000; i++) {
        int ans = count[0];
        ans = ans + 1;
        for (int j = 0; j < 333; j++)
            ;
        count[0] = ans;
    }
    debugf("parent: wait child...\n");
    for (int k = 0; k < 3; k++) {
        wait(rc[k]);
    }
    debugf("parent: read %d, if no race, the answer should be %d\n", count[0], 40000);

    debugf("=== Test Pool Bind ===\n");
    debugf("bind to another address... it should fail!\n");
    int id_fail = bind_mem_pool(n, 4096, 1);
    debugf("Return value: %d\n", id_fail);

    r = fork();
    if (r < 0) {
        user_panic("fork: %d", r);
    }

    if (r == 0) {
        count[1] = create_mem_pool(number, 4096, 1);
        debugf("child: created mem pool %d\n", count[1]);
        if (count[1] < 0) {
            user_panic("child: fail to create memory pool: %d", count[1]);
        }
        while (count[2] != 1) {
            syscall_yield();
        }
        debugf("child: ready to write...\n");
        for (int i = 0; i < 1000; i++) {
            number[i] = i * i;
        }
        debugf("child: exit\n");
        exit();
    } else {
        int id;
        while ((id = bind_mem_pool(number, count[1], 1)) < 0)
            syscall_yield();
        count[2] = 1;
        debugf("parent: wait child...\n");
        wait(r);
        debugf("parent: check the number...\n");
        for (int i = 0; i < 1000; i++) {
            if (number[i] != i * i) {
                user_panic("Failed: number[%d] = %d (%d is right)", i, number[i], i * i);
            }
        }
    }
}
