///
/// @file      testActiveScriptExecutor.cpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-04-30
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

#include "AstScript/JScriptExecutor.hpp"
#include "AstScript/VBScriptExecutor.hpp"
#include "AstUtil/IO.hpp"
#include "AstTest/Test.h"


AST_USING_NAMESPACE

#ifdef _WIN32

TEST(ActiveScriptExecutorTest, JScript)
{
    JScriptExecutor executor;
    errc_t rc = executor.initialize();
    EXPECT_EQ(rc, 0);

    // 输入变量（根据实际需求修改）
    rc = executor.setVariable("FinalRMag", 7000); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("CurrentApoapsisMag", 6800); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("CurrentEccentricity", 0.2); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("CurrentTrueAnomaly", 2.1); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("StopOnPeriapsisCoast", false); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("StopOnApoapsisCoast", false); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("StopOnPeriapsisTransfer", false); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("StopOnApoapsisTransfer", false); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("SkipInitialCoast", ""); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("DesiredRMagScript", 0); EXPECT_EQ(rc, 0);
    rc = executor.setVariable("DesiredRMagDC", 0); EXPECT_EQ(rc, 0);


    // 这是用户提供的脚本
    const char* userScript = u8R"script(
        // This script sets up a Hohmann transfer sequence.
        // ... (完整脚本内容如下)
        var startTrueAnomA;
        var startTrueAnomB;

        if (FinalRMag > CurrentApoapsisMag)
        {
            StopOnPeriapsisCoast = true;
            StopOnApoapsisCoast = false;
            StopOnPeriapsisTransfer = false;
            StopOnApoapsisTransfer = true;
            startTrueAnomA = 0.0;
            startTrueAnomB = 360.0;
        }
        else
        {
            StopOnPeriapsisCoast = false;
            StopOnApoapsisCoast = true;
            StopOnPeriapsisTransfer = true;
            StopOnApoapsisTransfer = false;
            startTrueAnomA = 180.0;
            startTrueAnomB = 180.0;
        }

        if ( CurrentEccentricity < 0.01 ||
             Math.abs(startTrueAnomA - CurrentTrueAnomaly) < 0.01 ||
             Math.abs(startTrueAnomB - CurrentTrueAnomaly) < 0.01 )
        {
            SkipInitialCoast = "Enable";
        }
        else
        {
            SkipInitialCoast = "Disable";
        }

        DesiredRMagScript = FinalRMag;
        DesiredRMagDC = FinalRMag;
    )script";

    // 执行脚本
    rc = executor.execute(userScript);
    EXPECT_EQ(rc, 0);

    // 输出变量
    {
        std::string skipInitialCoast;
        rc = executor.getVariable("SkipInitialCoast", skipInitialCoast);
        EXPECT_EQ(rc, 0);
        EXPECT_EQ(skipInitialCoast, "Disable");
    }
    {
        bool stopOnPeriapsisCoast;
        rc = executor.getVariable("StopOnPeriapsisCoast", stopOnPeriapsisCoast);
        EXPECT_EQ(rc, 0);
        EXPECT_EQ(stopOnPeriapsisCoast, true);
    }
    {
        double desiredRMagScript;
        rc = executor.getVariable("DesiredRMagScript", desiredRMagScript);
        EXPECT_EQ(rc, 0);
        EXPECT_EQ(desiredRMagScript, 7000.0);
    }
    {
        double desiredRMagDC;
        rc = executor.getVariable("DesiredRMagDC", desiredRMagDC);
        EXPECT_EQ(rc, 0);
        EXPECT_EQ(desiredRMagDC, 7000.0);
    }
}


TEST(ActiveScriptExecutorTest, ErrorHandling)
{
    JScriptExecutor executor;
    errc_t rc = executor.initialize();
    EXPECT_EQ(rc, 0);
    

    // 执行运行时错误的脚本：没有定义的变量
    // for(int i=0;i<3;i++)
    {
        const char* script = u8R"(
            if (FinalRMag > CurrentApoapsisMag)
            {
                StopOnPeriapsisCoast = true;
                StopOnApoapsisCoast = false;
                StopOnPeriapsisTransfer = false;
                StopOnApoapsisTransfer = true;
                startTrueAnomA = 0.0;
                startTrueAnomB = 360.0;
            }
        )";
        std::string errorOut;
        rc = executor.execute(script, &errorOut);
        EXPECT_NE(rc, 0);
        EXPECT_TRUE(!errorOut.empty());
    }

    // 测试执行错误的脚本后，是否能继续执行正常的脚本
    {
        std::string errorOut;
        rc = executor.execute("var a = 1", &errorOut);
        EXPECT_EQ(rc, 0);
        EXPECT_TRUE(errorOut.empty());
        
        int a = 1;
        rc = executor.getVariable("a", a);
        EXPECT_EQ(rc, 0);
        EXPECT_EQ(a, 1);
    }

    // 执行语法的脚本
    // for(int i=0;i<3;i++)
    {
        std::string errorOut;
        rc = executor.execute("....=@# ! abcx9279$%323", &errorOut);
        EXPECT_NE(rc, 0);
        EXPECT_TRUE(!errorOut.empty());
    }
}

#endif


GTEST_MAIN()

