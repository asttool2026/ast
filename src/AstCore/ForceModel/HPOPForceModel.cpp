///
/// @file      HPOPForceModel.cpp
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

#include "HPOPForceModel.hpp"

AST_NAMESPACE_BEGIN

HPOPForceModel::HPOPForceModel() = default;
HPOPForceModel::~HPOPForceModel() = default;

// HPOPForceModel::HPOPForceModel(HPOPForceModel&& other) = default;
// HPOPForceModel& HPOPForceModel::operator=(HPOPForceModel&& other) = default;

GravityForce& HPOPForceModel::gravity()
{
    if(!bodyAttraction_ || !bodyAttraction_->isAttractionType(EBodyAttractionType::eGravity))
    {
        bodyAttraction_ = new GravityForce();
    }
    return static_cast<GravityForce&>(*bodyAttraction_);
}

TwoBodyForce& HPOPForceModel::twoBody()
{
    if(!bodyAttraction_ || !bodyAttraction_->isAttractionType(EBodyAttractionType::eTwoBody))
    {
        bodyAttraction_ = new TwoBodyForce();
    }
    return static_cast<TwoBodyForce&>(*bodyAttraction_);
}

BodyAttraction& HPOPForceModel::bodyAttraction()
{
    if (!bodyAttraction_)
        bodyAttraction_ = new GravityForce();
    return *bodyAttraction_;
}

const BodyAttraction& HPOPForceModel::bodyAttraction() const
{
    return const_cast<HPOPForceModel*>(this)->bodyAttraction();
}

AST_NAMESPACE_END
