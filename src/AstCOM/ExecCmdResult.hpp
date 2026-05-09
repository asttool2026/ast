///
/// @file      ExecCmdResult.hpp
/// @brief     命令执行结果集合类
/// @details   封装ExecuteCommand返回的字符串集合
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
#include <unknwn.h>
#include <oaidl.h>
#include <atlbase.h>
#include <atlcom.h>
#include <vector>
#include <string>

AST_NAMESPACE_BEGIN

/*!
    @addtogroup AstCOM
    @{
*/

class CExecCmdResult : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CExecCmdResult, &CLSID_CExecCmdResult>,
    public IDispatchImpl<IExecCmdResult, &IID_IExecCmdResult, &IID_NULL, 0xFFFF, 0xFFFF> 
    // 使用 &IID_NULL, 0xFFFF, 0xFFFF时，会直接从当前动态库加载TypeLib，避免了注册COM组件
{
public:
    BEGIN_COM_MAP(CExecCmdResult)
        COM_INTERFACE_ENTRY(IExecCmdResult)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT();

public:
    CExecCmdResult() : isSucceeded_(VARIANT_TRUE) {}
    ~CExecCmdResult() = default;

    void setSucceeded(VARIANT_BOOL value) { isSucceeded_ = value; }
    void addResult(const std::wstring& result) { results_.push_back(result); }
    void clearResults() { results_.clear(); }

public:
    HRESULT __stdcall get_Count(long* pVal) override;
    HRESULT __stdcall get_IsSucceeded(VARIANT_BOOL* pVal) override;
    HRESULT __stdcall get_Item(long Index, BSTR* pVal) override;
    HRESULT __stdcall Range(long StartIndex, long EndIndex, SAFEARRAY** pVal) override;

private:
    VARIANT_BOOL isSucceeded_;
    std::vector<std::wstring> results_;
};

/*! @} */

AST_NAMESPACE_END