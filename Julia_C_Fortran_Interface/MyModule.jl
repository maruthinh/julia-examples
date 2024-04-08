module MyModule
export my_sqrt_func!

#compute sqrt of x in place
function my_sqrt_func!(x)
    x .= sqrt.(x)
end
end