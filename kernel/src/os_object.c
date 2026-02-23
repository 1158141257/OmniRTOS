#include "os_object.h"

void os_object_init(os_object_t *object, os_obj_type_t type, const char *name) {
    int i = 0;
    
    if (object == OS_NULL) return;
    
    /* 初始化对象类型和标志 */
    object->type = type;
    object->flag = 0;
    
    /* 拷贝名字，最多8个字符 */
    while (name && name[i] != '\0' && i < 7) {
        object->name[i] = name[i];
        i++;
    }
    object->name[i] = '\0';
    
    /* 初始化对象的链表节点 */
    os_list_init(&(object->list));
}
