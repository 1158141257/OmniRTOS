#include "os_task.h"
#include "os_config.h"

/* 就绪表与位图 */
os_list_t os_ready_table[OS_PRIORITY_MAX];
uint32_t  os_priority_bitmap = 0;

/* 全局任务指针 */
os_task_t *os_curr_task = OS_NULL;
os_task_t *os_next_task = OS_NULL;

/* --- 硬件抽象：临界区保护 --- */
static inline uint32_t os_enter_critical(void) {
    uint32_t primask;
    __asm volatile ("mrs %0, primask\n"
                    "cpsid i\n" : "=r" (primask) :: "memory");
    return primask;
}
static inline void os_exit_critical(uint32_t level) {
    __asm volatile ("msr primask, %0\n" :: "r" (level) : "memory");
}
/* ------------------------------------------------------------- */

void os_sched_init(void) {
    for (int i = 0; i < OS_PRIORITY_MAX; i++) {
        os_list_init(&os_ready_table[i]);
    }
    os_priority_bitmap = 0;
}

void os_sched_insert(os_task_t *task) {
    if (task == OS_NULL) return;

    /* 进入临界区，保护位图与队列 */
    uint32_t level = os_enter_critical();

    /* 1. 挂入队列 */
    os_list_insert_before(&os_ready_table[task->priority], &task->parent.list);
    /* 2. 置位位图 */
    os_priority_bitmap |= (1UL << task->priority);
    task->state = OS_TASK_STATE_READY;

    /* 退出临界区 */
    os_exit_critical(level);
}

void os_sched_remove(os_task_t *task) {
    if (task == OS_NULL) return;

    /* 进入临界区 */
    uint32_t level = os_enter_critical();

    /* 1. 移出队列 */
    os_list_remove(&task->parent.list);
    /* 2. 如果队列空，清除位图 */
    if (os_list_is_empty(&os_ready_table[task->priority])) {
        os_priority_bitmap &= ~(1UL << task->priority);
    }
    task->state = OS_TASK_STATE_SUSPEND;

    /* 退出临界区 */
    os_exit_critical(level);
}

/* 核心调度算法 */
void os_schedule(void) {
    uint32_t level;
    uint8_t highest_prio;
    os_list_t *node;
    os_task_t *high_task;

    level = os_enter_critical();

    if (os_priority_bitmap == 0) {
        os_exit_critical(level);
        return;
    }

    /* 使用硬件指令寻找最高优先级的任务 */
    /* __builtin_ctz 计算末尾有多少个0，即第一个 1 的位置 */
    highest_prio = __builtin_ctz(os_priority_bitmap);

    /* 获取该优先级队列的第一个任务 */
    node = os_ready_table[highest_prio].next;
    high_task = os_list_entry(node, os_task_t, parent.list);

    if (high_task != os_curr_task) {
        os_next_task = high_task;

        /* 触发 PendSV 进行切换 (向 ICSR 的 bit 28 写 1) */
        *(volatile uint32_t *)0xE000ED04 = (1UL << 28);
        __asm volatile ("dsb");
        __asm volatile ("isb");
    }
    
    os_exit_critical(level);
}
