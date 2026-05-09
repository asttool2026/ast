///
/// @file      ObjectDispatch.hpp
/// @brief     对象Dispatch接口
/// @details   提供COM自动化接口，作为脚本访问对象的基础类
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
#include <unknwn.h>
#include <oaidl.h>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

/// @brief 对象Dispatch接口
/// @details 作为COM自动化的对象基类，提供属性和方法访问能力
class ObjectDispatch : public IDispatch
{
public:
    /// @brief 构造函数
    ObjectDispatch();
    
    /// @brief 析构函数
    ~ObjectDispatch();

    // IUnknown 接口方法
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

    // IDispatch 接口方法
    HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo) override;
    HRESULT __stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override;
    HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, 
                                    LCID lcid, DISPID* rgDispId) override;
    HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                            DISPPARAMS* pDispParams, VARIANT* pVarResult, 
                            EXCEPINFO* pExcepInfo, UINT* puArgErr) override;

protected:
    /// @brief 处理方法调用
    /// @param dispId 方法ID
    /// @param pDispParams 参数
    /// @param pVarResult 返回值
    /// @return HRESULT 调用结果
    HRESULT invokeMethod(DISPID dispId, DISPPARAMS* pDispParams, VARIANT* pVarResult);

    /// @brief 处理属性访问
    /// @param dispId 属性ID
    /// @param wFlags 访问标志
    /// @param pVarResult 属性值
    /// @return HRESULT 访问结果
    HRESULT invokeProperty(DISPID dispId, WORD wFlags, VARIANT* pVarResult);

private:
    volatile long refcnt_;      ///< 引用计数
};

/*! @} */

AST_NAMESPACE_END