///
/// @file      ChatSession.cpp
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

#include "ChatSession.hpp"
#include "AstUtil/JsonValue.hpp"
#include "AstUtil/IO.hpp"
#include "AstUtil/Logger.hpp"

#include "AstUtil/RTTIAPI.hpp"
#include "AstUtil/PropertyVisitor.hpp"
#include "AstUtil/PropertyAll.hpp"
#include "AstCore/PropertyTimePoint.hpp"

AST_NAMESPACE_BEGIN


const char* defaultSystemPrompt = u8R"(
你是你个专业工程师，擅长航天任务设计与分析，能够熟练使用航天任务设计软件，你需要协助用户使用航天进行任务设计与分析。
更具体一点的描述：
- 正在使用的软件是ATK，类似于STK软件
- 你正在协助用户建立一个航天任务分析场景
- 场景中的对象包括：卫星、地面站、传感器等
)";

JsonValue aObjectToJson(Object* obj);

namespace{
    class PropertyVisitorImplForJson : public PropertyVisitor 
    {
    public:
    PropertyVisitorImplForJson()
        : internalJson_()
        , json_(internalJson_)
    {
    }
    PropertyVisitorImplForJson(JsonValue& json)
        : json_(json)
    {
    }
    ~PropertyVisitorImplForJson() override = default;
    errc_t visit(Property& property, const void* container) override
    {

        json()[property.name()] = property.getValue<std::string>(container);
        return 0;
    }
    errc_t visit(PropertyBool& property, const void* container) override
    {
        json()[property.name()] = property.getValue<bool>(container);
        return 0;
    }
    errc_t visit(PropertyDouble& property, const void* container) override
    {
        json()[property.name()] = property.getValue<double>(container);
        return 0;
    }
    errc_t visit(PropertyInt& property, const void* container) override
    {
        json()[property.name()] = property.getValue<int>(container);
        return 0;
    }
    errc_t visit(PropertyString& property, const void* container) override
    {
        json()[property.name()] = property.getValue<std::string>(container);
        return 0;
    }

    errc_t visit(PropertyObject& property, const void* container) override
    {
        Object* obj = nullptr;
        property.getValue(container, &obj);
        json()[property.name()] = aObjectToJson(obj);
        return 0;
    }
    errc_t visit(PropertyStruct& property, const void* container) override
    {
        // @todo
        aWarning("PropertyStruct is not supported in JsonVisitorImplForJson");
        return 0;
    }
    errc_t visit(PropertyQuantity& property, const void* container) override
    {
        json()[property.name()] = property.getValue<std::string>(container);
        return 0;
    }
    errc_t visit(PropertyPOD& property, const void* container) override
    {
        // @todo
        aWarning("PropertyPOD is not supported in JsonVisitorImplForJson");
        return 0;
    }
    errc_t visit(PropertyTimePoint& property, const void* container) override
    {
        json()[property.name()] = property.getValue<std::string>(container);
        return 0;
    }
    JsonValue& json() { return json_; }
private:
    JsonValue internalJson_;
    JsonValue& json_;
};
}


/// @brief 将对象转换为简化的JSON格式
/// @details 包含对象的名称、类型、ID和父作用域ID
/// @param obj 对象指针
/// @return 简化的JSON对象
JsonValue aObjectToBriefJson(Object* obj)
{
    JsonValue json;
    json["name"] = obj->getName();
    json["$type"] = obj->getType()->name();
    json["$id"] = static_cast<int>(obj->getID());
    if(auto parent = obj->getParentScope())
        json["$parent_id"] = static_cast<int>(parent->getID());
    return json;
}

JsonValue aObjectToJson(Object* obj)
{
    if(!obj){return JsonValue();}
    JsonValue json = aObjectToBriefJson(obj);
    auto clazz = obj->getType();
    PropertyVisitorImplForJson visitor(json);
    while(clazz)
    {
        auto& properties = clazz->getProperties();
        for (auto property : properties) {
            if (!property) continue;
            property->accept(visitor, obj);
        }
        clazz = clazz->getParent();
    }
    return visitor.json();
}


ChatSession::ChatSession()
{
    this->setSystemPrompt(defaultSystemPrompt);
}

std::string ChatSession::sendMessage(StringView message)
{
    this->messages_.addUserMessage(message);
    return this->makeChatCompletion();
}


std::string ChatSession::makeChatCompletion()
{
    auto& client = this->client();
    JsonValue json;
    json["messages"] = this->messages_.toJson();
    json["model"] = "deepseek-v4-flash";
    json["temperature"] = 0.2;
    json["thinking"]["type"] = "enabled";  // "enabled" or "disabled"
    json["stream"] = false;
    json["tools"] = u8R"(
        [
            {
                "type": "function",
                "function": {
                    "name": "find_classes",
                    "description": "查找软件支持的所有对象类型"
                }
            },
            {
                "type": "function",
                "function": {
                    "name": "create_object",
                    "description": "创建一个新的对象，如果创建成功，将会返回所创建对象的详细属性信息（与get_object_attributes函数接口返回的属性信息相同）",
                    "parameters": {
                        "type": "object",
                        "properties": {
                            "class": {
                                "type": "string",
                                "description": "对象的类型，必须是通过find_classes返回的类型"
                            },
                            "name": {
                                "type": "string",
                                "description": "对象的名称，可选"
                            },
                            "parent_id": {
                                "type": "integer",
                                "description": "对象的父作用域ID，对象的生命周期将与其父作用域绑定，可选"
                            }
                        }
                    }
                }
            },
            {
                "type": "function",
                "function": {
                    "name": "find_objects",
                    "description": "查找当前设计场景中的对象",
                    "parameters": {
                        "type": "object",
                        "properties": {
                            "class": {
                                "type": "string",
                                "description": "对象的类型，必须是通过find_classes返回的类型，如果没有指定类型，默认查找所有对象"
                            }
                        }
                    }
                }
            },
            {
                "type": "function",
                "function": {
                    "name": "get_object_attributes",
                    "description": "获取指定对象的详细属性信息",
                    "parameters": {
                        "type": "object",
                        "properties": {
                            "id": {
                                "type": "integer",
                                "description": "对象的ID"
                            }
                        }
                    }
                }
            },
            {
                "type": "function",
                "function": {
                    "name": "set_object_attribute",
                    "description": "设置指定对象的属性值",
                    "parameters": {
                        "type": "object",
                        "properties": {
                            "id": {
                                "type": "integer",
                                "description": "对象的ID"
                            },
                            "attribute": {
                                "type": "string",
                                "description": "要设置的属性名"
                            },
                            "value": {
                                "description": "要设置的属性值"
                            }
                        }
                    }
                }
            }
        ]
    )"_json;

    {
        JsonValue res = client.chat(json);
        if(!res["error"].isNull())
        {
            aError("%s", res["error"]["message"].toString().c_str());
            return aText("系统错误");
        }
        // ast_printf("res: %s\n", res.toJsonString().c_str());
        JsonValue& message = res["choices"][0]["message"];
        std::string response = message["content"].toString();
        ast_printf("ai: \n%s\n", response.c_str());
        JsonValue& toolCalls = message["tool_calls"];
        auto msg = ChatMessage::Assistant(response, toolCalls);
        if(!message["reasoning_content"].isNull())
            msg.setReasoningContent(message["reasoning_content"].toString());
        this->messages_.addMessage(msg);

        if(toolCalls.isArray() && toolCalls.size() > 0)
        {
            this->handleToolCalls(toolCalls);
        }
        return response;
    }
}

void ChatSession::setSystemPrompt(StringView systemPrompt)
{
    this->messages_.setSystemPrompt(systemPrompt);
}

void ChatSession::handleToolCalls(const JsonValue &toolCalls)
{
    if(toolCalls.isArray() && toolCalls.size() > 0)
    {
        for(auto& item: toolCalls.getArray())
        {
            std::string response = this->handleToolCall(item);
            std::string id = item["id"];
            // aDebug("tool %s: %s", item["function"]["name"].toString().c_str(), response.c_str());
            this->messages_.addToolMessage(response, id);
        }
        this->makeChatCompletion();
    }
}



std::string ChatSession::handleToolCall(const JsonValue &toolCall)
{
    auto& function = toolCall["function"];
    std::string name = function["name"];
    JsonValue args;
    args.parseFromString(function["arguments"].toString());
    // ast_printf("name: %s, arguments: %s\n", name.c_str(), args.toJsonString().c_str());
    if(name == "find_classes")
    {
        std::vector<std::string> classes;
        aGetAllClassNames(classes);
        JsonValue json;
        for(auto& cls : classes)
        {
            json.append(cls);
        }
        return json.toJsonString();
    }
    else if(name == "create_object")
    {
        std::string className = args["class"];
        Object* parentObj = nullptr;
        if(!args["parent_id"].isNull())
        {
            parentObj = aGetObject(args["parent_id"].toInt());
            if(!parentObj)
            {
                aError("parent object %d not found", args["parent_id"].toInt());
                return u8"父对象不存在";
            }
        }
        Object* obj = aNewObject(className, parentObj);
        if(obj)
        {
            obj->setName(args["name"].toString());
            return aObjectToJson(obj).toJsonString();
        }
        else
        {
            if(aIsVirtualClass(className))
            {
                aError("virtual class %s is not supported", className.c_str());
                return u8"虚类" + className + u8"不支持创建对象";
            }
            aError("create object failed: %s", className.c_str());
            return u8"创建对象失败";
        }
    }
    else if(name == "find_objects")
    {
        std::vector<Object*> objectsMatched;
        std::vector<Object*> objects = aGetAllObjects();
        if(!args["class"].isNull())
        {
            Class* cls = aGetClass(args["class"].toString());
            if(cls)
            {
                for(auto& obj : objects)
                {
                    if(obj->getType() == cls)
                    {
                        objectsMatched.push_back(obj);
                    }
                }
            }
        }
        else{
            objectsMatched = objects;
        }

        JsonValue json;
        for(auto& obj : objectsMatched)
        {
            JsonValue objJson = aObjectToBriefJson(obj);
            json.append(objJson);
        }
        return json.toJsonString();
    }
    else if(name == "get_object_attributes")
    {
        int id = args["id"].toInt();
        Object* obj = aGetObject(id);
        if(obj)
        {
            JsonValue json = aObjectToJson(obj);
            std::string str = json.toJsonString();
            return str;
        }
        else
        {
            aError("get object failed: %d", id);
            return u8"获取对象失败，可能是相应id的对象不存在或已被删除";
        }
    }
    else if(name == "set_object_attribute")
    {
        int id = args["id"].toInt();
        std::string attr = args["attribute"].toString();
        std::string value = args["value"].toString();
        Object* obj = aGetObject(id);
        if(obj)
        {
            errc_t rc = obj->setAttrString(attr, value);
            if(rc != 0)
            {
                aError("failed to set object attribute: %s", attr.c_str());
                return u8"设置对象属性失败，可能是数值不合法或者属性是只读的";
            }
            else
            {
                return u8"设置对象属性成功";
            }
        }
        else
        {
            aError("set object attribute failed: %d", id);
            return u8"设置对象属性失败，可能是相应id的对象不存在或已被删除";
        }
    }
    else
    {
        aError("unsupported tool call: %s", name.c_str());
        return u8"暂时还不支持调用工具" + name;
    }
}

OpenAI& ChatSession::client() {
    if (client_ == nullptr) {
        client_ = &internalClient_;
    }
    return *client_;
}

AST_NAMESPACE_END
