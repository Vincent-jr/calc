#include "expr.h"


const char operators[] = "+-/*^";

const char prio0[] = "+-";
const char prio1[] = "/*";
const char prio2[] = "^";

const char separators_open[] = "(";
const char separators_close[] = ")";

const char separator_deci[] = ".,";


void init_graph_m(graph_m_t *g, int nb_node_max){
    g->nb_nodes = 0;
//    g->nodes = calloc(nb_node_max, sizeof(int));
    
    g->nb_edges = 0;
    g->edges = malloc(sizeof(char*)*nb_node_max);
    for(int i=0; i < nb_node_max; i++){
        g->edges[i] = calloc(nb_node_max, sizeof(char));
    }
}

void free_graph_m(graph_m_t *g, int nb_nodes_max){
    //free(g->nodes);
    for(int i=0; i<nb_nodes_max; i++){
        free(g->edges[i]);
    }
    free(g->edges);
}

void init_expression(expression_t *expr, int nb_nodes_max){
    expr->elem = malloc(sizeof(token_t) * nb_nodes_max);
    
    expr->g = malloc(sizeof(graph_m_t));
    init_graph_m(expr->g, nb_nodes_max);
}

void free_expression(expression_t *expr, int nb_nodes_max){
    free_graph_m(expr->g, nb_nodes_max);
    free(expr->elem);
}


int add_node(graph_m_t *g){
    //g->nodes[g->nb_nodes] = c;
    return g->nb_nodes++;
}

void add_edge(graph_m_t *g, int n1, int n2){
    if(n1 < g->nb_nodes && n1 >= 0 && n2 < g->nb_nodes && n2 >= 0){
        g->edges[n1][n2] = 1;
        g->nb_edges++;
    }
}

int add_elem(expression_t *expr, token_t t){
    int new_elem_ind = add_node(expr->g);
    expr->elem[new_elem_ind] = t;
    return new_elem_ind;
}

char is_leaf(graph_m_t *g, int node){
    int nb_adj = 0;
    for(int i=0; i<g->nb_nodes; i++){
        if(g->edges[node][i]){
            nb_adj++;
        }
    }
    if(nb_adj > 1){
        return 0;
    }else{
        return 1;
    }
}

int operator_greater(char op1, char op2){
    if(strchr(prio2, op1)){
        return 1;
    }else if(strchr(prio1, op1)){
        if(strchr(prio0, op2)){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

int read_expr(expression_t *expr, char *expr_src){
    printf("################## read_expr ####################\n");
    char *expr_start = expr_src;
    graph_m_t *g = expr->g;
    
    int root; // racine de l'arbre syntaxique
    
    char err = 0; // boolen
    
    token_types token_type;
    token_types last_token_type;
    
    //long token_l;
    rational_t elem_r;
    //char *token_end;
    char *elem_end;
    
    int current_ops_nodes[100]; //pile des opérateurs pouvant etre affectés par la prio des opérations
    int cur_ops_nos_ind = 0; // indique le dessus de la pile
    int prev_op_node; // num de sommet de l'operateur précédent
    int new_opt_node; // num du nouveau sommet
    
    int last_opd_node;
    
    add_elem(expr, (token_t){OPERAND_NUM, 0, (rational_t){0, 1}});
    
    if(expr_src[0] == '-'){
        //add_node(g, '-');
        add_elem(expr, (token_t){OPERAND_C, '-', (rational_t){0, 0}});
        expr_src++;
    }else if(expr_src[0] == '+'){
        //add_node(g, '+');
        add_elem(expr, (token_t){OPERAND_C, '+', (rational_t){0, 0}});
        expr_src++;
    }else{
        //add_node(g, '+');
        add_elem(expr, (token_t){OPERAND_C, '+', (rational_t){0, 0}});
    }
    
    add_edge(g, g->nb_nodes-1, g->nb_nodes-2);
    root = g->nb_nodes-1;
    current_ops_nodes[cur_ops_nos_ind] = g->nb_nodes-1;
    last_token_type = OPERATOR;
    
    
    char c;
    while((c = expr_src[0]) != '\0'){
        printf("----------------\n\n");
        
        //token_l = strtol(expr_src, &token_end, 10);
        elem_r = read_r(expr_src, &elem_end, separator_deci);
        
        if(strchr(operators, c)){
            
            new_opt_node = add_elem(expr, (token_t){OPERAND_C, c, (rational_t){0, 0}});;
            
            if(last_token_type == OPERATOR){
                err = 1;
                break;
                
                
            }else if(last_token_type == OPERAND_NUM || last_token_type == SUB_EXPR){ // ou operand c
                printf("op %c %c %d\n", c, (char)expr->elem[current_ops_nodes[cur_ops_nos_ind]].c, operator_greater(c, current_ops_nodes[cur_ops_nos_ind]));
                
                if(operator_greater(c, expr->elem[current_ops_nodes[cur_ops_nos_ind]].c)){
                    add_edge(g, new_opt_node, last_opd_node);
                    
                    cur_ops_nos_ind++;
                    current_ops_nodes[cur_ops_nos_ind] = new_opt_node;
                    
                }else{
                    printf("prio inf, %d, %d\n", cur_ops_nos_ind, g->nb_edges);
                    prev_op_node = current_ops_nodes[cur_ops_nos_ind];
                    
                    add_edge(g, prev_op_node, last_opd_node);
                    cur_ops_nos_ind--; // on dépile
                    
                    while(cur_ops_nos_ind >= 0 && !operator_greater(c, expr->elem[current_ops_nodes[cur_ops_nos_ind]].c)){
                        add_edge(g, current_ops_nodes[cur_ops_nos_ind], prev_op_node);
                        prev_op_node = current_ops_nodes[cur_ops_nos_ind];
                        cur_ops_nos_ind--;
                    }
                    
                    add_edge(g, new_opt_node, prev_op_node);
                    
                    if(cur_ops_nos_ind < 0){// on a "remonté" tous les opérateurs
                        root = new_opt_node;
                    }
                    
                    cur_ops_nos_ind++;
                    current_ops_nodes[cur_ops_nos_ind] = new_opt_node;
                }
            }
            
            token_type = OPERATOR;
            expr_src++;
            
        }else if(strchr(separators_open, c)){
            token_type = SUB_EXPR;
            
            
            int sub_expr_len = (strrchr(expr_src+1, ')') - (expr_src+1)); // +1 car expr_src est une parenthèse
            char* sub_expr_src = malloc(sizeof(char)*(sub_expr_len+1));// +1 pr le \0

            strncpy(sub_expr_src, expr_src+1, sub_expr_len);
            sub_expr_src[sub_expr_len] = '\0';
            
            last_opd_node = read_expr(expr, sub_expr_src);
            free(sub_expr_src);

            expr_src += 2 + sub_expr_len;
            
        }else if(strchr(separators_close, c)){
            last_token_type = SEPARATOR_COLSE;
            
            
        }else if(expr_src != elem_end){
            token_type = OPERAND_NUM;
            
            expr_src = elem_end;
            print_rational(elem_r);
            
            if(last_token_type == OPERAND_NUM){
                printf("db num\n");
                err = 1;
                break;
            }
            
            token_t t = {token_type, 0, elem_r};
            
            last_opd_node = add_elem(expr, t);
        
        }else{
            err = 1;
            break;
        }
        
        last_token_type = token_type;
        printf("last type %d\n\n", last_token_type);
    }
    
    
    if((last_token_type == OPERAND_NUM || last_token_type == SUB_EXPR)&& cur_ops_nos_ind >= 0){ // ou operand c
        prev_op_node = current_ops_nodes[cur_ops_nos_ind];
        
        add_edge(g,current_ops_nodes[cur_ops_nos_ind], last_opd_node);
        cur_ops_nos_ind--;
        
        while(cur_ops_nos_ind >= 0 && !operator_greater('+', expr->elem[current_ops_nodes[cur_ops_nos_ind]].c)){
            add_edge(g, current_ops_nodes[cur_ops_nos_ind], prev_op_node);
            prev_op_node = current_ops_nodes[cur_ops_nos_ind];
            cur_ops_nos_ind--;
        }
    }
    
    if(err){
        int pos = expr_src - expr_start;
        printf("Erreure de syntaxe au caractère %d:\n", pos+1);
        printf("%s\n", expr_start);
        for(int i=0; i < pos; i++){
            printf(" ");
        }
        printf("^\n");
    }
    
    printf("################ fin read #################\n");
    
    return root;
}

void print_token(token_t t){
    switch(t.type){
        case OPERAND_NUM:
            print_rational(t.r);
            break;
        case OPERAND_C:
            printf("%c", t.c);
            break;
        case OPERATOR:
            printf("%c", t.c);
            break;
        default:
            break;
    }
}

void print_expr(expression_t *e){
    for(int i=0; i < e->g->nb_nodes; i++){
        printf("%d (", i);
        print_token(e->elem[i]);
        printf(") : ");
        for(int j=0; j < e->g->nb_nodes; j++){
            if(e->g->edges[i][j]){
                printf("%d, ", j);
            }
        }
        printf("\n");
    }
}

rational_t calc_graph(expression_t *expr, int root){
    rational_t res;
    rational_t ops[2]; // potentiellement faire un tab de pointeur pr distinguer initialisé de pas initialisé dans le cas d'un operateur unaire
    int nb_op = 0;
    graph_m_t *g = expr->g;
    
    for(int i=0; i < g->nb_nodes && nb_op < 2; i++){
        if(g->edges[root][i]){
            if(is_leaf(g, i)){
                ops[nb_op] = expr->elem[i].r;
            }else{
                ops[nb_op] = calc_graph(expr, i);
            }
            nb_op++;
        }
    }
    
    switch(expr->elem[root].c){ // tester si elem[root] est bien de type OPERATOR
        case '*':
            //res = ops[0] * ops[1];
            res = mult_r(ops[0], ops[1]);
            break;
        case '/':
            //res = ops[0] / ops[1];
            res = div_r(ops[0], ops[1]);
            break;
        case '+':
            //res = ops[0] + ops[1];
            res = add_r(ops[0], ops[1]);
            break;
        case '-':
            //res = ops[0] - ops[1];
            res = add_r(ops[0], ops[1]);
            break;
        case '^':
            //res = pow(ops[0], ops[1]);
            res = pow_r(ops[0], ops[1].nume);
            break;
        default:
            break;
    }
    return res;
}
