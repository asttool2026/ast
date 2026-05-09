target("AstCOM")
    add_files("**.cpp")
    add_files("**.idl")
    add_files("**.rc")
    set_kind("shared")
    add_headerfiles("**.hpp", {prefixdir="AstCOM"})
    add_defines("AST_BUILD_LIB_COM", "_USRDLL")
    add_deps("AstUtil", "AstMath", "AstWeather", "AstScript")
    if is_plat("windows", "mingw") then
        add_syslinks("oleaut32", "ole32", "uuid")
    else
        set_enabled(false)
    end