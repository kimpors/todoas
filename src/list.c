#include <stdio.h>
#include <stdint.h>

#define LIST_MAX 255

typedef struct node {
	uint8_t index;
	struct node *next;
	struct node *prev;
} Node;

struct {
	uint8_t index;
	Node items[LIST_MAX];
} buffer;

static Node *head; 
static Node *tail;

Node *new(uint8_t index);
void push(uint8_t index);
void lremove(uint8_t index);
void ledit(uint8_t old, uint8_t new);

void test(void)
{
	head = new(0);
	tail = new(0);

	head->next = tail;
	tail->prev = head;

	Node *curr = head;

	push(12);
	push(13);
	push(14);
	push(15);
	push(16);

	ledit(15, 32);

	while ((curr = curr->next) != NULL)
	{
		printf("index: %d\n", curr->index);
	}
}

Node *new(uint8_t index)
{
	buffer.items[buffer.index].index = index;
	return &buffer.items[buffer.index++];
}

void push(uint8_t index)
{
	Node *prev = tail->prev;
	prev->next = new(index);
	prev->next->prev = prev;
	prev->next->next = tail;
	tail->prev = prev->next;
}

void ledit(uint8_t old, uint8_t new)
{
	Node *curr = head;

	for (curr = head; curr->next; curr = curr->next)
	{
		if (curr->index == old)
		{
			curr->index = new;
		}
	}
}

void lremove(uint8_t index)
{
	Node *curr = head;

	for (curr = head; curr->next; curr = curr->next)
	{
		if (curr->index == index)
		{
			Node *temp = curr->prev;
			temp->next = curr->next;
		}
	}
}
