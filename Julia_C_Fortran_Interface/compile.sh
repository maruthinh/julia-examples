export JULIA_DIR="${HOME}/softwares/julia-1.10.0-alpha1"
gcc -c -fPIC -I. -I$JULIA_DIR/include/julia call_my_sqrt_func_from_julia_in_c.c
gfortran mod_call_my_sqrt_func_from_julia_in_fortran_via_c.f90 -c 
gfortran -c main.f90 
gfortran main.o mod_call_my_sqrt_func_from_julia_in_fortran_via_c.o call_my_sqrt_func_from_julia_in_c.o -o test.out -L$JULIA_DIR/lib -Wl,-rpath,$JULIA_DIR/lib -ljulia
./test.out
