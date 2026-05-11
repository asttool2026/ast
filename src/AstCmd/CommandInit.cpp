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


void aCommandDispatcherInit(CommandDispatcher& dispatcher)
{
    REGISTER_COMMAND(dispatcher,
        "Test1 <string> <quoted>",
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

AST_NAMESPACE_END


