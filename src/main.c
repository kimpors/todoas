#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include "cmdio.h"
#include "list.h"

#define TASKS_MAX 	UCHAR_MAX
#define DESC_MAX	USHRT_MAX

struct {
	uint8_t len;
	char name[TASKS_MAX][NAME_MAX]; 
	char desc[TASKS_MAX][DESC_MAX];
} tasks;

static char sbuf[DESC_MAX];
static char path[PATH_MAX];

void tksave(void);
void tkload(void);
char *tkgetline(uint16_t lim);
void tkadd(const char *name, const char *desc);

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

	FILE *fp;
	if (!(fp = fopen(path, "r")))
	{
		fclose(fopen(path, "w"));
	}
	fclose(fp);

	tkload();

	int ch;
	uint8_t index;
	char name_arg[NAME_MAX];
	char desc_arg[DESC_MAX];

	coclear();
	do
	{
		for (int i = 0; i < tasks.len; i++)
		{
			printf("%s\t", tasks.name[i]);
			printf("%s\n", tasks.desc[i]);
		}

		printf("\n\n\n[Q]uit A[dd] C[lear]\n");

		switch(ch = cogetch())
		{
			case 'a':
				cogetch();
				coclear();
				printf("name: ");
				strcpy(name_arg, tkgetline(NAME_MAX));

				printf("desc: ");
				strcpy(desc_arg, tkgetline(DESC_MAX));

				tkadd(name_arg, desc_arg);
				break;
			case 'c':
				tasks.len = 0;
				break;
		}
		coclear();
	} while (ch != 'q');

	tksave();
	return 0;
}

char *tkgetline(uint16_t lim)
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
	strcpy(tasks.name[tasks.len], name);
	strcpy(tasks.desc[tasks.len], desc);
	tasks.len++;
}

void tksave(void)
{
	FILE *fp;
	if (!(fp = fopen(path, "wb"))) {
		fprintf(stderr, "error: can't save tasks\n");
		exit(-1);
	}

	fwrite(&tasks.len, sizeof(uint8_t), 1, fp);
	fwrite(tasks.name, sizeof(char[NAME_MAX]), tasks.len, fp);
	fwrite(tasks.desc, sizeof(char[DESC_MAX]), tasks.len, fp);
	fclose(fp);
}

void tkload(void)
{
	FILE *fp;
	if (!(fp = fopen(path, "rb"))) {
		fprintf(stderr, "error: can't load tasks\n");
		exit(-1);
	}

	fread(&tasks.len, sizeof(uint8_t), 1, fp);
	fread(tasks.name, sizeof(char[NAME_MAX]), tasks.len, fp);
	fread(tasks.desc, sizeof(char[DESC_MAX]), tasks.len, fp);
	fclose(fp);
}
