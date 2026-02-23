#include "os_task.h"

extern void os_sched_init(void);
extern void os_idle_task_init(void);
extern void os_scheduler_start(void);

/* OS 启动主干流程 */
void os_system_start(void) {
    /* 1. 初始化就绪表与位图 */
    os_sched_init();
    
    /* 2. 初始化底层硬件 (此处预留) */
    // board_hw_init();
    
    /* 3. 创建空闲任务 (系统的基石) */
    os_idle_task_init();
    
    /* 4. 触发汇编，启动第一个最高优先级任务！(一去不复返) */
    os_scheduler_start();
}
