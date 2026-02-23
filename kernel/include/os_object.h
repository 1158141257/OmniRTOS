#ifndef __OS_OBJECT_H
#define __OS_OBJECT_H

#include "os_types.h"
#include "os_list.h"

/* 对象类型枚举 */
typedef enum {
    OS_Object_Class_Null = 0,
    OS_Object_Class_Thread,
    OS_Object_Class_Semaphore,
    OS_Object_Class_Mutex,
    OS_Object_Class_Unknown,
} os_obj_type_t;

/* 基类定义 */
struct os_object {
    char        name[8];    // 对象名 (调试用)
    uint8_t     type;       // 类型
    uint8_t     flag;       // 标志
    os_list_t   list;       // 全局链表节点
};

typedef struct os_object os_object_t;

void os_object_init(os_object_t *object, os_obj_type_t type, const char *name);

#endif /* __OS_OBJECT_H */
