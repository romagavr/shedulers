#include<stdio.h>
#include<stdlib.h>

#define TASK_COUNT 2

struct task {
	struct task *next;
    struct task *prev;
	void *data;
	void (*func)(void *);	
};

static struct task *list_head;

static struct task* task_create(void (*func)(void *), void *data) {
	struct task *task = malloc(sizeof(struct task));
	if (task == NULL) {
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	task->func = func;
	task->data = data;
	task->next = task->prev = task;
	
	if (!list_head){
		list_head = task;
	} else {
		task->prev = list_head->prev;
		task->next = list_head;
		task->prev->next = task;
		list_head->prev = task;
	}

	return task;
}

static int task_run(struct task *cur, void *data) {
	cur->func(data);
	return 0;
}

void list_traverse() {
    if (list_head != NULL) {
        struct task *t = list_head;
        printf("---- list traverse start ----\n");
        do {
            printf("%s\n",(char *)t->data);
            t = t->next;
        } while (t != list_head);
        printf("---- list traverse end ----\n");
    } else printf("---- empty list ----\n");
}

static struct task *task_get_next(void) {
	if (list_head) {
	    struct task *tmp = list_head;
		if (list_head != list_head->next) {
			list_head = list_head->next;
			list_head->prev = tmp->prev;
			list_head->prev->next = list_head;
			free(tmp);
            return list_head;
		} else {
            list_head = NULL;
            return tmp;
        }
	}
	return NULL;	
}

static void scheduler(void) {
    struct task *cur = list_head;
	while (list_head){
		task_run(cur, cur->data);
		cur = task_get_next();
	}
    free(cur);
}

static void worker2(void *data) {
    printf("%s\n",(char *)data);
}

static void worker1(void *data) {
    printf("%s\n", (char *)data);
}

int main(void) {
    task_create(&worker1, "First create");
    task_create(&worker2, "Second create");
    task_create(&worker2, "Second create again");

    scheduler();

    return 0;
}

