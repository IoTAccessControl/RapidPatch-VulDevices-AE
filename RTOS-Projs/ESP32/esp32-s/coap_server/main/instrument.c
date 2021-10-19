
__attribute__((__used__)) int fixed_patch_point_hanlder() {
	__asm__ volatile (
	"movi.n	a8, 0\n"
	"addi.n	a8, a8, 1\n"
	"movi.n	a2, 0\n"
	"addi.n	a2, a8, 1\n"
	"movi.n	a3, 0\n"
	"addi.n	a3, a8, 1\n"
	"movi.n	a4, 0\n"
	"addi.n	a4, a8, 1\n"
	"add.n	a2, a2, a8\n"
	"movi.n	a5, 0\n"
	"addi.n	a5, a8, 1\n"
	"add.n	a2, a2, a8\n"

	);
	int sum = 0;
	for (int i = 0; i < 5; i++) {
		sum += i;
	}
	return sum;
} 