#include "call_my_sqrt_func_from_julia_in_c.h"

void sqrt_func_from_julia(double *arr, int arr_len){

  /* required: setup the Julia context */
  jl_init();

  // Import MyModule
  jl_eval_string("Base.include(Main, \"MyModule.jl\")");
  jl_eval_string("using MyModule");
  jl_module_t *MyModule = (jl_module_t *)jl_eval_string("MyModule");

  // Get my_sqrt_func from MyModule
  jl_function_t *my_sqrt_func = jl_get_function(MyModule, "my_sqrt_func!");

  // Julia wrapper around C array
  jl_value_t *array_type =
      jl_apply_array_type((jl_value_t *)jl_float64_type, 1);
  jl_array_t *x = jl_ptr_to_array_1d(array_type, arr, arr_len, 0);

  /*Memory management using garbage collection (GC). To avoid GC from
  accidentaly freeing memory, we can mark them using a process called GC
  rooting*/
  JL_GC_PUSH1(&x);

  // To access data of x, we can use jl_array_data
  double *xData = (double *)jl_array_data(x);
  
  // Call the Julia function to compute sqrt on these array values
  jl_call1(my_sqrt_func, (jl_value_t *)x);

    for (int i = 0; i < jl_array_len(x); i++) {
    printf("Array vals after sqrt: %f\n", xData[i]);
  }

  JL_GC_POP();

  jl_atexit_hook(0);

}