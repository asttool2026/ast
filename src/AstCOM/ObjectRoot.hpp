///
/// @file      CObjectRoot.hpp
/// @brief     根对象Dispatch接口
/// @details   提供COM自动化接口，作为脚本访问的入口点
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
#include "ObjectRoot.h"
#include <unknwn.h>
#include <oaidl.h>
#include <atlbase.h>
#include <atlcom.h>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

/// @brief 根对象Dispatch接口
/// @details 作为COM自动化的根对象，提供类型创建和全局方法访问
class CObjectRoot : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CObjectRoot, &CLSID_CObjectRoot>,
    public IDispatchImpl<IObjectRoot, &IID_IObjectRoot, &IID_NULL, 0xFFFF, 0xFFFF>
{
public:
    // COM 映射表
    BEGIN_COM_MAP(CObjectRoot)
        COM_INTERFACE_ENTRY(IObjectRoot)       // 主接口
        COM_INTERFACE_ENTRY(IDispatch)         // 自动化接口
    END_COM_MAP()
    
    DECLARE_PROTECT_FINAL_CONSTRUCT();

public:
    /// @brief 获取单例实例
    /// @return CObjectRoot* 单例指针
    static CObjectRoot* Instance();

    CObjectRoot() = default;
    ~CObjectRoot() = default;


public:
    /// @brief 执行命令
    /// @param command 命令字符串
    /// @param result 执行结果字符串
    /// @return HRESULT 执行结果
    HRESULT __stdcall ExecuteCommand( 
        /* [in] */ BSTR command,
        /* [retval][out] */ BSTR *result
    ) override;
};

/*! @} */

AST_NAMESPACE_END