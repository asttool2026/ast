///
/// @file      ObjectLinkTo.cpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-05-10
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

#include "ObjectLinkTo.hpp"
#include "AstUtil/RTTIAPI.hpp"

AST_NAMESPACE_BEGIN

void ObjectLinkTo::setResolvedName(const std::string& name)
{
    resolvedName_ = name;
    resolvedObject_.reset();
}

const std::string& ObjectLinkTo::getResolvedName() const
{
    if(resolvedObject_.get())
        return resolvedObject_->getName();
    return resolvedName_;
}


void ObjectLinkTo::setResolvedType(Class* type)
{
    if(type == resolvedType_)
        return;
    resolvedType_ = type;
    resolvedObject_.reset();
}

Class* ObjectLinkTo::getResolvedType() const
{
    if(resolvedObject_.get())
        return resolvedObject_->getType();
    return resolvedType_;
}

Object* ObjectLinkTo::resolve() const
{
    if(auto object = resolvedObject_.get())
        return object;
    auto object = aResolveObject(getResolvedName(), getResolvedType());
    resolvedObject_ = object;
    return object;
}

AST_NAMESPACE_END

