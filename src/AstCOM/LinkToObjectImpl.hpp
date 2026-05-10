///
/// @file      LinkToObjectImpl.hpp
/// @brief     链接对象实现模板
/// @details   提供ILinkToObject接口的模板实现
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
#include "AstSim/SpaceObject.hpp"
#include "AstUtil/Object.hpp"
#include "AstUtil/Encode.hpp"
#include "AstUtil/RTTIAPI.hpp"
#include <unknwn.h>
#include <oaidl.h>
#include <atlbase.h>
#include <atlcom.h>
#include <vector>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

template <typename T, const IID* piid = &__uuidof(T)>
class ILinkToObjectImpl : 
    public IDispatchImpl<T, piid, &IID_NULL, 0xFFFF, 0xFFFF>
{
public:
    ILinkToObjectImpl() = default;
    ~ILinkToObjectImpl() = default;

    HRESULT __stdcall BindTo(BSTR objectPath) override
    {
        if (objectPath == nullptr)
            return E_POINTER;
        
        std::string path = aWideToUtf8(objectPath);
        aSpaceObject_SetReferenceVehicle(spaceObject_.get(), path);
        return S_OK;
    }

    HRESULT __stdcall get_AvailableObjects(SAFEARRAY** pVal) override
    {
        return E_NOTIMPL;
    }

    HRESULT __stdcall get_IsIntrinsic(VARIANT_BOOL* pVal) override
    {
        return E_NOTIMPL;
    }

    HRESULT __stdcall get_LinkedObject(IObject** ppRetVal) override
    {
        if (ppRetVal == nullptr)
            return E_POINTER;
        
        *ppRetVal = nullptr;
        
        auto referenceSpaceObject = aSpaceObject_GetReferenceVehicle(spaceObject_.get());
        auto comObject = aComGetObject(referenceSpaceObject);
        if (comObject)
        {
            comObject->AddRef();
            *ppRetVal = comObject;
        }
        return S_OK;
    }

    HRESULT __stdcall get_Name(BSTR* pVal) override
    {
        if (pVal == nullptr)
            return E_POINTER;
        
        *pVal = nullptr;
        auto referenceSpaceObject = aSpaceObject_GetReferenceVehicle(spaceObject_.get());
        
        if (referenceSpaceObject)
        {
            std::wstring name = aUtf8ToWide(referenceSpaceObject->getName());
            *pVal = SysAllocString(name.c_str());
        }else{
            *pVal = SysAllocString(L"");
        }
        
        return S_OK;
    }

public:
    void SetNativeParentObject(SpaceObject* object)
    {
        spaceObject_ = object;
    }

private:
    WeakPtr<SpaceObject> spaceObject_;
};

/*! @} */

AST_NAMESPACE_END