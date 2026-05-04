///
/// @file      ODEEventDetector.cpp
/// @brief     ~
/// @details   ~
/// @author    axel
/// @date      2026-01-16
/// @copyright 版权所有 (C) 2026-present, ast项目.
///
/// ast项目（https://github.com/space-ast/ast）
/// 本项目基于 Apache 2.0 开源许可证分发。
/// 您可在遵守许可证条款的前提下使用、修改和分发本软件。
/// 许可证全文请见：
/// 
///    http://www.apache.org/licenses/LICENSE-2.0
/// 
/// 重要须知：
/// 软件按"现有状态"提供，无任何明示或暗示的担保条件。
/// 除非法律要求或书面同意，作者与贡献者不承担任何责任。
/// 使用本软件所产生的风险，需由您自行承担。

#include "ODEEventDetector.hpp"
#include "AstMath/Bracket.hpp"
#include <cmath>

AST_NAMESPACE_BEGIN

bool ODEEventDetector::containsEvent(const Bracket& bracket) const
{
    auto direction = direction_;
    bool lastSign = std::signbit(bracket.leftValue());
    bool currentSign = std::signbit(bracket.rightValue());
    double delta = bracket.rightAbscissa() - bracket.leftAbscissa();

    // 处理事件的穿越方向
    if(direction == ODEEventDetector::eBoth){
        return lastSign ^ currentSign;
    }else{
        static_assert(ODEEventDetector::eDecrease < 0, "value not correct");
        static_assert(ODEEventDetector::eIncrease > 0, "value not correct");
        // 正向预报的上升触发相当于反向预报的下降触发
        bool timeSign = std::signbit(delta * (int)direction);
        return (lastSign^timeSign) && !(currentSign^timeSign);
    }
}

AST_NAMESPACE_END
