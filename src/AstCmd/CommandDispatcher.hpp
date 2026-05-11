///
/// @file      CommandManager.hpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-05-11
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
#include "AstUtil/StringView.hpp"
#include "AstUtil/Span.hpp"
#include "AstUtil/ParseFormat.hpp"
#include "AstUtil/RTTIAPI.hpp"
#include "CommandHandler.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <type_traits>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup 
    @{
*/

namespace detail{

/// @brief 查找命令参数结束位置
constexpr int find_closing(const char* s, int p) {
    return s[p] == '>' ? p : find_closing(s, p + 1);
}


/// @brief 检查命令参数是否为指定类型
constexpr bool is_type(const char* s, int p, const char* type) {
    int i = 0;
    while (type[i] != '\0') {
        if (s[p + i] != type[i]) return false;
        ++i;
    }
    return s[p + i] == '>';
}

/// @brief 统计命令参数个数
constexpr int count_args(const char* s, int p = 0) {
    return s[p] == '\0' ? 0 :
           s[p] == '<'  ? 1 + count_args(s, p + 1) :
           count_args(s, p + 1);
}


enum{
    type_none = 0,
    type_bool = 1,
    type_int = 2,
    type_double = 3,
    type_string = 4,
    type_quoted = 5,
    type_object = 6,
    num_types = 7,
};

constexpr int get_parameter_tag(const char* s, int p = 0) {
    return s[p] == '\0' ? 0 :
           s[p] == '<'  ? (
               is_type(s, p+1, "bool")   ? get_parameter_tag(s, find_closing(s, p)+1) * num_types + type_bool :
               is_type(s, p+1, "int")    ? get_parameter_tag(s, find_closing(s, p)+1) * num_types + type_int :
               is_type(s, p+1, "double") ? get_parameter_tag(s, find_closing(s, p)+1) * num_types + type_double :
               is_type(s, p+1, "string") ? get_parameter_tag(s, find_closing(s, p)+1) * num_types + type_string :
               is_type(s, p+1, "quoted") ? get_parameter_tag(s, find_closing(s, p)+1) * num_types + type_quoted :
               is_type(s, p+1, "object") ? get_parameter_tag(s, find_closing(s, p)+1) * num_types + type_object :
               throw "invalid parameter type"
           ) :
           get_parameter_tag(s, p + 1);
}

template <int Pos, typename T>
struct arg_pair {
    static constexpr int pos = Pos;
    using type = T;
};


// 1. 定义类型码到 C++ 类型的映射 traits
template <int TypeCode>
struct type_from_tag;

template <> struct type_from_tag<type_bool> { using type = bool;           }; // <bool>   → bool
template <> struct type_from_tag<type_int> { using type = int;             }; // <int>    → int
template <> struct type_from_tag<type_double> { using type = double;       }; // <double> → double
template <> struct type_from_tag<type_string> { using type = StringView;   }; // <string> → StringView
template <> struct type_from_tag<type_quoted> { using type = StringView;   }; // <quoted> → StringView
template <> struct type_from_tag<type_object> { using type = Object*;      }; // <object> → Object*


// 2. 极度简化的 make_arg_pair
template <int Tag, int Pos>
struct make_arg_pair {
    using type = arg_pair<Pos, typename type_from_tag<Tag % num_types>::type>;
};

// 递归解码，用 std::tuple 累积 arg_pair 类型（避免 seq 可变参数包问题）
template <int Tag, int Pos, int Count>
struct decode_impl {
    using type = decltype(
        std::tuple_cat(
            std::tuple<typename make_arg_pair<Tag % num_types, Pos>::type>(),
            typename decode_impl<Tag / num_types, Pos + 1, Count - 1>::type()
        )
    );
};

template <int Tag, int Pos>
struct decode_impl<Tag, Pos, 0> {
    using type = std::tuple<>;
};



// 对外接口：给定 tag 和参数个数 N，得到 std::tuple<arg_pair<Pos, Type>...>
template <int Tag, int N>
using decode_tag = typename decode_impl<Tag, 0, N>::type;


inline std::vector<StringView> split(StringView str) 
{
    std::vector<StringView> result;
    const char* p = str.data();
    const char* end = p + str.size();

    while (p < end) {
        // 跳过前导空白
        while (p < end && std::isspace(static_cast<unsigned char>(*p))) {
            ++p;
        }
        if (p >= end) break;

        if (*p == '"') {
            ++p; // 跳过左引号
            const char* start = p;
            while (p < end && *p != '"') {
                ++p;
            }
            // 添加引号内的内容（可能为空）
            result.emplace_back(start, static_cast<std::size_t>(p - start));
            if (p < end) ++p; // 跳过右引号
        } else {
            const char* start = p;
            while (p < end && !std::isspace(static_cast<unsigned char>(*p)) && *p != '"') {
                ++p;
            }
            result.emplace_back(start, static_cast<std::size_t>(p - start));
        }
    }
    return result;
}

inline std::string extract_command_name(const char* tmpl) {
    std::string name;
    while (*tmpl && *tmpl != ' ' && *tmpl != '<') {
        name += *tmpl++;
    }
    return name;
}


// ================== 运行时字符串转类型 ==================

template <typename T>
T convert_token(StringView s);

template <>
inline bool convert_token<bool>(StringView s) {
    return aParseBool(s);
}

template <>
inline int convert_token<int>(StringView s) {
    return aParseInt(s);
}


template <>
inline double convert_token<double>(StringView s) {
    return aParseDouble(s);
}

template<>
inline Object* convert_token<Object*>(StringView s) {
    return aResolveObject(s);
}

template <>
inline StringView convert_token<StringView>(StringView s) 
{
    return s;
}


// 处理 void 返回值
inline errc_t fill_result(CommandResult& /*result*/) {
    return 0;
}

// 处理 int（及其他整数类型）
template <typename T>
typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, errc_t>::type
fill_result(CommandResult& /*result*/, T ret) {
    return static_cast<errc_t>(ret);
}

// 处理 std::vector<std::string>
inline errc_t fill_result(CommandResult& result, std::vector<std::string> vec) {
    result = std::move(vec);
    return 0;
}

// 处理 std::string
inline errc_t fill_result(CommandResult& result, const std::string& str) {
    result.clear();
    result.push_back(str);
    return 0;
}
inline errc_t fill_result(CommandResult& result, std::string&& str) {
    result.clear();
    result.push_back(std::move(str));
    return 0;
}
// 处理 const char*
inline errc_t fill_result(CommandResult& result, const char* str) {
    result.clear();
    result.push_back(str);
    return 0;
}
template <typename T>
typename std::enable_if<!std::is_integral<T>::value &&
                        !std::is_same<T, std::vector<std::string>>::value &&
                        !std::is_same<T, std::string>::value &&
                        !std::is_same<T, const char*>::value, errc_t>::type
fill_result(CommandResult& /*result*/, T ret) {
    return static_cast<errc_t>(ret);
}


}



template <typename F, typename Tuple>
class TaggedRule : public CommandHandler 
{
public:
    explicit TaggedRule(F f) 
        : handler_(std::move(f)) 
    {}

    errc_t handle(const Span<StringView> tokens, CommandResult& result) const override {
        if (tokens.size() != std::tuple_size<Tuple>::value) {
            aError("invalid token count");
            return eErrorInvalidParam;
        }
        return call(tokens, result, Tuple{});
    }

private:
    // 展开 tuple<arg_pair<Pos,Type>...>
    template <typename... Args>
    errc_t call(const Span<StringView> toks, CommandResult& result, std::tuple<Args...>) const {
        try {
            // 调用 handler，并通过 fill_result 自动分发返回值
            return detail::fill_result(result, 
                handler_(detail::convert_token<typename Args::type>(toks[Args::pos])...));
        } catch (...) {
            return static_cast<errc_t>(-1);  // 异常统一返回 -1
        }
    }
private:
    F handler_;
};


class AST_CMD_API CommandDispatcher 
{
public:
    using CommandHandlerMap = std::unordered_map<std::string, std::shared_ptr<CommandHandler>>;
    CommandDispatcher() = default;
    explicit CommandDispatcher(bool whetherInit);
    ~CommandDispatcher() = default;
    errc_t execute(StringView command, CommandResult& result) const;

    // 注册命令：使用 integral_constant 技巧绕过 decltype(lambda) 的未计算上下文限制
    template <typename F, int Tag, int N>
    void registerCommand(const std::string& name, F handler,
                          std::integral_constant<int, Tag>,
                          std::integral_constant<int, N>)
    {
        using ArgTuple = detail::decode_tag<Tag, N>;
        auto rule = std::make_shared<TaggedRule<F, ArgTuple>>(std::move(handler));
        commands_[name] = std::move(rule);
    }
private:
    CommandHandlerMap commands_;
};


#define _AST_REGISTER_COMMAND(dispatcher, cmd_template, handler) \
    { \
        constexpr int tag = detail::get_parameter_tag(cmd_template);  \
        constexpr int n   = detail::count_args(cmd_template);         \
        std::string name = detail::extract_command_name(cmd_template);\
        (dispatcher).registerCommand(name, handler, \
            std::integral_constant<int, tag>{}, \
            std::integral_constant<int, n>{}); \
    }

#define REGISTER_COMMAND(dispatcher, cmd_template, handler) \
    _AST_REGISTER_COMMAND(dispatcher, cmd_template, handler)

/*! @} */

AST_NAMESPACE_END
