#include <lib.h>

// All of them are aligned
char m[4096];
char n[4096];
int count[1024];
int number[1024];

int main(int argc, char **argv) {
    debugf("\n=== Test Memory Pool ===\n");
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

    debugf("\n=== Test Data Race ===\n");
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
                for (int j = 0; j < 3333; j++)
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
        for (int j = 0; j < 3333; j++)
            ;
        count[0] = ans;
    }
    debugf("parent: wait child...\n");
    for (int k = 0; k < 3; k++) {
        wait(rc[k]);
    }
    debugf("parent: read %d, if no race, the answer should be %d\n", count[0], 40000);

    debugf("Now, the race should not happen\n");
    count[0] = 0;
    for (int k = 0; k < 3; k++) {
        rc[k] = fork();
        if (rc[k] < 0) {
            user_panic("fork: %d", rc[k]);
        }

        if (rc[k] == 0) {
            debugf("child%d: increase 10000...\n", k);
            for (int i = 0; i < 10000; i++) {
                mem_pool_lock(idn);
                int ans = count[0];
                ans = ans + 1;
                for (int j = 0; j < 3333; j++)
                    ;
                count[0] = ans;
                mem_pool_unlock(idn);
            }
            debugf("child: exit\n");
            exit();
        }
    }
    debugf("parent: increase 10000...\n");
    for (int i = 0; i < 10000; i++) {
        mem_pool_lock(idn);
        int ans = count[0];
        ans = ans + 1;
        for (int j = 0; j < 3333; j++)
            ;
        count[0] = ans;
        mem_pool_unlock(idn);
    }
    debugf("parent: wait child...\n");
    for (int k = 0; k < 3; k++) {
        wait(rc[k]);
    }
    debugf("parent: read %d\n", count[0]);
    if (count[0] != 40000) {
        user_panic("Race happened!");
    }

    debugf("\n=== Test Pool Bind ===\n");
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
        debugf("Ok!\n");
    }

    debugf("\n=== Test Huge Pool ===\n");
    char *va = (char *) 0x500000;
    int huge_pool = create_mem_pool(va, 4096 * 5 + 1, 1);
    // Although we only alloc (4096 * 5 + 1) bytes, actually we can use total six page
    if (huge_pool < 0) {
        user_panic("Failed: %d", huge_pool);
    }
    debugf("Return pool id: %d at va 0x%x\n", huge_pool, va);
    debugf("prepare data...");
    for (int i = 0; i < 4096 * 6; i++) {
        va[i] = i % 128;
    }
    r = fork();
    if (r < 0) {
        user_panic("Failed: fork");
    }
    if (r == 0) {
        debugf("child: ready to write\n");
        for (int i = 0; i < 4096 * 6; i++) {
            syscall_lock(huge_pool);
            va[i] = (va[i] + i * i) % 128;
            syscall_unlock(huge_pool);
        }
        debugf("child: exit\n");
        exit();
    } else {
        debugf("parent: ready to write\n");
        for (int i = 0; i < 4096 * 6; i++) {
            syscall_lock(huge_pool);
            va[i] = (va[i] + i * i) % 128;
            syscall_unlock(huge_pool);
        }
        wait(r);
    }
    debugf("parent: check the number\n");
    for (int i = 0; i < 4096 * 6; i++) {
        int ans = ((i % 128 + i * i) % 128 + i * i) % 128;
        int r = va[i] == ans;
        if (!r) {
            user_panic("Diff when i=%d\n  Expect: %d, Got: %d", i, ans, va[i]);
        }
    }
    debugf("Ok!\n");
}
