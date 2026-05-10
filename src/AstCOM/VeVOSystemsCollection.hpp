///
/// @file      VeVOSystemsCollection.hpp
/// @brief     VeVOSystemsCollection对象接口
/// @details   提供轨道系统集合对象的COM自动化接口
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
#include "VeVOSystemsCollectionImpl.hpp"

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

/// @brief VeVOSystemsCollection对象
/// @details 作为COM自动化的轨道系统集合对象
class CVeVOSystemsCollection :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CVeVOSystemsCollection, &CLSID_CVeVOSystemsCollection>,
    public IVeVOSystemsCollectionImpl<IVeVOSystemsCollection, &IID_IVeVOSystemsCollection> 
{
public:
    CVeVOSystemsCollection() = default;
    ~CVeVOSystemsCollection() = default;

    BEGIN_COM_MAP(CVeVOSystemsCollection)
        COM_INTERFACE_ENTRY(IVeVOSystemsCollection)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IUnknown)
    END_COM_MAP()
};

/*! @} */

AST_NAMESPACE_END