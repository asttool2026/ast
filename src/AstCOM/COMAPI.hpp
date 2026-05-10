///
/// @file      COMAPI.hpp
/// @brief     
/// @details   
/// @author    axel
/// @date      2026-05-09
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

struct IUnknown;
struct IDispatch;
struct IObject;

AST_NAMESPACE_BEGIN

/*!
    @addtogroup 
    @{
*/

/// @brief 获取根对象的COM接口
/// @details 用于获取根对象的COM接口，用于调用根对象的方法
/// @return IUnknown* 根对象的COM接口指针
/// @note 调用者负责释放返回的COM接口指针
/// @sa CObjectRoot
AST_COM_CAPI IDispatch* aComObjectRoot();


/// @brief 获取对象的COM接口
AST_COM_CAPI IObject* aComGetObject(Object* obj);


/*! @} */

AST_NAMESPACE_END
