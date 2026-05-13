///
/// @file      LinkToObject.hpp
/// @brief     链接对象接口
/// @details   提供链接对象的COM自动化接口
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

#pragma once

#include "AstGlobal.h"
#include "LinkToObjectImpl.hpp"

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

/// @brief 链接对象
/// @details 作为COM自动化的链接对象，用于链接到其他对象
class CLinkToObject :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CLinkToObject, &CLSID_CLinkToObject>,
    public ILinkToObjectImpl<ILinkToObject, &IID_ILinkToObject> 
{
public:
    CLinkToObject() = default;
    ~CLinkToObject() = default;

    BEGIN_COM_MAP(CLinkToObject)
        COM_INTERFACE_ENTRY(ILinkToObject)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IUnknown)
    END_COM_MAP()
};

/*! @} */

AST_NAMESPACE_END