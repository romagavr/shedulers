#include<stdio.h>

#define TASK_COUNT 2

struct task {
    void (*func)(void *);
    void *data;
};

static struct task tasks[TASK_COUNT];

static void scheduler(void) {
    int i;
    for (i=0; i<TASK_COUNT; i++) {
        tasks[i].func(tasks[i].data);
    }
}

static void worker(void *data) {
    printf("%s\n", (char *)data);
}

static struct task *task_create(void (*func)(void *), void *data) {
    static int i = 0;

    tasks[i].func = func;
    tasks[i].data = data;

    return &tasks[i++];
}

int main(void) {
    task_create(&worker, "First");
    task_create(&worker, "Second");

    scheduler();

    return 0;
}

