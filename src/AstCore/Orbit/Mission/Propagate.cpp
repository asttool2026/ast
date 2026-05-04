///
/// @file      Propagate.cpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-04-15
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

#include "Propagate.hpp"
#include "AstCore/HPOP.hpp"
#include "AstUtil/Logger.hpp"
#include "AstCore/SpacecraftState.hpp"
#include "AstCore/OrbitElement.hpp"
#include "AstMath/ODEIntegrator.hpp"

AST_NAMESPACE_BEGIN

errc_t Propagate::execute()
{
    auto inputState   = this->getInputState();        AST_CHECK_NULLPTR(inputState);
    auto outputState  = this->getOutputState();       AST_CHECK_NULLPTR(outputState);
    auto propagator   = this->propagator();           AST_CHECK_NULLPTR(propagator);
    auto integrator   = propagator->getIntegrator();  AST_CHECK_NULLPTR(integrator);
    
    // 获取初始状态
    State* orbitState = inputState->getOrbitState();  AST_CHECK_NULLPTR(orbitState);
    TimePoint startTime;
    errc_t rc = orbitState->getStateEpoch(startTime); AST_CHECK_ERRCODE(rc, "Failed to get state epoch");
    CartState cartState;
    rc = orbitState->getState(cartState);             AST_CHECK_ERRCODE(rc, "Failed to get cart state");
    
    TimePoint targetTime = startTime + maxPropTime();
    // 添加停止条件
    integrator->clearEventDetectors();
    for(auto& eventDetector: eventDetectors_)
    {
        propagator->getIntegrator()->addEventDetector(eventDetector->newODEEventDetector());
    }
    rc = propagator->propagate(startTime, targetTime, cartState.pos(), cartState.vel()); AST_CHECK_ERRCODE(rc, "Failed to propagate");
    // 保存结果
    // outputState->setStateEpoch(targetTime);
    // outputState->setState(cartState);
    // outputState->setOrbitElement(orbitState->getOrbitElement());
    // AST_CHECK_ERRCODE(rc, "Failed to set state");
    return 0;
}

AST_NAMESPACE_END
