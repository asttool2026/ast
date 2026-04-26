///
/// @file      ChatSession.hpp
/// @brief     聊天会话管理
/// @author    axel
/// @date      2026-04-23
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
#include "AstAI/OpenAI.hpp"
#include "AstAI/ChatTool.hpp"
#include "AstAI/ChatMessages.hpp"
#include <string>
#include <vector>
#include <functional>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup Chat
    @{
*/

/**
 * @brief 工具执行函数类型
 */
typedef std::function<std::string(const std::map<std::string, std::string>&)> ToolExecutor;

/// @brief 聊天会话
class AST_AI_API ChatSession {
public:
    /// @brief 构造函数
    /// @param api_key API密钥
    /// @param model 模型名称
    /// @param base_url API基础URL
    ChatSession();

    /// @brief 添加工具
    /// @param tool 工具定义
    /// @param executor 工具执行函数
    void addTool(const ChatTool& tool, ToolExecutor executor);

    /// @brief 发送消息
    /// @param message 消息内容
    /// @return 响应内容
    std::string sendMessage(StringView message);

    /// @brief 设置系统提示
    /// @param systemPrompt 系统提示
    void setSystemPrompt(StringView systemPrompt);

    /// @brief 获取消息历史
    /// @return 消息历史
    ChatMessages& messages(){return messages_;}

private:
    std::string makeChatCompletion();

    /// @brief 处理工具调用
    /// @param toolCalls 工具调用列表
    void handleToolCalls(const JsonValue& toolCalls);

    /// @brief 处理单个工具调用
    /// @param toolCall 单个工具调用
    static std::string handleToolCall(const JsonValue& toolCall);

    /// @brief 处理工具调用
    /// @param tool_calls 工具调用列表
    /// @return 工具响应列表
    std::vector<AIToolResponse> handleToolCalls(const std::vector<AIToolCall>& tool_calls);

    /// @brief 解析工具调用响应
    /// @param response 模型响应
    /// @param tool_calls 输出的工具调用列表
    /// @param content 输出的内容
    /// @return 是否包含工具调用
    bool parseToolCalls(const std::string& response, std::vector<AIToolCall>& tool_calls, std::string& content);

    /// @brief 获取当前使用的AI接口
    /// @note 目前还不支持指定或者切换client，只能使用对象内部默认的AI接口
    OpenAI& client();
private:
    using ChatToolMap = std::map<std::string, std::pair<ChatTool, ToolExecutor>>;
    OpenAI* client_;                        ///< 当前使用的AI接口
    OpenAI internalClient_;                 ///< 对象默认的AI接口
    ChatMessages messages_;                 ///< 消息历史
    ChatToolMap tools_;                     ///< 工具映射
};

/*! @} */

AST_NAMESPACE_END