program test_calling_julia_from_fortran_via_c
  use, intrinsic :: iso_c_binding, only : c_double, c_int, c_loc  
  use fortran_c_interface 
  implicit none 

  real (c_double), pointer :: c(:)  
  integer (c_int) :: nx 
  integer :: i

  nx=10

  allocate(c(nx))

  do i=1,nx
    c(i)=i*i
  enddo

  print*, "Vals before calling sqrt func from Julia"

  do i=1,nx
    print*, c(i)
  enddo

  call sqrt_func_from_julia(c, nx)

  print*, "After sqrt which is called from Julia"

  do i=1,nx
    print*, c(i)
  enddo

  deallocate(c)

end program test_calling_julia_from_fortran_via_c 


