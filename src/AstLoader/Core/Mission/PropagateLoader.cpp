///
/// @file      Propagate.cpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-04-15
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

#include "AstCore/Propagate.hpp"
#include "ValXMLLoader.hpp"
#include "ResultLoader.hpp"
#include "AstScript/Value.hpp"
#include "AstLoader/SegmentLoader.hpp"
#include "AstCore/DetectorAllHeaders.hpp"
#include "AstUtil/RTTIAPI.hpp"



AST_NAMESPACE_BEGIN

errc_t aLoadEventDetector(const Value& value, DetectorBodyRelated& detector)
{
    std::string bodyName = value["CalcObjectAttributes"]["CentralBody"];
    detector.setBodyByName(bodyName);
    return 0;
}

errc_t aLoadEventDetector(const Value& value, DetectorPointRelated& detector)
{
    std::string pointName = value["CalcObjectAttributes"]["ReferencePoint"];
    detector.setPointByName(pointName);
    return 0;
}

errc_t aLoadEventDetector(const Value& value, DetectorUserSelect& detector)
{
    auto& calcObject = value["CalcObject"];
    if(!calcObject.isNull())
    {
        Object* scope = &detector;
        SharedPtr<ObjectCalculation> result;
        aLoadResult(calcObject, result, scope);
        detector.setCalculation(aobject_cast<ScStateCalculation*>(result));
    }
    return 0;
}

errc_t aLoadStoppingCondition(const Value& value, SharedPtr<EventDetector>& eventDetector, Propagate& propagate)
{
    std::string type = value["Type"];
    Object* scope = &propagate;
    if(type == "Duration")
    {
        eventDetector = aNewObject<DetectorDuration>(scope);
    }
    else if(type == "Periapsis")
    {
        auto detectorPeriapsis = aNewObject<DetectorPeriapsis>(scope);
        aLoadEventDetector(value, *detectorPeriapsis);
        eventDetector = detectorPeriapsis;
    }
    else if(type == "Apoapsis")
    {
        auto detectorApoapsis = aNewObject<DetectorApoapsis>(scope);
        aLoadEventDetector(value, *detectorApoapsis);
        eventDetector = detectorApoapsis;
    }
    else if(type == "R Magnitude")
    {
        auto detectorRMagnitude = aNewObject<DetectorRMagnitude>(scope);
        aLoadEventDetector(value, *detectorRMagnitude);
        eventDetector = detectorRMagnitude;
    }
    else if(type == "Lighting")
    {
        auto detectorLighting = aNewObject<DetectorLighting>(scope);
        // aLoadEventDetector(value, *detectorLighting);
        eventDetector = detectorLighting;
    }
    else if(type == "UserSelect")
    {
        auto detectorUserSelect = aNewObject<DetectorUserSelect>(scope);
        aLoadEventDetector(value, *detectorUserSelect);
        eventDetector = detectorUserSelect;
    }
    else
    {
        aError("unsupported stopping condition type '%s'", type.c_str());
        return eErrorInvalidParam;
    }
    if(eventDetector)
    {
        auto& tolerance = value["Tolerance"];
        if(!tolerance.isNull())
            eventDetector->setThreshold(tolerance);
        
        // @fixme TripValue 有可能是日期类型，需要特殊处理
        auto& tripValue = value["TripValue"];
        if(!tripValue.isNull())
            eventDetector->setGoal(tripValue);

        auto& repeatCount = value["RepeatCount"];
        if(!repeatCount.isNull())
            eventDetector->setRepeatCount(repeatCount);
    }
    return 0;
}

errc_t aLoadStoppingConditions(const Value& dict, Propagate& propagate)
{
    errc_t rc;
    std::vector<SharedPtr<EventDetector>> eventDetectors;
    for(auto& item: dict.items())
    {
        std::string name = item.first;
        Value& value = *item.second;
        SharedPtr<EventDetector> eventDetector;
        rc = aLoadStoppingCondition(value, eventDetector, propagate);
        if(rc || !eventDetector)
        {
            std::string type = value["Type"];
            aWarning("failed to load stopping condition '%s' with type '%s'", name.c_str(), type.c_str());
        }
        else
            eventDetectors.push_back(eventDetector);
    }
    propagate.setEventDetectors(eventDetectors);
    return eNoError;
}

errc_t aLoadPropagate(const Value& dictRoot, Propagate& propagate)
{
    errc_t rc;
    const std::string type = dictRoot["Type"];
    if(type != "Propagate")
    {
        aError("invalid type, expect 'Propagate'");
        return eErrorInvalidParam;
    }
    // 加载公共属性
    rc = aLoadSegment(dictRoot, propagate);
    
    // 加载停止条件
    rc = aLoadStoppingConditions(dictRoot["StoppingConditions"], propagate);

    return eNoError;
}

errc_t aLoadPropagate(StringView filepath, Propagate& propagate)
{
    errc_t rc;
    SharedPtr<Value> value;
    rc = aLoadValue(filepath, value);
    if(rc)  return rc;
    if(!value) return eErrorNullPtr;
    return aLoadPropagate(*value, propagate);
}


AST_NAMESPACE_END
