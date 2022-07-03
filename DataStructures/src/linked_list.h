/*
 * MIT License
 *
 * Copyright (c) 2021 Uriel Rivas
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * https://github.com/Garz4/zoning/blob/master/LICENSE
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

typedef struct ll_node linked_list_node;
struct ll_node {
  int value;
  linked_list_node* next;
};

typedef struct ll linked_list;
struct ll {
  size_t size;
  linked_list_node* head;
  linked_list_node* tail;
};

// Every new linked list needs to be deleted using delete_linked_list(...).
extern
linked_list*
new_linked_list(int value) {
  linked_list* response;

  ALLOCATE(linked_list, response);
  ALLOCATE(linked_list_node, response->head);
  response->size = 1;
  response->head->value = value;
  response->head->next = NULL;
  response->tail = response->head;

  return response;
}

// Example output to screen: "list = {1, 2, 3, 4, 5};"
extern
void
print_linked_list(const linked_list* list) {
  linked_list_node* head = list->head;
  printf("list = {");

  while (head != NULL) {
    printf("%d", head->value);
    head = head->next;

    if (head != NULL) {
      printf(", ");
    }
  }

  printf("};\n");
}

// Linear time, stops when finding the first occurence.
extern
bool
exist_in_linked_list(const linked_list* list, int value) {
  linked_list_node* head = list->head;

  while (head != NULL) {
    if (head->value == value) {
      return true;
    }

    head = head->next;
  }

  return false;
}

extern
void
add_to_linked_list(linked_list* list, int value) {
  if (list == NULL) {
    return;
  }

  ALLOCATE(linked_list_node, list->tail->next);
  list->tail = list->tail->next;
  list->tail->value = value;
  list->tail->next = NULL;
  list->size++;
}

extern
void
delete_linked_list(linked_list* list) {
  if (list == NULL || list->head == NULL) {
    return;
  }

  linked_list_node* node;
  linked_list_node* next;

  for (node = list->head; node != NULL; node = next) {
    next = node->next;
    DEALLOCATE(node);
  }

  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  DEALLOCATE(list);
}

// PENDING: Deletes first occurence of 'target' in the linked list. It frees its memory.
extern
void
delete_in_linked_list(linked_list* list, int target) {
  if (list == NULL || list->head == NULL) {
    return;
  }

  linked_list_node* node = list->head;

  if (node->value == target) {
    list->head = node->next;
    node = node->next;
    // DEALLOCATE(aux);
    list->size--;
    return;
  }

  while (node->next != NULL) {
    if (node->next->value == target) {
      if (node->next == list->tail) {
        list->tail = node;
      }

      //aux = node;
      node = node->next;
      list->head = node;
      // DEALLOCATE(aux);
      list->size--;
      // delete
      return;
    }

    node = node->next;
  }
}

void
reverse_linked_list_nodes(linked_list_node* parent, linked_list_node* node) {
  if (node == NULL) {
    return;
  }

  reverse_linked_list_nodes(node, node->next);
  node->next = parent;
}

// Linear time.
extern
void
reverse_linked_list(linked_list* list) {
  if (list == NULL) {
    return;
  }

  reverse_linked_list_nodes(NULL, list->head);
  linked_list_node* aux = list->head;
  list->head = list->tail;
  list->tail = aux;
}

// By making a copy, deleting that copy is also necessary.
extern
linked_list*
copy_linked_list(const linked_list* list) {
  if (list == NULL || list->head == NULL) {
    return NULL;
  }

  linked_list* copy = new_linked_list(list->head->value);
  linked_list_node* node = list->head->next;

  while (node != NULL) {
    add_to_linked_list(copy, node->value);
    node = node->next;
  }

  return copy;
}

extern
bool
equal_linked_list(const linked_list* lhs, const linked_list* rhs) {
  if (lhs == NULL && rhs == NULL) {
    return true;
  } else if (lhs == NULL || rhs == NULL || lhs->size != rhs->size) {
    return false;
  }

  linked_list_node* lhs_node = lhs->head;
  linked_list_node* rhs_node = rhs->head;

  while (lhs_node != NULL && rhs_node != NULL) {
    if (lhs_node->value != rhs_node->value) {
      return false;
    }

    lhs_node = lhs_node->next;
    rhs_node = rhs_node->next;
  }

  return lhs_node == NULL && rhs_node == NULL;
}

#endif // LINKED_LIST_H_
