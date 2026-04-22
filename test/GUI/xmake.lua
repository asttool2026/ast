add_rules("mode.debug", "mode.release")

includes("../../xmake.lua")

-- 测试UiGravityForce
target("testUiGravityForce")
    set_kind("binary")
    add_files("testUiGravityForce.cpp")
    add_deps("AstCore", "AstGUI")
    add_packages("qt5")

-- 测试UiDragForce
target("testUiDragForce")
    set_kind("binary")
    add_files("testUiDragForce.cpp")
    add_deps("AstCore", "AstGUI")
    add_packages("qt5")

-- 测试UiSolarRadiationPressure
target("testUiSolarRadiationPressure")
    set_kind("binary")
    add_files("testUiSolarRadiationPressure.cpp")
    add_deps("AstCore", "AstGUI")
    add_packages("qt5")

-- 测试UiThirdBodyForce
target("testUiThirdBodyForce")
    set_kind("binary")
    add_files("testUiThirdBodyForce.cpp")
    add_deps("AstCore", "AstGUI")
    add_packages("qt5")