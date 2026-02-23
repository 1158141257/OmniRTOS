#ifndef __OS_TASK_H
#define __OS_TASK_H

#include "os_object.h"

#define OS_TASK_STATE_INIT      0x00
#define OS_TASK_STATE_READY     0x01
#define OS_TASK_STATE_RUNNING   0x02
#define OS_TASK_STATE_SUSPEND   0x03

typedef void (*os_task_func_t)(void *parameter);

/* 任务控制块 (TCB) */
struct os_task {
    void        *sp;          /* [必须放第一位] 栈顶指针 (SP)，汇编层偏移量固定为 #0 */
    os_object_t parent;       /* 继承基类 */
    void        *entry;
    void        *parameter;
    void        *stack_addr;
    uint32_t    stack_size;
    uint8_t     priority;
    uint8_t     state;
};

typedef struct os_task os_task_t;

void os_task_init(os_task_t *task, const char *name, 
                  os_task_func_t entry, void *parameter,
                  void *stack_addr, uint32_t stack_size,
                  uint8_t priority);

#endif /* __OS_TASK_H */
