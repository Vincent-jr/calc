#include "calc.h"


int main(int argc, char *argv[]){
    char *end;
    print_rational(read_r(argv[1], &end, "."));
    printf("\n");
    printf("end : %c\n", end[0]);
    return 1;
}
