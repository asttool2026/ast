///
/// @file      CObjectRoot.cpp
/// @brief     根对象Dispatch接口实现
/// @details   COM自动化接口的具体实现
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

#include "AstCOM/ObjectRoot.hpp"
#include "AstCOM/ExecCmdResult.hpp"
#include <cassert>
#include <stdio.h>

AST_NAMESPACE_BEGIN

CObjectRoot* CObjectRoot::Instance()
{
    static CComObject<CObjectRoot>* pInstance = nullptr;

    if (pInstance == nullptr)
    {
        HRESULT hr = CComObject<CObjectRoot>::CreateInstance(&pInstance);
        if (FAILED(hr))
        {
            return nullptr;
        }

        pInstance->AddRef();
    }

    return pInstance;
}


HRESULT __stdcall CObjectRoot::ExecuteCommand( 
    /* [in] */ BSTR command,
    /* [retval][out] */ IExecCmdResult **result
)
{
    if (!result)
        return E_POINTER;
    
    *result = nullptr;
    
    wprintf(L"ExecuteCommand: '%s'\n", command);
    
    CComObject<CExecCmdResult>* pResult = nullptr;
    HRESULT hr = CComObject<CExecCmdResult>::CreateInstance(&pResult);
    
    if (FAILED(hr))
        return hr;
    
    pResult->AddRef();
    pResult->setSucceeded(VARIANT_TRUE);
    pResult->addResult(L"hello world");
    pResult->addResult(L"command executed successfully");
    
    *result = pResult;
    return S_OK;
}

AST_NAMESPACE_END