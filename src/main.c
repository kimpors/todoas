#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
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
char *tkgetline(uint16_t lim);
void tkadd(char *name, char *desc);

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


	// tasks.index = 1;
	// strcpy(tasks.name[0], "hello");
	// strcpy(tasks.desc[0], "say hello");
	// tksave();
	tkload();

	char ch;
	char name_arg[NAME_MAX];
	char desc_arg[DESC_MAX];

	coclear();
	do
	{
		for (size_t i = 0; i < tasks.index; i++)
		{
			printf("%s\t", tasks.name[i]);
			printf("%s\n", tasks.desc[i]);
		}

		printf("\n\n\n[Q]uit [A]dd\n");
		scanf("%c", &ch);
		coclear();

		switch(ch)
		{
			case 'a':
				system("clear");

				printf("name: ");
				scanf("%[^\n]%*c", name_arg);
				scanf("%[^\n]%*c", desc_arg);

				// strcpy(name_arg, tkgetline(NAME_MAX));
				//
				// printf("desc: ");
				// strcpy(desc_arg, tkgetline(DESC_MAX));

				tkadd(name_arg, desc_arg);
				break;
		}
	} while (ch != 'q');

	tksave();
	return 0;
}

char *tkgetline(uint16_t lim)
{
	int ch;
	size_t i;

	for (i = 0; i < lim; i++)
	{
		ch = getchar();

		if (ch == EOF || ch == '\n')
		{
			break;
		}

		sbuf[i] = ch;
	}

	sbuf[i] = '\0';
	return sbuf;
}

void tkadd(char *name, char *desc)
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
