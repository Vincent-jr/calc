#include "expr.h"


const char operators[] = "+-/*^";

const char prio0[] = "+-";
const char prio1[] = "/*";
const char prio2[] = "^";

const char separators_open[] = "(";
const char separators_close[] = ")";

const char separator_deci[] = ".,";


arbin_t* new_node(token_t val){
    arbin_t* n = malloc(sizeof(arbin_t));
    n->val = val;
    n->left = NULL;
    n->right = NULL;
    
    return n;
}

void free_arb(arbin_t *arb){
    if(arb->left != NULL){
        free_arb(arb->left);
    }
    if(arb->right != NULL){
        free_arb(arb->right);
    }
    
    free(arb);
}

inline arbin_t* new_node_i(int i){
    return new_node((token_t){OPERAND_NUM, 0, (rational_t){i, 1}});
}

inline arbin_t* new_node_op(char c){
    return new_node((token_t){OPERATOR, c, (rational_t){0, 0}});
}

inline arbin_t* new_node_var(char c){
    return new_node((token_t){OPERAND_C, c, (rational_t){0, 0}});
}

inline arbin_t* new_node_r(rational_t r){
    return new_node((token_t){OPERAND_NUM, 0, r});
}


bool is_leaf(arbin_t *a){
    return a->right == NULL && a->left == NULL;
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


bool operator_smaller(char op1, char op2){ // retourne true si op1 est strictement + petit que op2
    if(strchr(prio2, op1)){
        return false;
    }else if(strchr(prio1, op1)){
        if(strchr(prio2, op2)){
            return true;
        }else{
            return false;
        }
    }else{
        if(strchr(prio0, op2)){
            return false;
        }else{
            return true;
        }
    }
}


int add_operator(arbin_t **op_stack, int *top, arbin_t* new_node, arbin_t* pending){
    
    arbin_t* active_node = op_stack[*top];
    
    while(!operator_greater(new_node->val.c, active_node->val.c)){
        printf("not greater\n");
        if(active_node == op_stack[0]){ // root
            printf("at root %d\n", *top);
            new_node->left = op_stack[0];
            op_stack[0]->right = pending;
            op_stack[0] = new_node;
            break;
        }else{
            active_node->right = pending;
            pending = active_node;
            
            (*top)--; // attention a ne pas passer en négatif
            active_node = op_stack[*top];
        }
    }
    
    if(op_stack[0] != new_node){
        new_node->left = pending;
        (*top)++;
        op_stack[*top] = new_node;
    }
    
    return 1;
}


arbin_t* read_expr(char *expr_src){
    printf("################## read_expr ####################\n");
    
    arbin_t* root;
    
    bool err = false;
    
    token_types token_type;
    token_types last_token_type;
    
    rational_t elem_r;
    char *elem_end;
    arbin_t* pending;
    arbin_t* new_node;
    
    
    arbin_t* current_ops_nodes[100]; //pile des opérateurs pouvant etre affectés par la prio des opérations
    int active_ind = -1; // indique le dessus de la pile
    arbin_t* active_node;
    
    
    
    char first_char = expr_src[0];
    if(first_char == '-' || first_char == '+'){
        root = new_node_op(first_char);
        expr_src++;
    }else{
        root = new_node_op('+');
    }
    
    root->left = new_node_i(0);
    
    active_ind++;
    current_ops_nodes[active_ind] = root;
    pending = root;
    
    
    last_token_type = OPERATOR;
    
    
    char c;
    print_expr(root);
    while((c = expr_src[0]) != '\0' && !err){
        printf("--------%d--------\n\n", c);
        
        elem_r = read_r(expr_src, &elem_end, separator_deci);
        
        if(strchr(operators, c)){
            
            if(last_token_type == OPERATOR){ // deux opérateurs de suite
                err = true;
                printf("db operateur\n");
                break;
            }
            
            new_node = new_node_op(c);
            
            if(last_token_type == OPERAND_NUM || last_token_type == SUB_EXPR || last_token_type == OPERAND_C){
                add_operator(current_ops_nodes, &active_ind, new_node, pending);
            }
            
            token_type = OPERATOR;
            expr_src++;
            
        }else if(strchr(separators_open, c)){
            token_type = SUB_EXPR;
            
            
            int sub_expr_len = (strrchr(expr_src+1, ')') - (expr_src+1)); // +1 car expr_src est une parenthèse
            char* sub_expr_src = malloc(sizeof(char)*(sub_expr_len+1));// +1 pr le \0

            strncpy(sub_expr_src, expr_src+1, sub_expr_len);
            sub_expr_src[sub_expr_len] = '\0';
            
            pending = read_expr(sub_expr_src);
            free(sub_expr_src);

            expr_src += 2 + sub_expr_len;
            
        }else if(strchr(separators_close, c)){
            last_token_type = SEPARATOR_COLSE;
            
        }else if(expr_src != elem_end){
            token_type = OPERAND_NUM;
            
            expr_src = elem_end;

            printf("num : ");
            print_rational(elem_r);
            printf("\n");
            
            if(last_token_type == OPERAND_NUM){
                printf("db num\n");
                err = true;
            }
            
            pending = new_node_r(elem_r);
        }else if(isspace(c)){
            expr_src++;
        }else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){ // inconnue
            token_type = OPERAND_C;

            printf("inconnu : \n");
            
            if(last_token_type == OPERAND_NUM || last_token_type == OPERAND_C){
                printf("add *\n");
                new_node = new_node_op('*');
                add_operator(current_ops_nodes, &active_ind, new_node, pending);
            }
            
            pending = new_node_var(c);
            expr_src++;
            
        }else{
            printf("unexpected\n");
            err = true;
        }
        
        last_token_type = token_type;
        printf("last type %d\n\n", last_token_type);
    }
    
    
    
    if(err){
        printf("erreur\n");
        return NULL;
    }
    
    if(pending != NULL){ // ou operand c
        printf("still pending %d\n", active_ind);
        active_node = current_ops_nodes[active_ind];
        do{
            active_node->right = pending;
            pending = active_node;
            active_ind--;
            active_node = current_ops_nodes[active_ind];
        }while(active_ind >= 0);
    }
    

    printf("################ fin read #################\n");
    
    return current_ops_nodes[0]; // si pile vide erreure
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


int depth(arbin_t *a){
    if(a == NULL){
        return 0;
    }else {
        int dleft = depth(a->left);
        int dright = depth(a->right);
        return dright > dleft ? 1+dright : 1+dleft;
    }
}


void print_expr_(arbin_t *a, int i){
    if(a != NULL){
        printf("%d (", i);
        print_token(a->val);
        int dleft = depth(a->left);
        printf(") : ");
        if(a->left != NULL){
            printf("%d", i+1);
        }else{
            printf("%d", -1);
        }
        printf(", ");
        if(a->right != NULL){
            printf("%d", i+1+dleft*dleft);
        }else{
            printf("%d", -1);
        }
        printf("\n");
        print_expr_(a->left, i+1);
        print_expr_(a->right, i+1+dleft*dleft);
    }
}
void print_expr(arbin_t *a){
    int i = 0;
    print_expr_(a, i);
}



rational_t calc_arb(arbin_t *arb){ // détecter un arbre invalide
    rational_t res;
    rational_t ops[2]; // potentiellement faire un tab de pointeur pr distinguer initialisé de pas initialisé dans le cas d'un operateur unaire
    
    token_types type = arb->val.type;
    switch(type){
        case OPERAND_NUM:
            return arb->val.r;
            break;
        case OPERATOR:
            ops[0] = calc_arb(arb->left);
            ops[1] = calc_arb(arb->right);
            
            if(ops[0].deno == 0 || ops[1].deno == 0){
                return (rational_t){0, 0};
            }
            
            switch(arb->val.c){
                case '*':
                    res = mult_r(ops[0], ops[1]);
                    break;
                case '/':
                    res = div_r(ops[0], ops[1]);
                    break;
                case '+':
                    res = add_r(ops[0], ops[1]);
                    break;
                case '-':
                    res = sub_r(ops[0], ops[1]);
                    break;
                case '^':
                    res = pow_r(ops[0], ops[1].nume);
                    break;
                default:
                    break;
            }
            break;
        default:
            return (rational_t){0, 0};
            break;
    }

    return res;
}
