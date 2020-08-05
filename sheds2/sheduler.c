#include<stdio.h>

#define TASK_COUNT 2

struct task {
    void (*func)(void *);
    void *data;
    int activated;
};

static struct task tasks[TASK_COUNT];

struct task_data {
    char *str;
    struct task *next_task;
};

static int task_run(struct task *task, void *data) {
    task->activated = 0;
    task->func(data);
    
    return 0;
}

static void scheduler(void) {
    int i;
    int fl = 1;

    while (fl) {
        fl = 0;

        for (i=0; i<TASK_COUNT; i++) {
            if (tasks[i].activated) {
                fl = 1;
                task_run(&tasks[i], tasks[i].data);
            }
        }
    }
}

static int task_activate(struct task *task, void *data) {
    task->data = data;
    task->activated = 1;
    
    return 0;
}

static void worker1(void *data) {
    printf("%s\n", (char *)data);
}

static void worker2(void *data) {
    struct task_data *task_data;
    task_data = data;
    printf("%s\n", task_data->str);
    task_activate(task_data->next_task, "First activated");
}

static struct task *task_create(void (*func)(void *), void *data) {
    static int i = 0;

    tasks[i].func = func;
    tasks[i].data = data;

    return &tasks[i++];
}

int main(void) {
    struct task *t1, *t2;
    struct task_data task_data;
    
    t1 = task_create(&worker1, "First");
    t2 = task_create(&worker2, "Second");

    task_data.next_task = t1;
    task_data.str = "Second activated";
    
    task_activate(t2, &task_data);

    scheduler();

    return 0;
}

