///
/// @file      CommandInit.cpp
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

#include "CommandInit.hpp"
#include "CommandDispatcher.hpp"
#include "AstUtil/Logger.hpp"
#include "AstUtil/IO.hpp"

AST_NAMESPACE_BEGIN


void aCommandInitMover(CommandDispatcher& dispatcher)
{
    REGISTER_COMMAND(dispatcher,
        "SetState <object> Cartesian <string> <string> <string> "
        "<double> <string> <string> "
        "<double> <double> <double> <double> <double> <double>",
        [](Object* obj, StringView propagator, StringView start, StringView stop,
        double stepSize, StringView coordSys, StringView epoch,
        double x, double y, double z, double vx, double vy, double vz) 
        {
            // 业务逻辑...
            printf("SetState Cartesian:\n");
            printf("object = %p\n", obj);
            printf("propagator = %.*s\n", propagator.size(), propagator.data());
            printf("start = %.*s\n", start.size(), start.data());
            printf("stop = %.*s\n", stop.size(), stop.data());
            printf("stepSize = %f\n", stepSize);
            printf("coordSys = %.*s\n", coordSys.size(), coordSys.data());
            printf("epoch = %.*s\n", epoch.size(), epoch.data());
            printf("x = %lf\n", x);
            printf("y = %lf\n", y);
            printf("z = %lf\n", z);
            printf("vx = %lf\n", vx);
            printf("vy = %lf\n", vy);
            printf("vz = %lf\n", vz);
            return 0;
        }
    );

    REGISTER_COMMAND(dispatcher,
        "SetState <object> Classical <string> <string> <string> "
        "<double> <string> <string> "
        "<double> <double> <double> <double> <double> <double>",
        [](Object* obj, StringView propagator, StringView start, StringView stop,
        double stepSize, StringView coordSys, StringView epoch,
        double a, double ecc, double inc, double arg, double raan, double M) 
        {
            // 业务逻辑...
            printf("SetState Classical:\n");
            printf("object = %p\n", obj);
            printf("propagator = %.*s\n", propagator.size(), propagator.data());
            printf("start = %.*s\n", start.size(), start.data());
            printf("stop = %.*s\n", stop.size(), stop.data());
            printf("stepSize = %f\n", stepSize);
            printf("coordSys = %.*s\n", coordSys.size(), coordSys.data());
            printf("epoch = %.*s\n", epoch.size(), epoch.data());
            printf("a = %lf\n", a);
            printf("ecc = %lf\n", ecc);
            printf("inc = %lf\n", inc);
            printf("arg = %lf\n", arg);
            printf("raan = %lf\n", raan);
            printf("M = %lf\n", M);
            return 0;
        }
    );

}

void aCommandInitBasic(CommandDispatcher& dispatcher)
{
    REGISTER_COMMAND(dispatcher,
        "Test1 <string> <string>",
        [](StringView scenario, StringView filePath) {
            ast_printf("Test1: Scenario = '%.*s', FilePath = '%.*s'\n", 
                scenario.size(), scenario.data(), 
                filePath.size(), filePath.data());
            return 0;
        }
    );

    REGISTER_COMMAND(dispatcher,
        "Test2 <string> <string>",
        [](StringView appPath, StringView unit) {
            ast_printf("Test2: Application = '%.*s', Unit = '%.*s'\n", 
                appPath.size(), appPath.data(), 
                unit.size(), unit.data());
            return 0;
        }
    );

    REGISTER_COMMAND(dispatcher,
        "Test3 <string> <string>",
        [](StringView objectPath, StringView style) {
            ast_printf("Test3: Object = '%.*s', Style = '%.*s'\n", 
                objectPath.size(), objectPath.data(), 
                style.size(), style.data());
            return 0;
        }
    );

    REGISTER_COMMAND(dispatcher,
        "DoesObjExist <string> <object>",
        [](StringView applicationPath, Object* object) 
        {
            if(object)
                return "1";
            else
                return "0";
        }
    );

    REGISTER_COMMAND(dispatcher,
        "New <string> <string> <string>",
        [](StringView applicationPath, StringView classPath, StringView newObjectName) 
        {
            // @todo: create new object
            return 0;
        }
    );
}


void aCommandDispatcherInit(CommandDispatcher& dispatcher)
{
    aCommandInitBasic(dispatcher);
    aCommandInitMover(dispatcher);
}

AST_NAMESPACE_END


