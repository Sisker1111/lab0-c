#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->tail = NULL;
    q->head = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    if (q->head) {
        list_ele_t *tmp = q->head;
        while (q->head) {  // free all element in link-list
            q->head = q->head->next;
            free(tmp->value);
            free(tmp);
            tmp = q->head;
        }
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    if (!newh)  // queue q is not exist || malloc return NULL
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);

    if (!q->head) {
        q->tail = newh;
    }
    newh->next = q->head;
    q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)  // queue q is not exist || malloc return NULL
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    if (!q->head) {
        newh->next = q->head;
        q->head = newh;
        q->tail = newh;
        q->size++;
        return true;
    }
    newh->next = NULL;
    q->tail->next = newh;
    q->tail = q->tail->next;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !q->head)  // queue q is not exist
        return false;
    if (sp) {
        if (bufsize > strlen(q->head->value)) {
            strncpy(sp, q->head->value, strlen(q->head->value));
            sp[strlen(q->head->value)] = '\0';
        } else {
            strncpy(sp, q->head->value, bufsize - 1);
            sp[bufsize - 1] = '\0';
        }
    }

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *cursor = NULL;
    list_ele_t **indirect = &q->head;
    q->tail = q->head;
    while (*indirect) {
        list_ele_t *next = (*indirect)->next;
        (*indirect)->next = cursor;
        cursor = *indirect;
        *indirect = next;
    }
    *indirect = cursor;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *merge(list_ele_t *left_list, list_ele_t *right_list)
{
    list_ele_t *tmp = NULL;
    if (strcmp(left_list->value, right_list->value) <= 0) {
        tmp = left_list;
        left_list = left_list->next;
    } else {
        tmp = right_list;
        right_list = right_list->next;
    }

    list_ele_t *head = tmp;
    while (left_list && right_list) {
        if (strcmp(left_list->value, right_list->value) <= 0) {
            tmp->next = left_list;
            tmp = tmp->next;
            left_list = left_list->next;
        } else {
            tmp->next = right_list;
            tmp = tmp->next;
            right_list = right_list->next;
        }
    }

    if (left_list)
        tmp->next = left_list;
    if (right_list)
        tmp->next = right_list;

    return head;
}

void merge_sort(list_ele_t **head)
{
    if (!(*head) || !(*head)->next)
        return;
    list_ele_t *fast = (*head)->next;
    list_ele_t *slow = *head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    merge_sort(head);
    merge_sort(&fast);

    *head = merge(*head, fast);
}


void q_sort(queue_t *q)
{
    if (!q)
        return;
    merge_sort(&q->head);
    if (!q->tail)
        return;
    while (q->tail->next)
        q->tail = q->tail->next;
}
