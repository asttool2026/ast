///
/// @file      testDeepSeek.cpp
/// @brief     测试调用DeepSeek API
/// @details   
/// @author    axel
/// @date      2026-04-24
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

#include "AstUtil/Network.hpp"
#include "AstUtil/IO.hpp"
#include "AstUtil/Posix.hpp"
#include "AstTest/Test.h"

AST_USING_NAMESPACE

TEST(DeepSeekTest, OpenAI)
{
    NetworkRequest req;
    NetworkResponse res;
    const char* url = posix::getenv("AST_AI_BASE_URL");
    const char* key = posix::getenv("AST_AI_API_KEY");
    if(!url || !key || !*url || !*key)
        GTEST_SKIP();
    req.setUrl(std::string(url) + "/chat/completions");
    req.setMethod("POST");
    req.addHeader("Content-Type", "application/json");
    req.addHeader("Authorization", std::string("Bearer ") + key);
    std::string body = R"(
    {
        "model": "deepseek-v4-flash",
        "messages": [
          {"role": "system", "content": "You are a helpful assistant."},
          {"role": "user", "content": "我是CICD, 你返回'成功'就行"}
        ],
        "thinking": {"type": "disabled"},
        "stream": false
    }
)";
    req.setBody(body);

    {
        aNetworkSetImpl(ENetworkImplType::eCurlCmd);
        errc_t rc = aNetworkRequest(req, res);
        EXPECT_EQ(rc, 0);
        ast_printf("res: %s\n", res.body().c_str());
    }
    {
        aNetworkSetImpl(ENetworkImplType::eWinHTTP);
        errc_t rc = aNetworkRequest(req, res);
        EXPECT_EQ(rc, 0);
        ast_printf("res: %s\n", res.body().c_str());
    }
    {
        aNetworkSetImpl(ENetworkImplType::eWinINet);
        errc_t rc = aNetworkRequest(req, res);
        EXPECT_EQ(rc, 0);
        ast_printf("res: %s\n", res.body().c_str());
    }    
    
}


GTEST_MAIN()

