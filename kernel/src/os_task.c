#include "os_task.h"

#define OS_XPSR_THUMB   (0x01000000UL)

/* 伪造中断现场，完整填充所有寄存器默认值 */
void *os_hw_stack_init(void *tentry, void *parameter, void *stack_top, void *exit_func) 
{
    struct stack_frame {
        uint32_t r4; uint32_t r5; uint32_t r6; uint32_t r7;
        uint32_t r8; uint32_t r9; uint32_t r10; uint32_t r11;
        uint32_t r0; uint32_t r1; uint32_t r2; uint32_t r3;
        uint32_t r12; uint32_t lr; uint32_t pc; uint32_t xpsr;
    };

    uint32_t stk = (uint32_t)stack_top & ~0x07UL; // 8字节对齐
    stk -= sizeof(struct stack_frame);

    struct stack_frame *frame = (struct stack_frame *)stk;

    /* 硬件自动压栈部分 */
    frame->r0   = (uint32_t)parameter; // R0 传参
    frame->r1   = 0x01010101;          // 调试填充值
    frame->r2   = 0x02020202;
    frame->r3   = 0x03030303;
    frame->r12  = 0x12121212;
    frame->lr   = (uint32_t)exit_func; // 任务退出时的返回地址
    frame->pc   = (uint32_t)tentry;    // 任务入口地址
    frame->xpsr = OS_XPSR_THUMB;       // T位必置1，否则 HardFault

    /* 软件手动压栈部分 (R4 - R11) */
    frame->r4   = 0x04040404; 
    frame->r5   = 0x05050505;
    frame->r6   = 0x06060606;
    frame->r7   = 0x07070707;
    frame->r8   = 0x08080808;
    frame->r9   = 0x09090909;
    frame->r10  = 0x10101010;
    frame->r11  = 0x11111111;

    return (void *)stk;
}

void os_task_init(os_task_t *task, const char *name, 
                  os_task_func_t entry, void *parameter,
                  void *stack_addr, uint32_t stack_size,
                  uint8_t priority)
{
    os_object_init(&task->parent, OS_Object_Class_Thread, name);
    
    task->entry = entry;
    task->parameter = parameter;
    task->stack_addr = stack_addr;
    task->stack_size = stack_size;
    task->priority = priority;

    /* 初始化栈 */
    void *stack_top = (void *)((char *)stack_addr + stack_size);
    task->sp = os_hw_stack_init(entry, parameter, stack_top, OS_NULL);
    task->state = OS_TASK_STATE_INIT;
}
