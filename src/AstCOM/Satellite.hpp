///
/// @file      Satellite.hpp
/// @brief     卫星对象接口
/// @details   提供卫星对象的COM自动化接口
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
#include "SatelliteImpl.hpp"

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

class Satellite;

/// @brief 卫星对象
/// @details 作为COM自动化的卫星对象，继承自IObject
class CSatellite :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CSatellite, &CLSID_CSatellite>,
    public ISatelliteImpl<ISatellite, &IID_ISatellite> 
{
public:
    CSatellite() = default;
    ~CSatellite() = default;

    BEGIN_COM_MAP(CSatellite)
        COM_INTERFACE_ENTRY(ISatellite)
        COM_INTERFACE_ENTRY(IObject)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IUnknown)
    END_COM_MAP()

};

/*! @} */

AST_NAMESPACE_END