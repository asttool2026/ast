///
/// @file      DifferentialCorrectorProfile.hpp
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
#include "TargeterProfile.hpp"
#include "AstCore/ShooterControl.hpp"
#include "AstCore/ShooterResult.hpp"
#include "AstCore/ScriptingTool.hpp"
#include "AstCore/TargeterGraph.hpp"

AST_NAMESPACE_BEGIN

/*!
    @addtogroup 
    @{
*/

/// @brief 微分修正器配置文件
class AST_CORE_API DifferentialCorrectorProfile : public TargeterProfile
{
public:

    DifferentialCorrectorProfile() = default;
    ~DifferentialCorrectorProfile() override = default;

    bool addBPlanePointsForNominals_ = false;
    bool addBPlanePointsForPerturbations_ = false;
    bool clearCorrectionsBeforeRun_ = true;
    std::string convergenceCriteria_;
    bool displayPopup_ = true;
    std::string finiteDifferenceMethod_;
    bool hideInactiveControls_ = false;
    bool hideInactiveResults_ = false;
    double lineSearchLowerBound_ = 1e-6;
    int lineSearchMaxNumCalls_ = 10;
    double lineSearchTolerance_ = 1e-6;
    double lineSearchUpperBound_ = 10.0;
    int maximumRunsToLog_ = 25;
    int maxIterations_ = 50;
    int numHomotopySteps_ = 1;
    int numIterations_ = 1;
    std::string perturbationColor_;
    std::string rootFindingAlgorithm_;
    std::string scaleControls_;
    std::string scaleGoals_;
    std::string status_;
    bool stopOnLimitCycleDetection_ = true;
    bool useHomotopy_ = false;
    bool useLineSearch_ = false;

    std::vector<SharedPtr<ShooterControl>> controls_;
    std::vector<SharedPtr<ShooterResult>> results_;
    std::vector<SharedPtr<TargeterGraph>> graphs_;
    SharedPtr<ScriptingTool> scriptingTool_;
};

/*! @} */

AST_NAMESPACE_END
