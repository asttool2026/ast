add_requires("python 3.x")
add_requires("swig >4.2")


target("AstPy")
    add_files("AstPy.i")
    add_rules("swig.cpp", {moduletype = "python"})
    add_packages("swig")
    add_packages("python")
    add_defines("SWIG_PYTHON_INTERPRETER_NO_DEBUG")
    add_deps("AstUtil", "AstCore", "AstSim")
    after_build(function(target)
        local autogendir = vformat(path.join("$(projectdir)", target:autogendir(), "rules", "swig"))
        local dest = target:targetdir()
        os.cp(path.join(autogendir, "**.py"), dest, {rootdir = autogendir})
    end)
    if is_plat("windows") and is_mode("debug") then
        set_suffixname("_d")
    end
    add_defines("SWIG", "AST_BUILD_LIB_PY")
    add_cxxflags("/bigobj")