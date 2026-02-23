#ifndef __OS_LIST_H
#define __OS_LIST_H

#include "os_types.h"

/* 链表节点 (只有指针，没有数据) */
typedef struct os_list_node {
    struct os_list_node *next;
    struct os_list_node *prev;
} os_list_t;

/* 初始化 */
os_inline void os_list_init(os_list_t *list) {
    list->next = list;
    list->prev = list;
}

/* 头部插入 */
os_inline void os_list_insert_after(os_list_t *head, os_list_t *node) {
    head->next->prev = node;
    node->next = head->next;
    head->next = node;
    node->prev = head;
}

/* 尾部插入 (常用) */
os_inline void os_list_insert_before(os_list_t *head, os_list_t *node) {
    head->prev->next = node;
    node->prev = head->prev;
    head->prev = node;
    node->next = head;
}

/* 移除 */
os_inline void os_list_remove(os_list_t *node) {
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = node; // 指向自己，防止野指针
    node->prev = node;
}

/* 判空 */
os_inline int os_list_is_empty(const os_list_t *head) {
    return head->next == head;
}

/* 获取宿主结构体指针 */
#define os_list_entry(node, type, member) \
    os_container_of(node, type, member)

/* 遍历宏 */
#define os_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#endif /* __OS_LIST_H */
