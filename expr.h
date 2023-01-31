#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "numbers.h"

enum token_types{OPERAND_NUM, OPERAND_C, OPERATOR, SEPARATOR_OPEN, SEPARATOR_COLSE, SUB_EXPR};
typedef enum token_types token_types;

struct token{ // le char et rational_t pourrait etre dans une union
    token_types type;
    char c;
    rational_t r;
};
typedef struct token token_t;

struct arbin{
    token_t val;

    struct arbin* left;
    struct arbin* right;
};
typedef struct arbin arbin_t;


typedef arbin_t expression_t;


extern const char operators[];
extern const char prio0[];
extern const char prio1[];
extern const char prio2[];

extern const char separators_open[];
extern const char separators_close[];

extern const char separator_deci[];


arbin_t* new_node(token_t val);
arbin_t* new_node_i(int i);
arbin_t* new_node_op(char c);
arbin_t* new_node_var(char c);
arbin_t* new_node_r(rational_t r);
void free_arb(arbin_t *arb);

bool is_leaf(arbin_t *a);
int operator_greater(char op1, char op2);
arbin_t* read_expr(char *expr_src);

void print_token(token_t t);
void print_expr(expression_t *e);
rational_t calc_arb(arbin_t *arb);
