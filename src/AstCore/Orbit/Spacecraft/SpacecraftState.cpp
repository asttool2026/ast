///
/// @file      SpacecraftState.cpp
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

#include "SpacecraftState.hpp"
#include "AstCore/State.hpp"

AST_NAMESPACE_BEGIN

State* SpacecraftState::getOrbitState() const
{
    if(!orbitState_)
        const_cast<SpacecraftState*>(this)->orbitState_ = State::NewDefault();
    return orbitState_;
}

errc_t SpacecraftState::getState(ModOrbElem& orbElem) const
{
    return getOrbitState()->getState(orbElem);
}

errc_t SpacecraftState::getState(CartState& state) const
{
    return getOrbitState()->getState(state);
}

errc_t SpacecraftState::setState(const ModOrbElem& orbElem)
{
    return getOrbitState()->setState(orbElem);
}

errc_t SpacecraftState::setState(const CartState& state)
{
    return getOrbitState()->setState(state);
}

void SpacecraftState::setStateEpoch(const TimePoint& stateEpoch)
{
    return getOrbitState()->setStateEpoch(stateEpoch);
}


SpacecraftState* SpacecraftState::NewDefault()
{
    auto scState = new SpacecraftState();
    scState->setOrbitState(State::NewDefault());
    return scState;
}

AST_NAMESPACE_END
