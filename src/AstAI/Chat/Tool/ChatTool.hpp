///
/// @file      AITool.hpp
/// @brief     AI工具定义
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
#include <string>
#include <vector>
#include <map>

AST_NAMESPACE_BEGIN

/*!
    @defgroup Chat
    @brief AI大语言模型相关接口
    @{
*/



class ChatTool {
public:
    ChatTool() = default;
    ~ChatTool() = default;
};


/// @brief AI工具参数
struct AIToolParameter {
    std::string name;          ///< 参数名
    std::string type;          ///< 参数类型
    std::string description;   ///< 参数描述
    bool required;             ///< 是否必填
};

#if 0

/// @brief AI工具定义
struct ChatTool {
    std::string name;                      ///< 工具名称
    std::string description;               ///< 工具描述
    std::map<std::string, AIToolParameter> parameters;  ///< 工具参数
};

#endif


/// @brief AI工具调用请求
struct AIToolCall {
    std::string id;            ///< 调用ID
    std::string type;          ///< 调用类型（function）
    std::string function_name; ///< 函数名称
    std::map<std::string, std::string> arguments; ///< 函数参数
};

/// @brief AI工具调用响应
struct AIToolResponse {
    std::string id;            ///< 调用ID
    std::string type;          ///< 响应类型（function）
    std::string function_name; ///< 函数名称
    std::string result;        ///< 执行结果
};

/*! @} */

AST_NAMESPACE_END