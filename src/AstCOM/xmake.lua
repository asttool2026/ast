target("AstCOM")
    add_files("**.cpp")
    set_kind("shared")
    add_headerfiles("**.hpp", {prefixdir="AstCOM"})
    add_defines("AST_BUILD_LIB_COM")
    add_deps("AstUtil", "AstMath", "AstWeather", "AstScript")
    if is_plat("windows", "mingw") then
        add_syslinks("oleaut32", "ole32", "uuid")
    else
        set_enabled(false)
    end

