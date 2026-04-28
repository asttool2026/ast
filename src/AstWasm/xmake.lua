target("AstWasm")
    set_kind("binary")
    if not is_plat("wasm") then
        set_enabled(false)
        return
    end
    
    add_files("**.cpp")
    add_includedirs(".")
    
    add_deps("AstUtil", "AstCore", "AstSim", "AstMath", "AstAI")
    add_ldflags("--bind")
    -- 设置为c++17标准，因为embind需要c++17标准的特性
    set_languages("c++17")
