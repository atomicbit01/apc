/*
 * Name: Emmanuel J
 * Date: 27-09-2026
 * Description: Basic Abritary Precision Calculator
 *
 */

#include "apc.h"

static void print_usage(const char *program) {
  fprintf(stderr, "Usage: %s <number1> <operator> <number2>\n\n", program);

  fprintf(stderr, "Operators:\n"
                  "  +   Addition\n"
                  "  -   Subtraction\n"
                  "  *   Multiplication\n"
                  "  /   Division\n\n");

  fprintf(stderr,
          "Eg. \n"
          "%s 1 + -1\n"
          "%s 2 - 1\n"
          "%s 3 '*' 2\n"
          "%s 2 / 2\n",
          program, program, program, program);
}

int main(int argc, char *argv[]) {
  Dlist *head1 = NULL;
  Dlist *tail1 = NULL;

  Dlist *head2 = NULL;
  Dlist *tail2 = NULL;

  Dlist *res_head = NULL;
  Dlist *res_tail = NULL;

  int sign1;
  int sign2;
  int result_sign = POSITIVE;

  int status;

  char operator;

  if (argc != 4) {
    print_usage(argv[0]);
    return FAILURE;
  }

  if (!validate_number(argv[1])) {
    fprintf(stderr, "Error: Invalid first number: %s\n", argv[1]);

    return FAILURE;
  }

  if (!validate_operator(argv[2])) {
    fprintf(stderr, "Error: Invalid operator: %s\n", argv[2]);

    return FAILURE;
  }

  if (!validate_number(argv[3])) {
    fprintf(stderr, "Error: Invalid second number: %s\n", argv[3]);

    return FAILURE;
  }

  operator = argv[2][0];

  if (string_to_list(argv[1], &head1, &tail1, &sign1) != SUCCESS) {
    fprintf(stderr, "Error: Memory allocation failed\n");

    return FAILURE;
  }

  if (string_to_list(argv[3], &head2, &tail2, &sign2) != SUCCESS) {
    fprintf(stderr, "Error: Memory allocation failed\n");

    free_list(&head1, &tail1);

    return FAILURE;
  }

  if (operator == '/' && is_zero(head2)) {
    fprintf(stderr, "Error: Division by zero\n");

    free_list(&head1, &tail1);
    free_list(&head2, &tail2);

    return FAILURE;
  }

  switch (operator) {
  case '+': {
    status = addition(head1, tail1, sign1, head2, tail2, sign2, &res_head,
                      &res_tail, &result_sign);

    break;
  }

  case '-': {
    status = subtraction(head1, tail1, sign1, head2, tail2, sign2, &res_head,
                         &res_tail, &result_sign);

    break;
  }

  case '*': {
    status = multiplication(head1, tail1, sign1, head2, tail2, sign2, &res_head,
                            &res_tail, &result_sign);

    break;
  }

  case '/': {
    status = division(head1, tail1, sign1, head2, tail2, sign2, &res_head,
                      &res_tail, &result_sign);

    break;
  }

  default: {
    status = FAILURE;
    break;
  }
  }

  if (status != SUCCESS) {
    fprintf(stderr, "Error: Arithmetic operation failed\n");

    free_list(&head1, &tail1);
    free_list(&head2, &tail2);
    free_list(&res_head, &res_tail);

    return FAILURE;
  }

  print_result(res_head, result_sign);

  free_list(&head1, &tail1);
  free_list(&head2, &tail2);
  free_list(&res_head, &res_tail);

  return SUCCESS;
}
