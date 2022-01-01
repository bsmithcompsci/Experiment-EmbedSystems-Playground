add_library( Scripting-Julia STATIC
    src/Julia/scripting_julia.cpp
    include/Embed-Pipeline/Julia/scripting_julia.h
    scripts/example.jl
)
target_link_libraries( Scripting-Julia PRIVATE JuliaSDK Utils )
add_linktoexe( Scripting-Julia )
