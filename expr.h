#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "numbers.h"

enum token_types{OPERAND_NUM, OPERAND_C, OPERATOR, SEPARATOR_OPEN, SEPARATOR_COLSE, SUB_EXPR};
typedef enum token_types token_types;

struct token{
    token_types type;
    char c;
    rational_t r;
};
typedef struct token token_t;

struct graph_matrice{
    int nb_nodes;
    
    int nb_edges;
    char **edges;
};
typedef struct graph_matrice graph_m_t;

struct expression{
    token_t *elem; // tableau des elements, les indexs correspondent au num des sommets du graph
    graph_m_t *g;
};
typedef struct expression expression_t;


extern const char operators[];
extern const char prio0[];
extern const char prio1[];
extern const char prio2[];

extern const char separators_open[];
extern const char separators_close[];

extern const char separator_deci[];

void init_graph_m(graph_m_t *g, int nb_node_max);
void free_graph_m(graph_m_t *g, int nb_node_max);
void init_expression(expression_t *expr, int nb_nodes_max);
void free_expression(expression_t *expr, int nb_nodes_max);
int add_node(graph_m_t *g);
void add_edge(graph_m_t *g, int n1, int n2);
int add_elem(expression_t *expr, token_t t);
char is_leaf(graph_m_t *g, int node);
int operator_greater(char op1, char op2);
int read_expr(expression_t *expr, char *expr_src);

void print_token(token_t t);
void print_expr(expression_t *e);
rational_t calc_graph(expression_t *expr, int root);
