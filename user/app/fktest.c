#include <lib.h>

int main() {
	int a = 0;
	int id = 0;

	if ((id = fork()) == 0) {
		if ((id = fork()) == 0) {
			a += 3;

			for (int i = 0; i < 1000; i++) {
				debugf("\t\tthis is child2 :a:%d\n", a);
			}
		}

		a += 2;

		for (int i = 0; i < 1000; i++) {
			debugf("\tthis is child :a:%d\n", a);
		}
	}

	a++;

	for (int i = 0; i < 1000; i++) {
		debugf("this is father: a:%d\n", a);
	}
	return 0;
}
