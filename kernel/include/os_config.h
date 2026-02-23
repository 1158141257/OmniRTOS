#ifndef __OS_CONFIG_H
#define __OS_CONFIG_H

/* 系统支持的最大优先级数量 (0 为最高优先级) */
#define OS_PRIORITY_MAX         32
/* 空闲任务的栈大小 (字节) */
#define OS_IDLE_TASK_STACK_SIZE 256
/* 系统 Tick 频率 (Hz) */
#define OS_TICK_PER_SECOND      1000

#endif /* __OS_CONFIG_H */
