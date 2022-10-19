#include "calc.h"
int main(int argc, char *argv[]){
    
    if(argc > 1){
        int nb_sommet_max = strlen(argv[1])*3;
        expression_t expr;
        init_expression(&expr, nb_sommet_max);
        
        int root = read_expr(&expr, argv[1]);
        
        print_expr(&expr);
        
        printf("Result : ");
        print_rational(calc_graph(&expr, root));
        printf("\n");
        
        free_expression(&expr, nb_sommet_max);
        
        printf("\n");
    }else{
        printf("Utilisation: ./calc [expr]\n");
    }
}


// passer la déclaration de certaines variables hors des conditions
// gérer les nombres à virgule
// changer le nom token en element
