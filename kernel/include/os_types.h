#ifndef __OS_TYPES_H
#define __OS_TYPES_H

#include <stdint.h>
#include <stddef.h>

/* 1. 基础类型重定义 (增强移植性) */
typedef uint32_t    os_ubase_t; // 32位无符号
typedef int32_t     os_base_t;  // 32位有符号
typedef uint32_t    os_tick_t;  // 时钟节拍
typedef os_base_t   os_err_t;   // 错误码

#define OS_TRUE     1
#define OS_FALSE    0
#define OS_NULL     ((void *)0)

/* 2. 性能优化：内存对齐 (防止 Cache 伪共享) */
#define OS_ALIGN_SIZE      4
#define OS_ALIGN(size, align) (((size) + (align) - 1) & ~((align) - 1))

/* 3. container_of (通过成员地址反推结构体首地址) */
#define os_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define os_inline           static inline

#endif /* __OS_TYPES_H */
