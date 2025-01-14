#include <limits.h>
#define MAX_TASKS UCHAR_MAX
#define MAX_NAME UCHAR_MAX
#define MAX_DESC USHRT_MAX

typedef struct {
	const char name[MAX_TASKS][MAX_NAME]; // 8.13 kb
	const char desc[MAX_TASKS][MAX_DESC]; // 2088.93 kb
} Tasks;

void save(Tasks tsk)
{

}

Tasks load(void)
{

}
