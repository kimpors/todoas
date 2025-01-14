#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include "cmdio.h"

#define TASKS_MAX 	UCHAR_MAX
#define DESC_MAX	USHRT_MAX

struct task{
	uint8_t index;
	char name[TASKS_MAX][NAME_MAX]; 
	char desc[TASKS_MAX][DESC_MAX];
} tasks;

static char sbuf[DESC_MAX];
static char path[PATH_MAX];

void tksave(void);
void tkload(void);
void tkadd(const char *name, const char *desc);
char *tkgetline(uint8_t lim);

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

	// strcpy(tasks.name[0], "hello");
	// strcpy(tasks.desc[0], "say hello to everyone");
	// save();

	tkload();

	int ch;
	char name_arg[NAME_MAX];
	char desc_arg[DESC_MAX];

	size_t name_max = NAME_MAX;
	size_t desc_max = DESC_MAX;

	coclear();
	do
	{
		for (int i = 0; i < tasks.index; i++)
		{
			printf("%s\t", tasks.name[i]);
			printf("%s\n", tasks.desc[i]);
		}

		printf("\n\n\n[Q]uit A[dd]\n");

		switch(ch = getchar())
		{
			case 'a':
				getchar();
				coclear();
				printf("name: ");
				strcpy(name_arg, tkgetline(NAME_MAX));

				printf("desc: ");
				strcpy(desc_arg, tkgetline(DESC_MAX));

				tkadd(name_arg, desc_arg);
				break;
		}

		coclear();
	} while (ch != 'q');

	tksave();
	return 0;
}

char *tkgetline(uint8_t lim)
{
	int ch;
	uint8_t i;

	for (i = 0; i < lim; i++)
	{
		ch = getchar();

		if (ch == '\n' || ch == EOF)
		{
			break;
		}

		sbuf[i] = ch;
	}

	sbuf[i] = '\0';
	return sbuf;
}


void tkadd(const char *name, const char *desc)
{
	strcpy(tasks.name[tasks.index], name);
	strcpy(tasks.desc[tasks.index], desc);
	tasks.index++;
}

void tksave(void)
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

void tkload(void)
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
