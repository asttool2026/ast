///
/// @file      OpenAI.cpp
/// @brief     
/// @details   
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
#include <curl/curl.h>
#include <sstream>
#include <iostream>

AST_NAMESPACE_BEGIN

/**
 * @brief 回调函数，用于接收HTTP响应
 */
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * @brief 构造函数
 */
OpenAIClient::OpenAIClient(const std::string& api_key, const std::string& base_url) 
    : api_key_(api_key), base_url_(base_url) {
}

/**
 * @brief 发送聊天请求
 */
std::string OpenAIClient::chat(const std::string& model, const std::vector<ChatMessage>& messages, 
                              const std::vector<AITool>& tools, float temperature) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "{\"error\": \"Failed to initialize curl\"}";
    }

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
        switch (msg.role) {
            case MessageRole::USER: role_str = "user";
 break;
            case MessageRole::ASSISTANT: role_str = "assistant";
 break;
            case MessageRole::SYSTEM: role_str = "system";
 break;
            case MessageRole::TOOL: role_str = "tool";
 break;
        }
        
        ss << "\"role\": \"" << role_str << "\",";
        ss << "\"content\": \"" << msg.content << "\"";
        
        if (msg.role == MessageRole::TOOL && !msg.tool_call_id.empty()) {
            ss << ",\"tool_call_id\": \"" << msg.tool_call_id << "\"";
        }
        
        ss << "}";
        if (i < messages.size() - 1) ss << ",";
    }
    ss << "],";
    
    // 添加工具
    if (!tools.empty()) {
        ss << "\"tools\": [";
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
        ss << "]",