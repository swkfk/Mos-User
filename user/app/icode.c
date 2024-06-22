#include <lib.h>

int main() {
	int r;
#ifdef MOS_DEBUG
	int fd, n;
	char buf[512 + 1];

	debugf("icode: open /motd\n");
	if ((fd = open("/motd", O_RDONLY)) < 0) {
		user_panic("icode: open /motd: %d", fd);
	}

	debugf("icode: read /motd\n");
	while ((n = read(fd, buf, sizeof buf - 1)) > 0) {
		buf[n] = 0;
		debugf("%s\n", buf);
	}

	debugf("icode: close /motd\n");
	close(fd);

	debugf("icode: spawn /init\n");
#endif
	if ((r = spawnl("init.b", NULL)) < 0) {
		user_panic("icode: spawn /init: %d", r);
	}

	// debugf("icode: exiting\n");
	return 0;
}
