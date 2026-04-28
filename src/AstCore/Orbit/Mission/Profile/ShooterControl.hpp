///
/// @file      ShooterControl.hpp
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

/// @brief 射击控制器
class AST_CORE_API ShooterControl : public ObjectNamed
{
public:

    ShooterControl() = default;
    ~ShooterControl() override = default;

    bool active_ = true;
    std::string category_;
    bool clonable_ = true;
    std::string controlName_;
    double correction_ = 0.0;
    std::string description_;
    std::string dimension_;
    double maxStep_ = 100.0;
    std::string parentSegmentName_;
    std::string parentType_;
    double perturbation_ = 0.1;
    bool readOnly_ = false;
    double scale_ = 1.0;
    std::string slaveSegments_;
    int stkVersion_ = 1160;
    double tolerance_ = 1e-6;
    double totalCorrection_ = 0.0;
    std::string unit_;
    bool useCustomUnit_ = false;
    std::string userComment_;
    std::string version_;
};

AST_NAMESPACE_END