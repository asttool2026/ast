///
/// @file      ObjectImpl.hpp
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
#include "AstCOM.h"
#include "AstUtil/Object.hpp"
#include <unknwn.h>
#include <oaidl.h>
#include <atlbase.h>
#include <atlcom.h>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup 
    @{
*/

template <typename T, const IID* piid = &__uuidof(T)>
class IObjectImpl : 
    public IDispatchImpl<T, piid, &IID_NULL, 0xFFFF, 0xFFFF>
    // 使用 &IID_NULL, 0xFFFF, 0xFFFF时，会直接从当前动态库加载TypeLib，避免了注册COM组件
{
public: // 接口重写
    HRESULT __stdcall Children( 
    /* [in] */ BSTR name,
    /* [retval][out] */ IObject **ppRetVal) override
    {
        return E_NOTIMPL;
    }
public: // 辅助函数

    /// @brief 获取原始对象指针
    Object* GetNativeObject()
    {
        return object_.get();
    }
    
    /// @brief 设置原始对象指针
    void SetNativeObject(Object* object)
    {
        object_ = object;
    }

    /// @brief 获取原始对象指针
    template<typename U>
    U GetNative()
    {
        return aobject_cast<U>(object_.get());
    }
private:
    WeakPtr<Object> object_;        ///< 原始对象指针
};


/*! @} */

AST_NAMESPACE_END
