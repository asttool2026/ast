///
/// @file      OpenAI.hpp
/// @brief     OpenAI规范API调用接口
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
#include "AITool.hpp"
#include <string>
#include <vector>
#include <map>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup Chat
    @{
*/

/**
 * @brief 消息角色
 */
enum class MessageRole {
    USER,      ///< 用户
    ASSISTANT, ///< 助手
    SYSTEM,    ///< 系统
    TOOL       ///< 工具
};

/**
 * @brief 聊天消息
 */
struct ChatMessage {
    MessageRole role;          ///< 消息角色
    std::string content;       ///< 消息内容
    std::string tool_call_id;  ///< 工具调用ID（当角色为TOOL时使用）
};

/**
 * @brief OpenAI客户端
 */
class OpenAIClient {
public:
    /**
     * @brief 构造函数
     * @param api_key API密钥
     * @param base_url API基础URL
     */
    OpenAIClient(const std::string& api_key, const std::string& base_url = "https://api.openai.com/v1");

    /**
     * @brief 发送聊天请求
     * @param model 模型名称
     * @param messages 消息列表
     * @param tools 工具列表
     * @param temperature 温度参数
     * @return 响应内容
     */
    std::string chat(const std::string& model, const std::vector<ChatMessage>& messages, 
                    const std::vector<AITool>& tools = {}, float temperature = 0.7f);

private:
    std::string api_key_;  ///< API密钥
    std::string base_url_; ///< API基础URL
};

/*! @} */

AST_NAMESPACE_END