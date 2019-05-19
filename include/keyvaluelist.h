#ifndef _KEYVALUELIST_H
#define _KEYVALUELIST_H

struct KeyValueNode
{
    char* key;
    char* value;
    struct KeyValueNode* next;
};

/* 链表头节点为空 */
typedef struct KeyValueNode* KeyValueList;

/* 初始化链表，初始化成功返回1，否则返回0 */
int KeyValueList_init(KeyValueList* kvl);

/* 释放链表资源 */
void KeyValueList_free(KeyValueList kvl);

/* 添加节点至链表，采用头插法，添加成功返回1，否则返回0 */
int KeyValueList_append(KeyValueList kvl, const char* key, const char* value);

/* 根据key值删除链表节点，并返回该节点指针 */
struct KeyValueNode* KeyValueList_remove(KeyValueList kvl, const char* key);

/* 根据key值获取节点value值 */
const char* KeyValueList_getValue(KeyValueList kvl, const char* key);

/* 根据key值设置节点value值，设置成功返回1，否则返回0 */
int KeyValueList_setValue(KeyValueList kvl, const char* key, const char* value);

/* 判断链表是否为空，为空返回1，否则返回0 */
int KeyValueList_isEmpty(KeyValueList kvl);

#endif