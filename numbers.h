#include "tools.h"

struct rational{
    long int nume;
    long int deno;
};
typedef struct rational rational_t;


void simp(rational_t *r);
rational_t add_r(rational_t r1, rational_t r2);
rational_t sub_r(rational_t r1, rational_t r2);
rational_t mult_r(rational_t r1, rational_t r2);
rational_t inv_r(rational_t r);
rational_t div_r(rational_t r1, rational_t r2);
rational_t pow_r(rational_t n, long int e);
