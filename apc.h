#ifndef APC_H
#define APC_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE -1

#define POSITIVE 1
#define NEGATIVE -1

typedef struct node {
  int data;
  struct node *prev;
  struct node *next;
} Dlist;

int append_node(Dlist **head, Dlist **tail, int data);

int prepend_node(Dlist **head, Dlist **tail, int data);

void free_list(Dlist **head, Dlist **tail);

int copy_list(Dlist *src, Dlist **dest_head, Dlist **dest_tail);

int validate_number(const char *str);

int validate_operator(const char *str);

int string_to_list(const char *str, Dlist **head, Dlist **tail, int *sign);

void print_list(Dlist *head);

void print_result(Dlist *head, int sign);

int is_zero(Dlist *head);

int compare_magnitude(Dlist *head1, Dlist *head2);

int addition(Dlist *head1, Dlist *tail1, int sign1, Dlist *head2, Dlist *tail2,
             int sign2, Dlist **res_head, Dlist **res_tail, int *result_sign);

int subtraction(Dlist *head1, Dlist *tail1, int sign1, Dlist *head2,
                Dlist *tail2, int sign2, Dlist **res_head, Dlist **res_tail,
                int *result_sign);

int multiplication(Dlist *head1, Dlist *tail1, int sign1, Dlist *head2,
                   Dlist *tail2, int sign2, Dlist **res_head, Dlist **res_tail,
                   int *result_sign);

int division(Dlist *head1, Dlist *tail1, int sign1, Dlist *head2, Dlist *tail2,
             int sign2, Dlist **res_head, Dlist **res_tail, int *result_sign);

#endif
