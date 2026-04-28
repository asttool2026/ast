///
/// @file      TargeterSequenceLoader.cpp
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

#include "TargeterSequenceLoader.hpp"
#include "AstUtil/StringView.hpp"
#include "AstScript/Value.hpp"
#include "ValXMLLoader.hpp"
#include "AstUtil/Logger.hpp"
#include "AstCore/TargeterSequence.hpp"
#include "SequenceLoader.hpp"
#include "MissionCommandLoader.hpp"

AST_NAMESPACE_BEGIN

errc_t aLoadTargeterSequence(const Value& dictRoot, TargeterSequence& sequence)
{
    std::string type = dictRoot["Type"];
    if(type != "TargeterSequence")
    {
        aError("invalid type, expect 'TargeterSequence'");
        return eErrorInvalidParam;
    }
    auto& dictSegmentList = dictRoot["SegmentList"];
    auto& items = dictSegmentList.items();
    std::vector<HMissionCommand> commands;
    commands.reserve(items.size());
    for(auto& item: items)
    {
        auto& name = item.first;
        const auto& dictSegment = *item.second;
        HMissionCommand command;
        errc_t rc = aLoadMissionCommand(dictSegment, command);
        if(!rc && command != nullptr)
        {
            commands.push_back(command);
        }else
        {
            aError("failed to load mission command '%s'", name.c_str());
        }
    }
    sequence.setCommands(commands);
    return eNoError;
}

errc_t aLoadTargeterSequence(StringView filepath, TargeterSequence& sequence)
{
    errc_t rc;
    SharedPtr<Value> value;
    rc = aLoadValue(filepath, value);
    if(rc)  return rc;
    if(!value) return eErrorNullPtr;
    return aLoadTargeterSequence(*value, sequence);
}

AST_NAMESPACE_END