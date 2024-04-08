#include <julia.h>
/* only define this once, in an executable (not in a shared library) if you
want fast code.*/
// JULIA_DEFINE_FAST_TLS

void sqrt_func_from_julia(double *, int);