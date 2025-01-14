#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define TASKS_MAX 	UCHAR_MAX
#define DESC_MAX	USHRT_MAX

struct {
	const char name[TASKS_MAX][NAME_MAX]; 
	const char desc[TASKS_MAX][DESC_MAX];
} tasks;

static char path[PATH_MAX];

int main(void)
{
	strcpy(path, getpwuid(getuid())->pw_dir);
	strcat(path, "/.local/share/todoas");
	printf("path: %s\n", path);
	return 0;
}
