#include "apc.h"

static void remove_leading_zeroes(Dlist **head, Dlist **tail) {
  Dlist *temp;

  while (*head != NULL && (*head)->data == 0 && (*head)->next != NULL) {
    temp = *head;

    *head = (*head)->next;
    (*head)->prev = NULL;

    free(temp);
  }

  if (*head == NULL) {
    *tail = NULL;
  }
}

int append_node(Dlist **head, Dlist **tail, int data) {
  Dlist *new_node;

  new_node = malloc(sizeof(Dlist));

  if (new_node == NULL) {
    return FAILURE;
  }

  new_node->data = data;
  new_node->next = NULL;
  new_node->prev = *tail;

  if (*tail != NULL) {
    (*tail)->next = new_node;
  } else {
    *head = new_node;
  }

  *tail = new_node;

  return SUCCESS;
}

int prepend_node(Dlist **head, Dlist **tail, int data) {
  Dlist *new_node;

  new_node = malloc(sizeof(Dlist));

  if (new_node == NULL) {
    return FAILURE;
  }

  new_node->data = data;
  new_node->prev = NULL;
  new_node->next = *head;

  if (*head != NULL) {
    (*head)->prev = new_node;
  } else {
    *tail = new_node;
  }

  *head = new_node;

  return SUCCESS;
}

void free_list(Dlist **head, Dlist **tail) {
  Dlist *temp;
  Dlist *current = *head;

  while (current != NULL) {
    temp = current->next;
    free(current);
    current = temp;
  }

  *head = NULL;
  *tail = NULL;
}

int is_zero(Dlist *head) {
  if (head == NULL) {
    return 0;
  }

  return (head->data == 0 && head->next == NULL);
}

int validate_number(const char *str) {
  int i = 0;

  if (str == NULL || str[0] == '\0') {
    return 0;
  }

  if (str[0] == '+' || str[0] == '-') {
    i++;
  }

  if (str[i] == '\0') {
    return 0;
  }

  while (str[i] != '\0') {
    if (!isdigit((unsigned char)str[i])) {
      return 0;
    }

    i++;
  }

  return 1;
}

int validate_operator(const char *str) {
  if (str == NULL) {
    return 0;
  }

  if (str[0] == '\0' || str[1] != '\0') {
    return 0;
  }

  switch (str[0]) {
  case '+':
  case '-':
  case '*':
  case '/':
    return 1;

  default:
    return 0;
  }
}

int string_to_list(const char *str, Dlist **head, Dlist **tail, int *sign) {
  int i = 0;

  *head = NULL;
  *tail = NULL;

  *sign = POSITIVE;

  if (str[0] == '+') {
    *sign = POSITIVE;
    i++;
  } else if (str[0] == '-') {
    *sign = NEGATIVE;
    i++;
  }

  while (str[i] == '0') {
    i++;
  }

  if (str[i] == '\0') {
    if (append_node(head, tail, 0) != SUCCESS) {
      return FAILURE;
    }

    *sign = POSITIVE;

    return SUCCESS;
  }

  while (str[i] != '\0') {
    if (append_node(head, tail, str[i] - '0') != SUCCESS) {
      free_list(head, tail);
      return FAILURE;
    }

    i++;
  }

  return SUCCESS;
}

int copy_list(Dlist *src, Dlist **dest_head, Dlist **dest_tail) {
  *dest_head = NULL;
  *dest_tail = NULL;

  while (src != NULL) {
    if (append_node(dest_head, dest_tail, src->data) != SUCCESS) {
      free_list(dest_head, dest_tail);
      return FAILURE;
    }

    src = src->next;
  }

  return SUCCESS;
}

void print_list(Dlist *head) {
  while (head != NULL) {
    putchar(head->data + '0');
    head = head->next;
  }
}

void print_result(Dlist *head, int sign) {
  if (sign == NEGATIVE && !is_zero(head)) {
    putchar('-');
  }

  print_list(head);
  putchar('\n');
}

int compare_magnitude(Dlist *head1, Dlist *head2) {
  int len1 = 0;
  int len2 = 0;

  Dlist *temp1;
  Dlist *temp2;

  temp1 = head1;
  temp2 = head2;

  while (temp1 != NULL) {
    len1++;
    temp1 = temp1->next;
  }

  while (temp2 != NULL) {
    len2++;
    temp2 = temp2->next;
  }

  if (len1 > len2) {
    return 1;
  }

  if (len1 < len2) {
    return -1;
  }

  while (head1 != NULL && head2 != NULL) {
    if (head1->data > head2->data) {
      return 1;
    }

    if (head1->data < head2->data) {
      return -1;
    }

    head1 = head1->next;
    head2 = head2->next;
  }

  return 0;
}

static int addition_magnitude(Dlist *tail1, Dlist *tail2, Dlist **res_head,
                              Dlist **res_tail) {
  int carry = 0;

  *res_head = NULL;
  *res_tail = NULL;

  while (tail1 != NULL || tail2 != NULL || carry != 0) {
    int sum = carry;

    if (tail1 != NULL) {
      sum += tail1->data;
      tail1 = tail1->prev;
    }

    if (tail2 != NULL) {
      sum += tail2->data;
      tail2 = tail2->prev;
    }

    if (prepend_node(res_head, res_tail, sum % 10) != SUCCESS) {
      free_list(res_head, res_tail);
      return FAILURE;
    }

    carry = sum / 10;
  }

  return SUCCESS;
}

static int subtraction_magnitude(Dlist *tail1, Dlist *tail2, Dlist **res_head,
                                 Dlist **res_tail) {
  int borrow = 0;

  *res_head = NULL;
  *res_tail = NULL;

  while (tail1 != NULL) {
    int value;

    value = tail1->data - borrow;

    if (tail2 != NULL) {
      value -= tail2->data;
      tail2 = tail2->prev;
    }

    if (value < 0) {
      value += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }

    if (prepend_node(res_head, res_tail, value) != SUCCESS) {
      free_list(res_head, res_tail);
      return FAILURE;
    }

    tail1 = tail1->prev;
  }

  remove_leading_zeroes(res_head, res_tail);

  if (*res_head == NULL) {
    if (append_node(res_head, res_tail, 0) != SUCCESS) {
      return FAILURE;
    }
  }

  return SUCCESS;
}

int addition(Dlist *head1, Dlist *tail1, int sign1, Dlist *head2, Dlist *tail2,
             int sign2, Dlist **res_head, Dlist **res_tail, int *result_sign) {
  int comparison;

  *res_head = NULL;
  *res_tail = NULL;

  if (sign1 == sign2) {
    if (addition_magnitude(tail1, tail2, res_head, res_tail) != SUCCESS) {
      return FAILURE;
    }

    *result_sign = sign1;
  } else {

    comparison = compare_magnitude(head1, head2);

    if (comparison == 0) {

      if (append_node(res_head, res_tail, 0) != SUCCESS) {
        return FAILURE;
      }

      *result_sign = POSITIVE;
    } else if (comparison > 0) {

      if (subtraction_magnitude(tail1, tail2, res_head, res_tail) != SUCCESS) {
        return FAILURE;
      }

      *result_sign = sign1;
    } else {

      if (subtraction_magnitude(tail2, tail1, res_head, res_tail) != SUCCESS) {
        return FAILURE;
      }

      *result_sign = sign2;
    }
  }

  if (is_zero(*res_head)) {
    *result_sign = POSITIVE;
  }

  return SUCCESS;
}

int subtraction(Dlist *head1, Dlist *tail1, int sign1, Dlist *head2,
                Dlist *tail2, int sign2, Dlist **res_head, Dlist **res_tail,
                int *result_sign) {
  return addition(head1, tail1, sign1, head2, tail2, -sign2, res_head, res_tail,
                  result_sign);
}

static int increment_list(Dlist **head, Dlist **tail) {
  Dlist *current = *tail;
  int carry = 1;

  while (current != NULL && carry != 0) {
    int value;

    value = current->data + carry;

    current->data = value % 10;
    carry = value / 10;

    current = current->prev;
  }

  if (carry != 0) {
    if (prepend_node(head, tail, carry) != SUCCESS) {
      return FAILURE;
    }
  }

  return SUCCESS;
}

static int append_zeroes(Dlist **head, Dlist **tail, int count) {
  while (count > 0) {
    if (append_node(head, tail, 0) != SUCCESS) {
      return FAILURE;
    }

    count--;
  }

  return SUCCESS;
}

static int multiplication_magnitude(Dlist *head1, Dlist *tail1, Dlist *head2,
                                    Dlist *tail2, Dlist **res_head,
                                    Dlist **res_tail) {
  Dlist *result_head = NULL;
  Dlist *result_tail = NULL;
  Dlist *b;
  int shift = 0;

  *res_head = NULL;
  *res_tail = NULL;

  if (is_zero(head1) || is_zero(head2)) {
    if (append_node(res_head, res_tail, 0) != SUCCESS)
      return FAILURE;

    return SUCCESS;
  }

  if (append_node(&result_head, &result_tail, 0) != SUCCESS)
    return FAILURE;

  b = tail2;

  while (b != NULL) {
    Dlist *partial_head = NULL;
    Dlist *partial_tail = NULL;
    Dlist *a = tail1;
    int carry = 0;

    while (a != NULL || carry != 0) {
      int product = carry;

      if (a != NULL) {
        product += a->data * b->data;
        a = a->prev;
      }

      if (prepend_node(&partial_head, &partial_tail, product % 10) != SUCCESS) {
        free_list(&partial_head, &partial_tail);
        free_list(&result_head, &result_tail);
        return FAILURE;
      }

      carry = product / 10;
    }

    if (append_zeroes(&partial_head, &partial_tail, shift) != SUCCESS) {
      free_list(&partial_head, &partial_tail);
      free_list(&result_head, &result_tail);
      return FAILURE;
    }

    {
      Dlist *new_head = NULL;
      Dlist *new_tail = NULL;

      if (addition_magnitude(result_tail, partial_tail, &new_head, &new_tail) !=
          SUCCESS) {
        free_list(&partial_head, &partial_tail);
        free_list(&result_head, &result_tail);
        return FAILURE;
      }

      free_list(&result_head, &result_tail);

      result_head = new_head;
      result_tail = new_tail;
    }

    free_list(&partial_head, &partial_tail);

    shift++;
    b = b->prev;
  }

  remove_leading_zeroes(&result_head, &result_tail);

  *res_head = result_head;
  *res_tail = result_tail;

  return SUCCESS;
}

int multiplication(Dlist *head1, Dlist *tail1, int sign1, Dlist *head2,
                   Dlist *tail2, int sign2, Dlist **res_head, Dlist **res_tail,
                   int *result_sign) {
  if (multiplication_magnitude(head1, tail1, head2, tail2, res_head,
                               res_tail) != SUCCESS) {
    return FAILURE;
  }

  *result_sign = sign1 * sign2;

  if (is_zero(*res_head)) {
    *result_sign = POSITIVE;
  }

  return SUCCESS;
}

int division(Dlist *head1, Dlist *tail1, int sign1, Dlist *head2, Dlist *tail2,
             int sign2, Dlist **res_head, Dlist **res_tail, int *result_sign) {
  Dlist *remainder_head = NULL;
  Dlist *remainder_tail = NULL;

  Dlist *quotient_head = NULL;
  Dlist *quotient_tail = NULL;

  if (copy_list(head1, &remainder_head, &remainder_tail) != SUCCESS) {
    return FAILURE;
  }

  if (append_node(&quotient_head, &quotient_tail, 0) != SUCCESS) {
    free_list(&remainder_head, &remainder_tail);

    return FAILURE;
  }

  while (compare_magnitude(remainder_head, head2) >= 0) {
    Dlist *new_remainder_head = NULL;
    Dlist *new_remainder_tail = NULL;

    if (subtraction_magnitude(remainder_tail, tail2, &new_remainder_head,
                              &new_remainder_tail) != SUCCESS) {
      free_list(&remainder_head, &remainder_tail);

      free_list(&quotient_head, &quotient_tail);

      return FAILURE;
    }

    free_list(&remainder_head, &remainder_tail);

    remainder_head = new_remainder_head;
    remainder_tail = new_remainder_tail;

    if (increment_list(&quotient_head, &quotient_tail) != SUCCESS) {
      free_list(&remainder_head, &remainder_tail);

      free_list(&quotient_head, &quotient_tail);

      return FAILURE;
    }
  }

  free_list(&remainder_head, &remainder_tail);

  *res_head = quotient_head;
  *res_tail = quotient_tail;

  *result_sign = sign1 * sign2;

  if (is_zero(*res_head)) {
    *result_sign = POSITIVE;
  }

  return SUCCESS;
}
