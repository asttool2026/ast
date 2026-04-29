///
/// @file      ShooterResult.hpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-04-28
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
#include "AstUtil/Object.hpp"
#include "AstUtil/ObjectNamed.hpp"
#include <string>

AST_NAMESPACE_BEGIN

/// @brief 结果变量，用于定义微分修正中的结果变量
class AST_CORE_API ShooterResult : public ObjectNamed
{
public:
    AST_OBJECT(ShooterResult)
    static ShooterResult* New();

    ShooterResult() = default;
    ~ShooterResult() override = default;

    bool active() const { return active_; }
    void setActive(bool active) { active_ = active; }

    double desired() const { return desired_; }
    void setDesired(double desired) { desired_ = desired; }
    
    double scale() const { return scale_; }
    void setScale(double scale) { scale_ = scale; }
    
    double tolerance() const { return tolerance_; }
    void setTolerance(double tolerance) { tolerance_ = tolerance; }
    
    bool valid() const { return valid_; }
    void setValid(bool valid) { valid_ = valid; }
    
    double weight() const { return weight_; }
    void setWeight(double weight) { weight_ = weight; }
    
private:
    bool active_{false};
    bool valid_{true};
    double desired_{0.0};
    double scale_{1.0};
    double tolerance_{0.0};
    double weight_{1.0};
};

AST_NAMESPACE_END