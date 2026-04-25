///
/// @file      ChatMessage.hpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-04-25
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

#pragma once

#include "AstGlobal.h"
#include <string>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup 
    @{
*/



/// @brief 消息角色
enum class EChatRole {
    eUser,      ///< 用户
    eAssistant, ///< 助手
    eSystem,    ///< 系统
    eTool       ///< 工具
};


/// @brief 聊天消息
class ChatMessage 
{
public:
    /// @brief 构造函数
    ChatMessage() = default;
    /// @brief 析构函数
    ~ChatMessage() = default;

    /// @brief 设置消息角色
    void setRole(EChatRole role) { role_ = role; }
    /// @brief 获取消息角色
    EChatRole role() const { return role_; }

    /// @brief 设置角色名称
    void setName(const std::string& name) { name_ = name; }
    /// @brief 获取角色名称
    std::string name() const { return name_; }

    /// @brief 设置消息内容
    void setContent(const std::string& content) { content_ = content; }
    /// @brief 获取消息内容
    std::string content() const { return content_; }
    
    /// @brief 设置工具调用ID
    void setToolCallId(const std::string& toolCallId) { toolCallId_ = toolCallId; }
    
    /// @brief 获取工具调用ID
    std::string toolCallId() const { return toolCallId_; }
    
private:
    EChatRole        role_{EChatRole::eUser};           ///< 消息角色
    std::string      name_;                             ///< 角色名称
    std::string      content_;                          ///< 消息内容
    std::string      toolCallId_;                       ///< 工具调用ID（当角色为TOOL时使用）
};


/*! @} */

AST_NAMESPACE_END
