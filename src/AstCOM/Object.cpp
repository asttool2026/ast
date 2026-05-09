///
/// @file      CObject.cpp
/// @brief     对象Dispatch接口实现
/// @details   COM自动化接口的具体实现，作为脚本访问对象的基础类
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

#include "AstCOM/Object.hpp"

AST_NAMESPACE_BEGIN

CObject::CObject()
    : refcnt_(1)
{
}

CObject::~CObject()
{
}

HRESULT __stdcall CObject::QueryInterface(REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr)
        return E_POINTER;

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IDispatch)
    {
        *ppvObject = static_cast<IDispatch*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

ULONG __stdcall CObject::AddRef()
{
    return InterlockedIncrement(&refcnt_);
}

ULONG __stdcall CObject::Release()
{
    ULONG ref = InterlockedDecrement(&refcnt_);
    if (ref == 0)
    {
        delete this;
    }
    return ref;
}

HRESULT __stdcall CObject::GetTypeInfoCount(UINT* pctinfo)
{
    if (pctinfo == nullptr)
        return E_POINTER;
    
    *pctinfo = 0;
    return S_OK;
}

HRESULT __stdcall CObject::GetTypeInfo(UINT /*iTInfo*/, LCID /*lcid*/, ITypeInfo** ppTInfo)
{
    if (ppTInfo == nullptr)
        return E_POINTER;
    
    *ppTInfo = nullptr;
    return E_NOTIMPL;
}

HRESULT __stdcall CObject::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, 
                                                LCID /*lcid*/, DISPID* rgDispId)
{
    if (rgszNames == nullptr || rgDispId == nullptr)
        return E_POINTER;

    for (UINT i = 0; i < cNames; ++i)
    {
        rgDispId[i] = DISPID_UNKNOWN;
    }

    return S_OK;
}

HRESULT __stdcall CObject::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID /*lcid*/, 
                                        WORD wFlags, DISPPARAMS* pDispParams, 
                                        VARIANT* pVarResult, EXCEPINFO* /*pExcepInfo*/, 
                                        UINT* /*puArgErr*/)
{
    if (pDispParams == nullptr)
        return E_POINTER;

    if (pVarResult != nullptr)
        VariantInit(pVarResult);

    if (wFlags & DISPATCH_METHOD)
    {
        return invokeMethod(dispIdMember, pDispParams, pVarResult);
    }
    else if ((wFlags & DISPATCH_PROPERTYGET) || (wFlags & DISPATCH_PROPERTYPUT) || 
             (wFlags & DISPATCH_PROPERTYPUTREF))
    {
        return invokeProperty(dispIdMember, wFlags, pVarResult);
    }

    return E_INVALIDARG;
}

HRESULT CObject::invokeMethod(DISPID /*dispId*/, DISPPARAMS* /*pDispParams*/, VARIANT* /*pVarResult*/)
{
    return DISP_E_MEMBERNOTFOUND;
}

HRESULT CObject::invokeProperty(DISPID /*dispId*/, WORD /*wFlags*/, VARIANT* /*pVarResult*/)
{
    return DISP_E_MEMBERNOTFOUND;
}

AST_NAMESPACE_END