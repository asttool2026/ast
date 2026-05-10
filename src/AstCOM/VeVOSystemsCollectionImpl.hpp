///
/// @file      VeVOSystemsCollectionImpl.hpp
/// @brief     VeVOSystemsCollection实现模板
/// @details   提供IVeVOSystemsCollection接口的模板实现
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
#include "AstCOM.h"
#include "AstCOM/COMAPI.hpp"
#include "AstCOM/VeVOSystemsElement.hpp"
#include "AstUtil/Object.hpp"
#include "AstUtil/Encode.hpp"
#include <unknwn.h>
#include <oaidl.h>
#include <atlbase.h>
#include <atlcom.h>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

template <typename T, const IID* piid = &__uuidof(T)>
class IVeVOSystemsCollectionImpl : 
    public IDispatchImpl<T, piid, &IID_NULL, 0xFFFF, 0xFFFF>
{
public:
    IVeVOSystemsCollectionImpl() = default;
    ~IVeVOSystemsCollectionImpl() = default;


    HRESULT __stdcall get_Count(long* pVal) override
    {
        if (pVal == nullptr)
            return E_POINTER;
        *pVal = 0;
        return S_OK;
    }

    HRESULT __stdcall get_Item(long Index, IVeVOSystemsElement** ppRetVal) override
    {
        if (ppRetVal == nullptr)
            return E_POINTER;
        *ppRetVal = nullptr;
        return E_NOTIMPL;
    }

    HRESULT __stdcall Add(BSTR name) override
    {
        if (name == nullptr)
            return E_POINTER;
        return S_OK;
    }

    HRESULT __stdcall Contains(BSTR name, VARIANT_BOOL* pVal) override
    {
        if (name == nullptr || pVal == nullptr)
            return E_POINTER;
        *pVal = VARIANT_FALSE;
        return S_OK;
    }

public:


private:

};

/*! @} */

AST_NAMESPACE_END