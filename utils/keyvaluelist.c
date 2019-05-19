#include "keyvaluelist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct KeyValueNode* locate_prev(KeyValueList kvl, const char *key)
{
	KeyValueList pos, prev;
	prev = kvl;
	pos = kvl->next;
	while(pos && !strcmp(key, pos->key))
	{
		prev = pos;
		pos = pos->next;
	}
	return pos == NULL ? NULL : prev;
}

static struct KeyValueNode* locate(KeyValueList kvl, const char *key)
{
	KeyValueList p = kvl->next;
	while(p && strcmp(key, p->key))
		p = p->next;
	return p;
}

/* 初始化链表，初始化成功返回1，否则返回0 */
int KeyValueList_init(KeyValueList* kvl)
{
	KeyValueList head;
	head = (KeyValueList)malloc(sizeof(struct KeyValueNode));
	head->next = NULL;
	head->key = NULL;
	head->value = NULL;
	*kvl = head;
	return head == NULL;
}

/* 释放链表资源，不释放头结点 */
void KeyValueList_free(KeyValueList kvl)
{
	KeyValueList p;
	kvl = kvl->next;
	while(kvl)
	{
		free(kvl->key);
		free(kvl->value);
		p = kvl->next;
		free(kvl);
		kvl = p;
	}
}

/* 添加节点至链表，采用头插法，添加成功返回1，否则返回0 */
int KeyValueList_append(KeyValueList kvl, const char* key, const char* value)
{
	KeyValueList p = (KeyValueList)malloc(sizeof(struct KeyValueNode));
	if(!p)
		return 0;
	p->key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if(!p->key)
		return 0;
	strcpy(p->key,key);

	p->value = (char*)malloc(sizeof(char)*(strlen(value)+1));
	if(!p->value)
		return 0;
	strcpy(p->value, value);

	p->next = kvl->next;
	kvl->next = p;
	return 1;
}

/* 根据key值删除链表节点，并返回该节点指针 */
struct KeyValueNode* KeyValueList_remove(KeyValueList kvl, const char *key)
{
	struct KeyValueNode *node = locate_prev(kvl, key), *var;
	if(!node)
		return NULL;
	var = node->next;
	node->next = var->next;
	return var;
}

/* 根据key值设置节点value值，设置成功返回1，否则返回0 */
int KeyValueList_setValue(KeyValueList kvl, const char *key, const char *value)
{
	struct KeyValueNode *node = locate(kvl, key);
	if(!node)
		KeyValueList_append(kvl, key, value);
	else
	{
		free(node->value);
		node->value = (char*)malloc(sizeof(char) * strlen(value) + 1);
		strcpy(node->value, value);
	}
	return 1;
}

/* 根据key值获取节点value值  找不到返回null*/   
const char* KeyValueList_getValue(KeyValueList kvl, const char* key)
{
	struct KeyValueNode *node = locate(kvl, key);
	return node == NULL ? NULL : node->value;
}

/* 判断链表是否为空，为空返回1，否则返回0 */
int KeyValueList_isEmpty(KeyValueList kvl)
{
	return kvl->next == NULL;
}
