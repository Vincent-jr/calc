#include "numbers.h"
 
void simp(rational_t *r){
    long int fact = gcd(r->nume, r->deno);
    r->nume /= fact;
    r->deno /= fact;
}

rational_t add_r(rational_t r1, rational_t r2){
    rational_t res;
    res.nume = r1.nume*r2.deno + r2.nume*r1.deno;
    res.deno = r1.deno*r2.deno;
    simp(&res);
    return res;
}

rational_t sub_r(rational_t r1, rational_t r2){
    rational_t res;
    res.nume = r1.nume*r2.deno - r2.nume*r1.deno;
    res.deno = r1.deno*r2.deno;
    simp(&res);
    return res;
}

rational_t mult_r(rational_t r1, rational_t r2){
    rational_t res;
    res.nume = r1.nume*r2.nume;
    res.deno = r1.deno*r2.deno;
    simp(&res);
    return res;
}

rational_t inv_r(rational_t r){
    rational_t inv;
    inv.nume = r.deno;
    inv.deno = r.nume;
    return inv;
}

rational_t div_r(rational_t r1, rational_t r2){
    return mult_r(r1, inv_r(r2));
}

rational_t pow_r(rational_t n, long int e){ // faire le meilleur algo qui fait carré par carré
    rational_t res = n;
    for(int i=1; i < e; i++){
        res.nume *= n.nume;
        res.deno *= n.deno;
    }
    return res;
}
