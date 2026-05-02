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
#include "AstUtil/RTTIAPI.hpp"

AST_NAMESPACE_BEGIN

errc_t aLoadResult(const Value& value, SharedPtr<ObjectCalculation>& result, Object* scope)
{
    std::string type = value["Type"];
    if(type == "AsStateCalcEccentricity")
    {
        result = new ScStateCalcEccentricity();
    }
    else if(type == "AsStateCalcVx")
    {
        result = new ScStateCalcVx();
    }
    else if(type == "AsStateCalcVy")
    {
        result = new ScStateCalcVy();
    }
    else if(type == "AsStateCalcVz")
    {
        result = new ScStateCalcVz();
    }
    else
    {
        aError("unsupported result type: '%s'", type.c_str());
        return eErrorInvalidParam;
    }

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
    return eNoError;
}


AST_NAMESPACE_END
