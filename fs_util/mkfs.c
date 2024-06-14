#include <stdio.h>
#include <string.h>

#define DISK_SIZE 500 * 1024 /* 500KB */

struct _dir {
	int offset;
	int size;
	char   name[128];
};

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Usage: ./mkfs fs.img exe1 exe2 ...\n");
		return -1;
	}
	int null_data = 0;
	FILE *fsimg = fopen(argv[1], "wb");
	for (int i = 0; i < DISK_SIZE / sizeof(int); i++) {
		fwrite(&null_data, sizeof(int), 1, fsimg);
	}
	fflush(fsimg);

	size_t total_size = 0;
	int n_files = argc - 2;
	size_t img_head = (n_files + 1) * sizeof(struct _dir); /* a DIR + n FILE entries from the img start*/

	char buf[512];
	struct _dir dir = {0, n_files, 0}; /* DIR entry has `size` as the number of files*/
	fseek(fsimg, 0, SEEK_SET);
	fwrite(&dir, sizeof(struct _dir), 1, fsimg);

	size_t f_start = (img_head + 512) / 512 * 512; /* entries occupy the sectors and files start at some sectors. */

    for (int i = 2; i < argc; i++) {
		FILE *f = fopen(argv[i], "rb");
		if (!f) {
			printf("File `%s` cannot open.\n", argv[i]);
			break;
		}
		fseek(f, 0, SEEK_END);
		long f_size = ftell(f);
		if (-1 == f_size) {
			printf("Invalid file: %s\n", argv[i]);
			break;
		}
		struct _dir cf = {f_start + total_size, f_size, 0};
		strcpy(cf.name, argv[i]);
		/*write the entry*/
		fseek(fsimg, (i - 1) * sizeof(struct _dir), SEEK_SET);
		fwrite(&cf, sizeof(struct _dir), 1, fsimg);
		/*write the file content*/
		fseek(fsimg, f_start + total_size, SEEK_SET);
		fseek(f, 0, SEEK_SET);
		while(!feof(f)) {
			size_t r = fread(buf, 1, sizeof(buf), f);
			if (r == 0) break;
			fwrite(buf, 1, r, fsimg);
		}
		fclose(f);
		printf("Adding %s into %s, with %ld bytes data at 0x%lx.\n", argv[i], argv[1], f_size, f_start + total_size);
		total_size += f_size;
	}

	fclose(fsimg);

	return 0;
}

