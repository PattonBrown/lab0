/* 
 * C 编程技能测试实验代码
 * 为课程预检验
 */

/*
 * 程序实现一个同时支持 FIFO 和 LIFO 的队列
 *
 * 使用单链表表示队列元素
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
 
/*
  创建空队列
  如果没有分配空间则返回 NULL
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/*
  释放队列所用到的所有空间
  如果 q 为 NULL 则没有效果
*/
void q_free(queue_t *q)
{
    if (q == NULL) {
        return;
    }

    list_ele_t *current = q->head;
    while (current != NULL) {
        list_ele_t *next = current->next;
        free(current->value); // 释放字符串内存
        free(current); // 释放链表节点内存
        current = next;
    }

    free(q); // 释放队列结构内存
}

/*
  在队首插入一个元素
  成功则返回 true
  q 为 NULL 或者 无法分配空间则返回 false 
  参数 s 指向待存储的字符串
  函数必须显式地分配空间并拷贝字符串到空间里面
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL) {
        return false;
    }

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    }

    newh->value = malloc(strlen(s) + 1); // 为字符串分配内存
    if (newh->value == NULL) {
        free(newh);
        return false;
    }

    strcpy(newh->value, s); // 拷贝字符串

    if(q->size==0){
        q->tail = newh;
        q->head=newh;
        newh->next=NULL;
    }
    else{
        newh->next = q->head;
        q->head = newh;
    }
    q->size++;
    return true;
}


/*
  在队尾插入一个元素
  成功则返回 true
  q 为 NULL 或者 无法分配空间则返回 false 
  参数 s 指向待存储的字符串
  函数必须显式地分配空间并拷贝字符串到空间里面
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL || s == NULL)
        return false;

    // 创建新节点
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (newt == NULL)
        return false;

    // 分配并拷贝字符串
    newt->value = malloc(strlen(s) + 1);
    if (newt->value == NULL) {
        free(newt);
        return false;
    }
    strcpy(newt->value, s);

    newt->next = NULL;

    // 特殊处理空队列的情况
    if (q->head == NULL) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }

    q->tail = newt;
    q->size++;

    return true;
}
/*
  移除队首元素Attempt to remove element from head of queue.
  成功则返回 true
  队列为 NULL 或者为 空 则返回 false 
  如果 sp 非空 并移除了一个元素，则将移除的字符串拷贝给 *sp
  (至多 bufsize-1 个字符, 加上1个 null 结束符（'\0',0）)
  元素和字符串使用的空间需要释放
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->size == 0)
        return false;

    list_ele_t *tmp = q->head;
    q->head = q->head->next;

    // 如果 sp 非空，拷贝移除的字符串
    if (sp != NULL) {
        strncpy(sp, tmp->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    // 释放节点内存和字符串内存
    free(tmp->value);
    free(tmp);

    q->size--;

    return true;
}

/*
  返回队列的元素数量
  如果队列为 NULL 或者为 空 则返回 0
 */
int q_size(queue_t *q)
{
    if (q == NULL)
        return 0;

    return q->size;
}

/*
  反转队列元素
  如果 q 为 NULL 则没有效果
  该函数不能直接或者间接调用其他函数为队列元素分配任何额外的空间
  (例如, 不能调用 q_insert_head, q_insert_tail, 或者 q_remove_head).
  应该是通过重排现有元素实现
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->head == NULL || q->head->next == NULL)
        return;

    list_ele_t *prev = NULL;
    list_ele_t *current = q->head;
    list_ele_t *next = NULL;

    // 遍历链表反转指针
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    // 交换头尾指针
    list_ele_t *temp = q->head;
    q->head = q->tail;
    q->tail = temp;
}

