///
/// @file      VariableLoader.cpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-05-01
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

#include "VariableLoader.hpp"
#include "AstScript/Variable.hpp"
#include "AstScript/Value.hpp"
#include "AstScript/ScriptAPI.hpp"
#include "AstUtil/Logger.hpp"
#include "AstUtil/Quantity.hpp"
#include "AstUtil/ParseFormat.hpp"

AST_NAMESPACE_BEGIN

errc_t aLoadParameter(const Value& value, Variable& var)
{
    // 检查类型
    {
        std::string type = value["Type"];
        if(type != "AgScriptingToolParameter")
        {
            aError("invalid type: '%s', expected 'AgScriptingToolParameter'", type.c_str());
            return -1;
        }
    }
    // 加载参数
    std::string paramType = value["ParamType"];
    // 加载数量参数
    if(paramType == "Quantity")
    {
        const Value& valueQuantity = value["ParamValue"];
        var.setExpr(valueQuantity.toQuantity());
    }
    else if(paramType == "Double")
    {
        var.setExpr(value["ParamValue"].toDouble());
    }
    else if(paramType == "Boolean")
    {
        auto& valueParam = const_cast<Value&>(value["ParamValue"]);
        // 如果是string类型，则需要解析为bool
        if(aValueIsString(&valueParam)){
            var.setExpr(aParseBool(valueParam.toString()));
        }
        else{
            var.setExpr(valueParam.toBool());
        }
    }
    else if(paramType == "Enumeration")
    {
        aWarning("enumeration type is not supported yet, use string instead");
        var.setExpr(value["ParamValue"].toString());
    }
    else if(paramType == "Integer")
    {
        var.setExpr(value["ParamValue"].toInt());
    }
    else if(paramType == "Date")
    {
        aWarning("date type is not supported yet, use string instead");
        var.setExpr(value["ParamValue"].toString());
    }
    else
    {
        Value* paramValue = const_cast<Value*>(&value["ParamValue"]);
        var.setExpr(paramValue);
        aWarning("unsupported parameter type: '%s'", paramType.c_str());
    }
    return 0;
}

errc_t aLoadCalcObject(const Value& value, Variable& var, Object* scope)
{
    return 0;
}


errc_t aLoadAttribute(const Value& value, Variable& var, Object* scope)
{
    return 0;
}



AST_NAMESPACE_END

