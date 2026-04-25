///
/// @file      OpenAI.cpp
/// @brief     OpenAI规范API调用接口实现
/// @details   实现OpenAI规范的API调用接口，包括聊天、工具调用等。
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


#include "OpenAI.hpp"
#include "AstUtil/Network.hpp"
#include <sstream>
#include <iostream>

AST_NAMESPACE_BEGIN


OpenAI::OpenAI(const std::string& api_key, const std::string& base_url) 
    : api_key_(api_key), base_url_(base_url) {
}


std::string OpenAI::chat(const std::string& model, const std::vector<ChatMessage>& messages, 
                              const std::vector<AITool>& tools, float temperature) {
    std::string response;
    std::stringstream ss;
    
    // 构建请求体
    ss << "{";
    ss << "\"model\": \"" << model << "\",";
    ss << "\"messages\": [";
    
    for (size_t i = 0; i < messages.size(); ++i) {
        const auto& msg = messages[i];
        ss << "{";
        
        // 角色转换
        std::string role_str;
        switch (msg.role_) {
            case EChatMessageRole::eUser: role_str = "user";
                break;
            case EChatMessageRole::eAssistant: role_str = "assistant";
                break;
            case EChatMessageRole::eSystem: role_str = "system";
                break;
            case EChatMessageRole::eTool: role_str = "tool";
                break;
        }
        
        ss << "\"role\": \"" << role_str << "\",";
        ss << "\"content\": \"" << msg.content_ << "\"";
        
        if (msg.role_ == EChatMessageRole::eTool && !msg.toolCallId_.empty()) {
            ss << ",\"tool_call_id\": \"" << msg.toolCallId_ << "\"";
        }
        
        ss << "}";
        if (i < messages.size() - 1) ss << ",";
    }
    ss << "]";
    
    // 添加工具
    if (!tools.empty()) {
        ss << ",\"tools\": [";
        for (size_t i = 0; i < tools.size(); ++i) {
            const auto& tool = tools[i];
            ss << "{";
            ss << "\"type\": \"function\",";
            ss << "\"function\": {";
            ss << "\"name\": \"" << tool.name << "\",";
            ss << "\"description\": \"" << tool.description << "\",";
            ss << "\"parameters\": {";
            ss << "\"type\": \"object\",";
            ss << "\"properties\": {";
            
            size_t param_count = 0;
            for (const auto& param : tool.parameters) {
                if (param_count > 0) ss << ",";
                ss << "\"" << param.first << "\": {";
                ss << "\"type\": \"" << param.second.type << "\",";
                ss << "\"description\": \"" << param.second.description << "\"";
                if (param.second.required) {
                    ss << ",\"required\": true";
                }
                ss << "}";
                param_count++;
            }
            
            ss << "},";
            ss << "\"required\": [";
            
            size_t required_count = 0;
            for (const auto& param : tool.parameters) {
                if (param.second.required) {
                    if (required_count > 0) ss << ",";
                    ss << "\"" << param.first << "\"";
                    required_count++;
                }
            }
            
            ss << "]";
            ss << "}}";
            if (i < tools.size() - 1) ss << ",";
        }
        ss << "]";
    }
    
    // 添加温度参数
    ss << ",\"temperature\": " << temperature;
    ss << "}";
    
    // 构建网络请求
    NetworkRequest request;
    request.setMethod(ENetworkRequestMethod::ePost);
    request.setUrl(base_url_ + "/chat/completions");
    request.setBody(ss.str());
    
    // 设置请求头
    request.addHeader("Content-Type", "application/json");
    request.addHeader("Authorization", "Bearer " + api_key_);
    
    // 发送请求
    NetworkResponse network_response;
    errc_t error = aNetworkRequest(request, network_response);
    
    if (error != 0) {
        return "{\"error\": \"Failed to send request\"}";
    }
    
    return network_response.body();
}

AST_NAMESPACE_END