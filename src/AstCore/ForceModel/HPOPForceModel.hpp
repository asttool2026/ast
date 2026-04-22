///
/// @file      HPOPForceModel.hpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-03-25
/// @copyright 版权所有 (C) 2026-present, ast项目.
///
/// SpaceAST项目（https://github.com/space-ast/ast）
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

#pragma once

#include "AstGlobal.h"
#include "AstCore/CelestialBody.hpp"
#include "AstCore/GravityForce.hpp"
#include "AstCore/TwoBodyForce.hpp"
#include "AstCore/DragForce.hpp"
#include "AstCore/ThirdBodyForce.hpp"
#include "AstCore/SolarRadiationPressure.hpp"
#include "AstCore/RelativityCorrection.hpp"
#include "AstUtil/Constants.h"
#include "AstUtil/ScopedPtr.hpp"
#include "AstUtil/ClonePtr.hpp"
#include <vector>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup 
    @{
*/



/// @brief 高精度轨道预报力模型
class AST_CORE_API HPOPForceModel
{
public:
    HPOPForceModel();
    ~HPOPForceModel();

    // HPOPForceModel(HPOPForceModel&& other);
    // HPOPForceModel& operator=(HPOPForceModel&& other);

    using ThirdBodyList = std::vector<ThirdBodyForce>;
    
    /// @brief 获取中心天体引力场模型
    /// @warning 调用该接口后，中心天体引力模型将被设置为引力场模型
    /// @return 中心天体引力模型
    GravityForce& gravity();

    /// @brief 获取中心天体二体引力模型
    /// @warning 调用该接口后，中心天体引力模型将被设置为二体引力模型
    /// @return 二体引力模型
    TwoBodyForce& twoBody();
    
    /// @brief 获取中心天体引力模型
    /// @return 中心天体引力模型
    BodyAttraction& bodyAttraction();

    /// @brief 获取中心天体引力模型
    /// @return 中心天体引力模型
    const BodyAttraction& bodyAttraction() const;
public:
    bool                        useDrag_{false};                    ///< 是否使用阻力模型
    bool                        useSRP_{false};                     ///< 是否使用太阳辐射压模型
    bool                        useMoonGravity_{false};             ///< 是否使用月球引力
    SharedPtr<Body>             centerBody_;                        ///< 中心天体
    DragForce                   drag_;                              ///< 大气阻力
    SolarRadiationPressure      srp_;                               ///< 太阳辐射压模型
    double                      moonGravity_{kMoonGrav};            ///< 月球引力系数
    ThirdBodyList               thirdBodies_;                       ///< 三体引力
private:
    ClonePtr<BodyAttraction>    bodyAttraction_;                    ///< 中心天体引力模型(二体或者引力场)
};


/*! @} */

AST_NAMESPACE_END
