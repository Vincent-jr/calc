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

rational_t read_r(const char *str, char **end, const char *sep){ // sep les separateurs entre partie entière et parties décimale, sous form de string
    rational_t r_out;
    long int integ, decimals;
    
    integ = strtol(str, end, 10);
    if(**end != 0 && strchr(sep, **end)){ // si partie entière est suivis d'un séparateur valide (partie entière peut etre la chaine vide)
        char* decimals_start = *end + 1;
        decimals = strtol(decimals_start, end, 10);

        int nb_decimals = *end - decimals_start;

        if(nb_decimals){
            int rank_shift = pow(10, nb_decimals);
            
            r_out.nume = integ * rank_shift + decimals;
            r_out.deno = rank_shift;
            
            simp(&r_out);
        }else{
            r_out.nume = integ;
            r_out.deno = 1;
        }
    }else{
        r_out.nume = integ;
        r_out.deno = 1;
    }
    
    return r_out;
}

void print_rational(rational_t r){
    if(r.deno == 1){
        printf("%ld", r.nume);               
    }else{
        printf("%ld/%ld", r.nume, r.deno);
    }
}
