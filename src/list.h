/* list.h 

   An implementation of doubly liked list 

*/

#ifndef __LIST_H__
#define __LIST_H__ (1)

/* A node of a linked list */
typedef struct list_node_t {
	struct list_node_t * next; /* Pointer to the next node in a linked list     */
	struct list_node_t * prev; /* Pointer to the previous node in a linked list */
	void        * data;        /* The data that this node holds                 */
} list_node_t ;

/* A linked list */
typedef struct list_t {
	list_node_t * head;  /* when typecasted to list_node_t*, it has list_node_t.next */
	list_node_t * tail;  /* when typecasted to list_node_t*, it has list_node_t.prev */
} list_t ;

/* Get the NULL node of the list */
#define LIST_NODE_NULL(list) (list_node_t *)(list)

/* Initialize a list */
void list_init(list_t * list);

/* Add a node to the head of a list */
void list_add_head(list_t *list, list_node_t *node );

/* Add a node to the tail of a list */
void list_add_tail(list_t *list, list_node_t *node );

/* Remove a node from a list */
void list_remove(list_t *list, list_node_t *node );

/* Append all nodes in list2 to the tail of list1 */
void list_append_to_tail(list_t * list1, list_t * list2 );

/* Append all nodes in list2 to the head of list1 */
void list_append_to_head(list_t * list1, list_t * list2 );

/* Destroy a list */
#define list_destory(list)   /* do nothing */

/* Is the list empty? */
#define list_isempty(list)   ( (list)->head == LIST_NODE_NULL((list)) )

#endif /* __LIST_H__ */


