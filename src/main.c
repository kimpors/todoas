#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>

#define TASKS_MAX 	UCHAR_MAX
#define DESC_MAX	USHRT_MAX

struct task{
	uint8_t index;
	char name[TASKS_MAX][NAME_MAX]; 
	char desc[TASKS_MAX][DESC_MAX];
} tasks;

static char path[PATH_MAX];

void save(void);
void load(void);

int main(void)
{
	struct stat state;

	strcpy(path, getpwuid(getuid())->pw_dir);
	strcat(path, "/.local/share/todoas");

	if (stat(path, &state))
	{
		if (mkdir(path, 0777))
		{
			fprintf(stderr, "can't create directory '%s'\n", path);
			exit(-1);
		}
	}

	strcat(path, "/data.bin");
	printf("path: %s\n", path);

	strcpy(tasks.name[0], "hello");
	strcpy(tasks.desc[0], "say hello to everyone");
	save();

	load();
	printf("name: %s\n", tasks.name[0]);
	printf("desc: %s\n", tasks.desc[0]);
	return 0;
}

void save(void)
{
	FILE *fp;
	if (!(fp = fopen(path, "wb"))) {
		fprintf(stderr, "error: can't save tasks\n");
		exit(-1);
	}

	fwrite(&tasks.index, sizeof(uint8_t), 1, fp);
	fwrite(tasks.name, sizeof(char[NAME_MAX]), tasks.index, fp);
	fwrite(tasks.desc, sizeof(char[DESC_MAX]), tasks.index, fp);
	fclose(fp);
}

void load(void)
{
	FILE *fp;
	if (!(fp = fopen(path, "rb"))) {
		fprintf(stderr, "error: can't load tasks\n");
		exit(-1);
	}

	fread(&tasks.index, sizeof(uint8_t), 1, fp);
	fread(tasks.name, sizeof(char[NAME_MAX]), tasks.index, fp);
	fread(tasks.desc, sizeof(char[DESC_MAX]), tasks.index, fp);
	fclose(fp);
}
