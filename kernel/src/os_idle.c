#include "os_task.h"
#include "os_config.h"

static os_task_t idle_task;
static uint8_t   idle_task_stack[OS_IDLE_TASK_STACK_SIZE];

extern void os_sched_insert(os_task_t *task);

/* 空闲任务：当所有用户任务都被挂起时运行 */
static void os_idle_task_entry(void *parameter) {
    while (1) {
        /* WFI：等待中断，让 CPU 睡眠以降低功耗 */
        __asm volatile ("wfi");
    }
}

void os_idle_task_init(void) {
    /* 空闲任务永远放置在最低优先级 */
    os_task_init(&idle_task, "idle", os_idle_task_entry, OS_NULL, 
                 &idle_task_stack[0], OS_IDLE_TASK_STACK_SIZE, 
                 OS_PRIORITY_MAX - 1);
                 
    os_sched_insert(&idle_task);
}
