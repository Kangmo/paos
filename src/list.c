/*
    <SOURCE-LICENSE>
        [ PA(Peter-Anna) OS ] ========================================

          Peter-Anna OS is a real time OS.
          Peter-Anna OS is made by Peter and Anna.
          Peter and Anna lives in South Korea.
          They love each other so much.

          Peter usually develops PAOS,
          Anna usually feeds hungry Peter.
          Peter sometimes makes money,
          Anna sometimes makes cookie.

          Like shared memory region,
          PAOS is where they live in together.
          They belong to PAOS.
          PAOS belongs to them.

                               03/29/2006
                               Peter and Anna ( kangmo.kim@gmail.com )
        [ License Information ] ======================================
         
          Dual License
            1. GPL
            2. PAOS Commercial License
             
          License Selection Algorithm             
            IF You can open your whole source code
              You are under GPL license
            ELSE
              You are under PAOS Commercial License
            FI
                                
        [ Information on Layering Architecture of PAOS ] =============

          Files in lower layer are not allowed to 
          use,include files in upper layer.

          user    ; user program layer      ; main.c
          pthread ; pthread layer           ; pthread.c
          conc    ; concurrency layer       ; conc_mutex.c conc_cond.c
          evt     ; event layer             ; evt.c
          sched   ; scheduler layer         ; sched.c
          task    ; task layer              ; task.c
          base    ; base layer              ; base.c list.c debug.c
          md      ; machine dependent layer ; md.c md_asm.S uart.c
          config  ; configuration layer     ; config.h
                                
    </SOURCE-LICENSE>
    $Id$
*/
#include "debug.h"
#include "list.h"

/* Initialize a list */
void list_init(list_t * list)
{
    /* Set both the head node and the tail node to the NULL node of the list */
    list->head = LIST_NODE_NULL(list);
    list->tail = LIST_NODE_NULL(list);
}

/* Add a node to the head of a list */
void list_add_head(list_t *list, list_node_t *node )
{
    node->next = list->head;
    node->prev = LIST_NODE_NULL(list);
    
    /* if list->head is LIST_NODE_NULL,  "list->head->prev" equals to "list->tail" */
    list->head->prev = node;
    list->head = node;
}

/* Add a node to the tail of a list */
void list_add_tail(list_t *list, list_node_t *node )
{
    node->next = LIST_NODE_NULL(list);
    node->prev = list->tail;

    /* if list->tail is LIST_NODE_NULL,  "list->tail->next" equals to "list->head" */
    list->tail->next = node;
    list->tail = node;
}

/* Remove a node from a list */
void list_remove(list_t *list, list_node_t *node )
{
    /*
      1. if node->prev is LIST_NODE_NULL, node is the head node
         -  "node->prev->next" becomes "list->head"
         -  "node->prev->next = node->next" means "list->head = node->next"
    */
    node->prev->next = node->next;

    /*
      2. if node->next is LIST_NODE_NULL, node is the tail node
         -  "node->next->prev" becomes "list->tail"
         -  "node->next->tail := node->prev" means "list->tail = node->prev
    */
    node->next->prev = node->prev;
}



/* Append all nodes in list2 to the tail of list1 */
void list_append_to_tail(list_t * list1, list_t * list2 )
{
    if ( ! list_isempty( list2 ) ) {
        
        /* if list1->tail is LIST_NODE_NULL, list1->tail->next is list1->head */
        list1->tail->next = list2->head;
        list1->tail = list2->tail;

        list2->head->prev = list1->tail;
        list2->tail->next = LIST_NODE_NULL( list1 );
    }
}

/* Append all nodes in list2 to the head of list1 */
void list_append_to_head(list_t * list1, list_t * list2 )
{
    list_append_to_tail(list2, list1 );
    list1->head = list2->head;
    list1->tail = list2->tail;
}

