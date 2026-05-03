///
/// @file      Sequence.cpp
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

#include "Sequence.hpp"
#include "AstUtil/Logger.hpp"


AST_NAMESPACE_BEGIN

errc_t Sequence::execute()
{
    errc_t rc = 0;
    // 设置第一个轨道段的输入状态
    auto intputState = getInputState();
    for(auto& command : commands_)
    {
        if(auto segment = aobject_cast<Segment*>(command.get()))
        {
            segment->setInputState(intputState);
            break;
        }
    }
    // 执行脚本
    if(auto scriptingTool = this->scriptingTool_.get())
    {
        errc_t rc =  scriptingTool->execute();
        if(rc != eNoError)
            aError("failed to execute scripting tool %s", scriptingTool->getName().c_str());
    }
    // 执行任务序列
    for(int repeat=0;repeat<repeatCount_;repeat++)
    {
        for (auto& command : commands_)
        {
            errc_t err = command->execute();
            if(err != eNoError)
            {
                aError("failed to execute command: %s<%s>", command->getName().c_str(), command->typeName().c_str());
                rc = err;
            }
        }
    }
    
    // 设置输出状态为输出输出状态为输出状态
    for(auto iter = commands_.rbegin();iter != commands_.rend();iter++)
    {
        if(auto segment = aobject_cast<Segment*>(iter->get()))
        {
            // @todo: 设置输出状态
            break;
        }
    }
    return rc;
}


void Sequence::linkCommands()
{
    SpacecraftState* inputState = getInputState();
    for(int i=0;i<commands_.size();i++)
    {
        auto command = commands_[i].get();
        if(auto segment = aobject_cast<Segment*>(command))
        {
            segment->setInputState(inputState);
            inputState = segment->getOutputState();
        }
    }
}


void Sequence::setCommands(const std::vector<HMissionCommand>& commands)
{
    commands_ = commands;
    linkCommands();
}

void Sequence::setCommands(std::vector<HMissionCommand>&& commands)
{
    commands_ = std::move(commands);
    linkCommands();
}

Segment* Sequence::getSegmentByPath(StringView path)
{
    return aobject_cast<Segment*>(getCommandByPath(path));
}

MissionCommand* Sequence::getCommandByPath(StringView path)
{
    auto pos = path.find('.');
    if(pos == String::npos)
        return getCommandByName(path);
    else
    {
        auto sequence = aobject_cast<Sequence*>(getCommandByName(path.substr(0, pos)));
        if(sequence)
            return sequence->getCommandByPath(path.substr(pos + 1));
        else
            return nullptr;
    }
}

Segment* Sequence::getSegmentByName(StringView name)
{
    return aobject_cast<Segment*>(getCommandByName(name));
}

MissionCommand* Sequence::getCommandByName(StringView name)
{
    for (auto& commandPtr : commands_)
    {
        if(auto command = commandPtr.get())
        {
            if(name == command->getName())
                return command;
        }
    }
    return nullptr;
}

AST_NAMESPACE_END

