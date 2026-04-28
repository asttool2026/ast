///
/// @file      WasmBind.cpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-04-28
/// @copyright 版权所有 (C) 2026-present, SpaceAST项目.
///
/// SpaceAST项目（https://github.com/space-ast/ast）
/// 本软件基于 Apache 2.0 开源许可证分发。
/// 您可在遵守许可证条款的前提下使用、修改和分发本软件。
/// 许可证全文请见：
/// 
///    http://www.apache.org/licenses/LICENSE-2.0
/// 
/// 重要须知：
/// 软件按"现有状态"提供，无任何明示或暗示的担保条件。
/// 除非法律要求或书面同意，作者与贡献者不承担任何责任。
/// 使用本软件所产生的风险，需由您自行承担。

#include "WasmBind.hpp"
#include "WasmRuntimeProtocol.hpp"
#include <emscripten/bind.h>


static int add(int a, int b) {
    return a + b;
}

static std::vector<std::string> testVector() 
{
    return {"ClassA", "ClassB"};
}

EMSCRIPTEN_BINDINGS(ObjectProtocol) {
    using namespace emscripten;
    AST_USING_NAMESPACE

    register_vector<std::string>("vector<string>");

    register_optional<std::string>();
    register_optional<double>();
    register_optional<int>();
    register_optional<bool>();
    register_optional<ObjectId>();

    function("add", &add);
    function("testVector", &testVector);


    class_<WasmRuntimeProtocol>("WasmRuntimeProtocol")
        .constructor()
        .class_function("setAttrString", &WasmRuntimeProtocol::setAttrString)
        .class_function("setAttrDouble", &WasmRuntimeProtocol::setAttrDouble)
        .class_function("setAttrInt", &WasmRuntimeProtocol::setAttrInt)
        .class_function("setAttrBool", &WasmRuntimeProtocol::setAttrBool)
        .class_function("setAttrObject", &WasmRuntimeProtocol::setAttrObject)
        
        .class_function("getAttrString", &WasmRuntimeProtocol::getAttrString)
        .class_function("getAttrDouble", &WasmRuntimeProtocol::getAttrDouble)
        .class_function("getAttrInt", &WasmRuntimeProtocol::getAttrInt)
        .class_function("getAttrBool", &WasmRuntimeProtocol::getAttrBool)
        .class_function("getAttrObject", &WasmRuntimeProtocol::getAttrObject)


        .class_function("newObject", &WasmRuntimeProtocol::newObject)  
        .class_function("removeObject", &WasmRuntimeProtocol::removeObject)
        .class_function("objectToJsonStr", &WasmRuntimeProtocol::objectToJsonStr)
        .class_function("classJsonSchemaStr", &WasmRuntimeProtocol::classJsonSchemaStr)

        .class_function("getAllClassNames", &WasmRuntimeProtocol::getAllClassNames)
        ;
    
}
