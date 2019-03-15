#ifndef _KEYVALUELIST_H
#define _KEYVALUELIST_H

struct KeyValueNode
{
    char* key;
    char* value;
    struct KeyValueNode* next;
};

/* the list head is empty */
typedef struct KeyValueNode* KeyValueList;

int keyValueList_init(KeyValueList* kvl);
int keyValueList_append(KeyValueList kvl, const char* key, const char* value);
struct KeyValueNode* keyValueList_remove(KeyValueList kvl, const char* key);
const char* keyValueList_getValue(KeyValueList kvl, const char* key);
int keyValueList_setValue(KeyValueList kvl, const char* key, const char* value);

#endif