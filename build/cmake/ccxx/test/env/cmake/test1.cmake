
function(print_list arg)
    foreach(v ${arg})
        message(${v})
    endforeach()
endfunction()

list(APPEND ls1 aa1)
list(APPEND ls1 aa2)
list(APPEND ls1 aa3)
list(APPEND ls1 [=["aa3 bb1
bb2 \"cc\""]=])

print_list(${ls1})
