#include<stdio.h>
#include<stdlib.h>

#define TASK_COUNT 2

struct message {
    void *data;
    struct message *next;
};

struct task {
    void (*func)(void *);
    struct message *first;
};

struct task_data {
    char *str;
    struct task *next_task;
};

static struct task tasks[TASK_COUNT];

static struct task *task_create(void (*func)(void *)) {
    static int i = 0;

    tasks[i].func = func;
    tasks[i].first = NULL;

    return &tasks[i++];
}

static int task_activate(struct task *task, void *data) {
    struct message *msg;

    msg = malloc(sizeof(struct message));
    if (msg){
        msg->data = data;
        msg->next = task->first;
        task->first = msg;
    }
    return 0;
}

static int task_run(struct task *task, void *data) {
    struct message *msg = data;

    task->first = msg->next;
    task->func(msg->data);
    free(data);
    
    return 0;
}

static void scheduler(void) {
    int i;
    int fl = 1;

    while (fl) {
        fl = 0;

        for (i=0; i<TASK_COUNT; i++) {
            while (tasks[i].first){
                fl = 1;
                task_run(&tasks[i], tasks[i].first);
            }
        }
    }
}

static void worker1(void *data) {
    printf("%s\n", (char *)data);
}

static void worker2(struct task_data *task) {
    printf("%s\n", task->str);
    task_activate(task->next_task, "Message 1 to first");
    task_activate(task->next_task, "Message 2 to first");
}

int main(void) {
    struct task *t1, *t2;
    struct task_data task_data;
    
    t1 = task_create(&worker1);
    t2 = task_create(&worker2);

    task_data.next_task = t1;
    task_data.str = "Second activated";
    
    task_activate(t2, &task_data);

    scheduler();

    return 0;
}

