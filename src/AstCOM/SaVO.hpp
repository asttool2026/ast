///
/// @file      SaVO.hpp
/// @brief     SaVO对象接口
/// @details   提供卫星飞行器对象的COM自动化接口
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
#include "SaVOImpl.hpp"

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

/// @brief SaVO对象
/// @details 作为COM自动化的卫星飞行器对象
class CSaVO :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CSaVO, &CLSID_CSaVO>,
    public ISaVOImpl<ISaVO, &IID_ISaVO> 
{
public:
    CSaVO() = default;
    ~CSaVO() = default;

    BEGIN_COM_MAP(CSaVO)
        COM_INTERFACE_ENTRY(ISaVO)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IUnknown)
    END_COM_MAP()
};

/*! @} */

AST_NAMESPACE_END