///
/// @file      SegmentLoader.cpp
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

#include "SegmentLoader.hpp"
#include "AstCore/Segment.hpp"
#include "AstCore/ScStateCalcAllHeaders.hpp"
#include "SpacecraftStateLoader.hpp"
#include "AstScript/Value.hpp"
#include "AstUtil/ObjectCalculation.hpp"
#include "AstUtil/ObjectLinker.hpp"
#include "AstUtil/RTTIAPI.hpp"
#include "AstScript/ExprAttribute.hpp"
#include "AstCore/Resolve.hpp"

AST_NAMESPACE_BEGIN


errc_t aLoadCalculation(const Value& value, ScStateCalcFrameRelated& calculation)
{
    std::string frameName = value["CoordSystem"];
    auto calcPtr = &calculation;
    auto resolveFunc = [calcPtr, frameName]() -> errc_t {
        if(auto frame = aResolveFrame(frameName))
        {
            calcPtr->setFrame(frame);
            return eNoError;
        }
        aError("frame '%s' not found", frameName.c_str());
        return eErrorNullPtr;
    };
    calculation.addDelayedLink(resolveFunc);
    return 0;
}

errc_t aLoadCalculation(const Value& value, ScStateCalcBodyRelated& calculation)
{
    std::string bodyName = value["CentralBody"];
    auto calcPtr = &calculation;
    auto resolveFunc = [calcPtr, bodyName]() -> errc_t {
        if(auto body = aResolveBody(bodyName))
        {
            calcPtr->setBody(body);
            return eNoError;
        }
        aError("body '%s' not found", bodyName.c_str());
        return eErrorNullPtr;
    };
    calculation.addDelayedLink(resolveFunc);
    return 0;
}

errc_t aLoadCalculation(const Value& value, ScStateCalcPointRelated& calculation)
{
    std::string pointName = value["ReferencePoint"];
    auto calcPtr = &calculation;
    auto resolveFunc = [calcPtr, pointName]() -> errc_t {
        if(auto point = aResolvePoint(pointName))
        {
            calcPtr->setPoint(point);
            return eNoError;
        }
        aError("point '%s' not found", pointName.c_str());
        return eErrorNullPtr;
    };
    calculation.addDelayedLink(resolveFunc);
    return 0;
}

errc_t aLoadResult(const Value& value, SharedPtr<ObjectCalculation>& result, Object* scope)
{
    std::string type = value["Type"];
    if(type == "AsStateCalcEccentricity")
    {
        auto calculation = new ScStateCalcEccentricity();
        result = calculation;
        aLoadCalculation(value, *calculation);
    }
    else if(type == "AsStateCalcVx")
    {
        auto calculation = new ScStateCalcVx();
        result = calculation;
        aLoadCalculation(value, *calculation);
    }
    else if(type == "AsStateCalcVy")
    {
        auto calculation = new ScStateCalcVy();
        result = calculation;
        aLoadCalculation(value, *calculation);
    }
    else if(type == "AsStateCalcVz")
    {
        auto calculation = new ScStateCalcVz();
        result = calculation;
        aLoadCalculation(value, *calculation);
    }
    else if(type == "AsStateCalcRMag")
    {
        auto calculation = new ScStateCalcRMag();
        result = calculation;
        aLoadCalculation(value, *calculation);
    }
    else
    {
        result.reset();
        aError("unsupported result type: '%s'", type.c_str());
        return eErrorInvalidParam;
    }
    if(result)
        result->setParentScope(scope);
    return eNoError;
}

errc_t aLoadResults(const Value& dict, Object* scope)
{
    for(auto& item: dict.items())
    {
        std::string name = item.first;
        Value& value = *item.second;
        SharedPtr<ObjectCalculation> calculation;
        aLoadResult(value, calculation, scope);
        if(calculation)
            calculation->setName(name);
    }
    return 0;
}

errc_t aLoadShooterControl(const Value& value, SharedPtr<ExprAttribute>& control, Object* scope)
{
    Attribute attr(scope, nullptr);
    std::string attrPath = value.toString();
    aWarning("todo: resolve attribute %s", attrPath.c_str());
    control = new ExprAttribute(attr);
    return 0;   
}

errc_t aLoadShooterControls(const Value& dict, Object* scope)
{
    for(auto& item: dict.items())
    {
        std::string name = item.first;
        Value& value = *item.second;
        SharedPtr<ExprAttribute> control;
        aLoadShooterControl(value, control, scope);
        if(control)
            control->setName(name);
    }
    return 0;
}

errc_t aLoadSegment(const Value& dict, Segment& segment)
{
    if(auto finalStatePtr = segment.getFinalState())
    {
        aLoadSpacecraftState(dict["FinalState"], *finalStatePtr);
    }
    // 加载 Results
    {
        auto& dictResults = dict["Results"];
        aLoadResults(dictResults, &segment);
    }
    // 加载 ShooterControls
    {
        auto& dictShooterControls = dict["ShooterControls"];
        aLoadShooterControls(dictShooterControls, &segment);
    }
    return eNoError;
}


AST_NAMESPACE_END
