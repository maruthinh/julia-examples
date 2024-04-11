#include <julia.h>
JULIA_DEFINE_FAST_TLS // only define this once, in an executable (not in a
                      // shared library) if you want fast code.

    int
    main(int argc, char *argv[]) {
  /* required: setup the Julia context */
  jl_init();

  /* run Julia commands */
  jl_eval_string("print(sqrt(2.0))");

  // getting result back to C code from Julia by unboxing
  jl_value_t *ret = jl_eval_string("sqrt(2.0)");

  if (jl_typeis(ret, jl_float64_type)) {
    double ret_unboxed = jl_unbox_float64(ret);
    printf("\nsqrt(2.0) in C: %e \n", ret_unboxed);
  } else {
    printf("ERROR: unexpected return type from sqrt(::Float64)\n");
  }

  // Passing C arguments to Julia function using jl_call
  jl_function_t *func = jl_get_function(jl_base_module, "sqrt");
  jl_value_t *argument = jl_box_float64(4.0);
  // jl_call1 to pass 1 argument, jl_call2 to pass two arguments and jl_call to
  // pass more arguments
  jl_value_t *ret1 = jl_call1(func, argument);

  // Another way of calling Julia function using @cfunction
  double (*sqrt_jl)(double) = jl_unbox_voidpointer(
      jl_eval_string("@cfunction(sqrt, Float64, (Float64,))"));
  double ret2 = sqrt_jl(2.0);

  /*Memory management using garbage collection (GC). To avoid GC from
  accidentaly freeing memory, we can mark them using a process called GC
  rooting*/
  jl_value_t *ret3 = jl_eval_string("sqrt(2.0)");
  JL_GC_PUSH1(&ret3);
  // Do something with ret2
  JL_GC_POP();

  // Working with arrays
  // Create an 1D array containing 10 elements of float64 type
  jl_value_t *array_type =
      jl_apply_array_type((jl_value_t *)jl_float64_type, 1);
  jl_array_t *x = jl_alloc_array_1d(array_type, 10);

  /*If you have already allocated an array, you can generate thin wrapper around
  it. The last argument is a boolean indicating whether Julia should take
  ownership of the data. If this argument is non-zero, the GC will call free on
  the data pointer when the array is no longer referenced.
  */
  double *existingArray = (double *)malloc(sizeof(double) * 10);
  jl_array_t *x1 = jl_ptr_to_array_1d(array_type, existingArray, 10, 0);

  // To access data of x, we can use jl_array_data
  double *xData = (double *)jl_array_data(x);

  for (size_t i = 0; i < jl_array_len(x); i++)
    xData[i] = i;

  for (size_t i = 0; i < jl_array_len(x); i++) {
    printf("Values of array x %f\n", xData[i]);
  }

  jl_function_t *func1 = jl_get_function(jl_base_module, "reverse!");
  jl_call1(func1, (jl_value_t *)x);

  printf("-------------After reversing array-------------\n");
  for (size_t i = 0; i < jl_array_len(x); i++) {
    printf("Values of array x %f\n", xData[i]);
  }

  // Accessing returned arrays
  jl_function_t *func2 = jl_get_function(jl_base_module, "reverse");
  jl_array_t *y = (jl_array_t *)jl_call1(func2, (jl_value_t *)x);

  // To access data of y, we can use jl_array_data
  double *yData = (double *)jl_array_data(y);
  printf("----------After reversing array which is accessed via y---------\n");
  for (size_t i = 0; i < jl_array_len(y); i++) {
    printf("Values of array y %f\n", yData[i]);
  }

  /*Multidimensional Arrays
    Julia's multidimensional arrays are stored in memory in column-major order
    like in Fortran
    Notice that while Julia arrays use 1-based indexing, the C API uses 0-based
    indexing (for example in calling jl_array_dim) in order to read as idiomatic
     C code.
  */

  jl_value_t *array_type1 =
      jl_apply_array_type((jl_value_t *)jl_float64_type, 2);
  jl_array_t *a = jl_alloc_array_2d(array_type1, 10, 5);

  // Get array pointer
  double *p = (double *)jl_array_data(a);

  // Get number of dimensions
  int ndims = jl_array_ndims(a);

  printf("Dimensions of multi array computed from Julia func %d \n", ndims);

  size_t size0 = jl_array_dim(a, 0);
  size_t size1 = jl_array_dim(a, 1);

  printf("Size of each dimensions of multi array computed from Julia func: %ld "
         "%ld\n",
         size0, size1);

  // Fill array with data
  for (size_t i = 0; i < size1; i++)
    for (size_t j = 0; j < size0; j++)
      p[j + size0 * i] = i + j;

  for (size_t i = 0; i < size0 * size1; i++)
    printf("Values p are %ld, %f \n", i, p[i]);

  /* strongly recommended: notify Julia that the
       program is about to terminate. this allows
       Julia time to cleanup pending write requests
       and run all finalizers
  */
  jl_atexit_hook(0);
  return 0;
}