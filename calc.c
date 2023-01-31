#include "calc.h"
int main(int argc, char *argv[]){
    
    if(argc > 1){
        arbin_t* expr;
        
        expr = read_expr(argv[1]);
        
        print_expr(expr);
        
        printf("Result : ");
        print_rational(calc_arb(expr));
        printf("\n");
        
        free_arb(expr);
    }else{
        printf("Utilisation: ./calc [expr]\n");
    }
}
