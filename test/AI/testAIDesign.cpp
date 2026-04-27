///
/// @file      testAIDesign.cpp
/// @brief     测试AI辅助设计
/// @details   测试AI辅助设计框架的基础能力，包括设计轨道、计算状态等
/// @author    axel
/// @date      2026-04-26
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

#include "AstAI/ChatSession.hpp"
#include "AstSim/Satellite.hpp"
#include "AstCore/StateCartesian.hpp"
#include "AstCore/TimePoint.hpp"
#include "AstUtil/IO.hpp"
#include "AstUtil/RTTIAPI.hpp"
#include "AstUtil/Literals.hpp"
#include "AstTest/Test.h"

AST_USING_NAMESPACE



TEST(DeepSeekTest, StateCartesian)
{
    if(aIsCI())
        GTEST_SKIP();
    aRemoveAllObjects();
    ChatSession session;
    std::vector<std::string> messages({
        R"(帮我新建一个笛卡尔状态:
        - 参考系是地球惯性系
        - 轨道历元是2026-04-26 00:00:00
        - 位置速度分别为100km, 200e3, 300km, 3km/s, 2200m/s, 1km/s
        )"
    });
    for(auto& message : messages)
    {
        std::string response = session.sendMessage(message);
    }
    StateCartesian* state = dynamic_cast<StateCartesian*>(aFindObject(StateCartesian::StaticType()));
    ASSERT_TRUE(state != nullptr);
    EXPECT_EQ(state->x(), 100.0_km);
    EXPECT_EQ(state->y(), 200e3_m);
    EXPECT_EQ(state->z(), 300.0_km);
    EXPECT_EQ(state->vx(), 3.0_km_s);
    EXPECT_EQ(state->vy(), 2200.0_m_s);
    EXPECT_EQ(state->vz(), 1.0_km_s);
    EXPECT_EQ(state->getStateEpoch_TimePoint(), TimePoint::Parse("2026-04-26 00:00:00"));
}


TEST(DeepSeekTest, SSODesign)
{
    GTEST_SKIP();
    {Satellite sat;} // for link libray AstSim
    ChatSession session;
    std::vector<std::string> messages({
        "帮我设计一个太阳同步轨道"
    });
    for(auto& message : messages)
    {
        std::string response = session.sendMessage(message);
        // ast_printf("AI: %s\n", response.c_str());
    }
}



GTEST_MAIN()