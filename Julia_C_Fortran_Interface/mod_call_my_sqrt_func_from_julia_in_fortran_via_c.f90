module fortran_c_interface
use, intrinsic :: iso_c_binding, only : c_int, c_double, c_ptr
implicit none 
  interface 
  subroutine sqrt_func_from_julia(arr, arr_len) bind(C, name="sqrt_func_from_julia")
  import :: c_int, c_double, c_ptr 
  implicit none 
  integer(c_int), intent(in), value :: arr_len 
  real(c_double), intent(inout) :: arr(*)
  end subroutine sqrt_func_from_julia
  end interface
end module fortran_c_interface