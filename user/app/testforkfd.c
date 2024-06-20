#include <lib.h>

int main(int argc, char **argv) {
    int fd = open("motd", O_RDONLY);
    if (fd < 0) {
        user_panic("Cannot open the 'motd' for %d\n", fd);
    }

    char buf[9] = {0}, buf2[9] = {0};
    int r = read(fd, buf, 8);
    if (r != 8) {
        user_panic("Cannot read the first 8 bytes for %d\n", r);
    }
    printf("Before fork: Read '%s'\n", buf);

    int pid = fork();
    if (pid < 0) {
        user_panic("Cannot fork for %d\n", pid);
    }

    if (pid == 0) {
        // Child
        int r = read(fd, buf2, 8);
        if (r != 8) {
            user_panic("Cannot read the second 8 bytes in child for %d\n", r);
        }
        printf("Child read: '%s'\n", buf2);
    } else {
        // Parent
        int r = read(fd, buf2, 8);
        if (r != 8) {
            user_panic("Cannot read the second 8 bytes in parent for %d\n", r);
        }
        printf("Parent read: '%s'\n", buf2);
    }

    return 0;
}