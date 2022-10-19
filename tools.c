#include "tools.h"

long int gcd(long int a, long int b){
    if(b == 0) return a;
    
    int rest;
    while((rest=a%b) != 0){
        a = b;
        b = rest;
    }
    return b;
}
