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

    // size_t len = strlen(s) + 1;
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

    size_t len = strlen(s) + 1;
    newh->value = malloc(sizeof(char) * len);
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len);
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
list_ele_t *SortedMerge(list_ele_t *left_list, list_ele_t *right_list)
{
    /* Base cases */
    if (!left_list)
        return right_list;
    else if (!right_list)
        return left_list;
    list_ele_t *result = NULL;

    /* Pick either a or b, and recur */
    if (strcmp(left_list->value, right_list->value) < 0) {
        result = left_list;
        result->next = SortedMerge(left_list->next, right_list);
    } else {
        result = right_list;
        result->next = SortedMerge(left_list, right_list->next);
    }

    return result;
}

void FrontBackSplit(list_ele_t *source,
                    list_ele_t **frontRef,
                    list_ele_t **backRef)
{
    if (source == NULL || source->next == NULL) {
        *frontRef = source;
        *backRef = NULL;
        return;
    }
    list_ele_t *slow = source;
    list_ele_t *fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void MergeSort(list_ele_t **head)
{
    if (*head == NULL || (*head)->next == NULL)
        return;

    list_ele_t *left_list, *right_list;
    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(*head, &left_list, &right_list);

    /* Recursively sort the sublists */
    MergeSort(&left_list);
    MergeSort(&right_list);

    /* answer = merge the two sorted lists together */
    *head = SortedMerge(left_list, right_list);
}



void q_sort(queue_t *q)
{
    if (!q)
        return;
    MergeSort(&q->head);
    list_ele_t *cursor = q->head;
    for (; cursor && cursor->next; cursor = cursor->next)
        ;
    q->tail = cursor;
}
