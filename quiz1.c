#include <assert.h>void assert( int expression );
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct __node {
    int value;
    struct __node *next;
} node_t;

void add_entry(node_t **head, int new_value)
{
    node_t **indirect = head;

    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = new_value;
    new_node->next = NULL;

    assert(new_node);
    while (*indirect)
        indirect = &(*indirect)->next;
    *indirect = new_node;
}

node_t *find_entry(node_t *head, int value)
{
    node_t *current = head;
    for (; current && current->value != value; current = current->next)
        /* interate */;
    return current;
}


void remove_entry(node_t **head, node_t *entry)
{
    node_t **indirect = head;

    while ((*indirect) != entry)
        indirect = &(*indirect)->next;

    *indirect = entry->next;
    free(entry);
}

void swap_pair(node_t **head)
{
    node_t *tmp = NULL;
    for (; *head && (*head)->next; head = &(*head)->next->next) {
        tmp = *head;
        *head = (*head)->next;
        tmp->next = (*head)->next;
        (*head)->next = tmp;
    }
}
void shuffle(node_t **head)
{
    int link_length = 0;
    srand(time(NULL));
    node_t *cursor, *old_head;
    cursor = old_head = *head;
    for (; cursor; cursor = cursor->next)
        link_length++; /* interate */

    *head = NULL;
    while (link_length) {  // link-list is non-empty
        node_t **indirect = &old_head;
        for (int i = rand() % link_length; i > 0; i--) {
            indirect = &(*indirect)->next;
        }
        cursor = *indirect;
        *indirect = (*indirect)->next;
        cursor->next = *head;
        *head = cursor;
        link_length--;
    }
}

node_t *rev_reverse(node_t **head, node_t *node)
{
    if (!node->next) {
        *head = node;
        return node;
    }
    node_t *node1 = rev_reverse(head, node->next);
    node1->next = node;
    return node;
}

void recursive_reverse(node_t **head)
{
    if (*head && (*head)->next) {
        node_t *cursor = *head;
        node_t *node1 = rev_reverse(head, (*head)->next);
        node1->next = cursor;
        cursor->next = NULL;
    }
}


// void reverse(node_t **head)
// {
//     node_t *cursor = NULL;
//     while (*head) {
//         node_t *next = (*head)->next;
//         (*head)->next = cursor;
//         cursor = *head;
//         *head = next;
//     }
//     *head = cursor;
// }

void print_list(node_t *head)
{
    for (node_t *current = head; current; current = current->next)
        printf("%d ", current->value);
    printf("\n");
}

int main(int argc, char const *argv[])
{
    node_t *head = NULL;
    print_list(head);

    add_entry(&head, 72);
    add_entry(&head, 101);
    add_entry(&head, 108);
    add_entry(&head, 109);
    add_entry(&head, 110);
    add_entry(&head, 111);

    print_list(head);

    node_t *entry = find_entry(head, 101);
    remove_entry(&head, entry);

    entry = find_entry(head, 111);
    remove_entry(&head, entry);

    print_list(head);

    /* swap pair.
     * See https://leetcode.com/problems/swap-nodes-in-pairs/
     */
    swap_pair(&head);
    print_list(head);

    // reverse(&head); // pointer to pointer
    recursive_reverse(&head);
    print_list(head);

    shuffle(&head);
    print_list(head);
    return 0;
}