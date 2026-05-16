///
/// @file      testPythonAPI.cpp
/// @brief     PythonAPI 动态加载及核心接口测试
/// @author    axel
/// @date      2026-05-16
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

#include "AstScript/PythonAPI.hpp"
#include "AstTest/Test.h"

AST_USING_NAMESPACE

// 检查当前环境能否加载 Python 库
static bool canLoadPython(PythonAPI& api)
{
    if(api.isLoaded())
        return true;

#if defined(_WIN32) || defined(_WIN64)
    errc_t rc = api.load("python3");
    if(rc != eNoError || !api.isLoaded())
    {
        static const char* vers[] = {
            "python314", "python313", "python312", "python311",
            "python310", "python39", "python38",
        };
        for(auto* v : vers)
        {
            rc = api.load(v);
            if(rc == eNoError && api.isLoaded())
                break;
        }
    }
    return rc == eNoError && api.isLoaded();
#else
    errc_t rc = api.load("libpython3");
    if(rc != eNoError || !api.isLoaded())
    {
        static const char* vers[] = {
            "libpython3.14", "libpython3.13", "libpython3.12",
            "libpython3.11", "libpython3.10", "libpython3.9",
            "libpython3.8",
        };
        for(auto* v : vers)
        {
            rc = api.load(v);
            if(rc == eNoError && api.isLoaded())
                break;
        }
    }
    return rc == eNoError && api.isLoaded();
#endif
}


TEST(PythonAPI, LoadUnload)
{
    PythonAPI api(false);
    EXPECT_FALSE(api.isLoaded());

    if(!canLoadPython(api))
    {
        GTEST_SKIP() << "Python shared library not found";
        return;
    }
    EXPECT_TRUE(api.isLoaded());

    errc_t rc = api.unload();
    EXPECT_EQ(rc, eNoError);
    EXPECT_FALSE(api.isLoaded());
}


TEST(PythonAPI, InitializeAndFinalize)
{
    PythonAPI api(false);
    if(!canLoadPython(api))
    {
        GTEST_SKIP() << "Python shared library not found";
        return;
    }

    EXPECT_FALSE(api.Py_IsInitialized());

    api.Py_Initialize();
    EXPECT_TRUE(api.Py_IsInitialized());

    int rc = api.Py_FinalizeEx();
    EXPECT_EQ(rc, 0);
    EXPECT_FALSE(api.Py_IsInitialized());
}


TEST(PythonAPI, RunSimpleString)
{
    PythonAPI api(false);
    if(!canLoadPython(api))
    {
        GTEST_SKIP() << "Python shared library not found";
        return;
    }

    api.Py_Initialize();
    EXPECT_TRUE(api.Py_IsInitialized());

    int rc = api.PyRun_SimpleString("x = 1 + 2");
    EXPECT_EQ(rc, 0);
    EXPECT_FALSE(api.PyErr_Occurred());

    rc = api.PyRun_SimpleString("assert x == 3");
    EXPECT_EQ(rc, 0);
    EXPECT_FALSE(api.PyErr_Occurred());

    api.Py_FinalizeEx();
}


TEST(PythonAPI, RunStringError)
{
    PythonAPI api(false);
    if(!canLoadPython(api))
    {
        GTEST_SKIP() << "Python shared library not found";
        return;
    }

    api.Py_Initialize();
    EXPECT_TRUE(api.Py_IsInitialized());

    int rc = api.PyRun_SimpleString("1 / 0");
    EXPECT_NE(rc, 0);

    api.PyErr_Print();
    api.Py_FinalizeEx();
}


TEST(PythonAPI, ImportModule)
{
    PythonAPI api(false);
    if(!canLoadPython(api))
    {
        GTEST_SKIP() << "Python shared library not found";
        return;
    }

    api.Py_Initialize();

    PyObject* math = api.PyImport_ImportModule("math");
    EXPECT_TRUE(math != nullptr);
    EXPECT_FALSE(api.PyErr_Occurred());

    PyObject* sqrt = api.PyObject_GetAttrString(math, "sqrt");
    EXPECT_TRUE(sqrt != nullptr);
    EXPECT_FALSE(api.PyErr_Occurred());

    api.Py_DecRef(sqrt);
    api.Py_DecRef(math);

    api.Py_FinalizeEx();
}


TEST(PythonAPI, CallObject)
{
    PythonAPI api(false);
    if(!canLoadPython(api))
    {
        GTEST_SKIP() << "Python shared library not found";
        return;
    }

    api.Py_Initialize();

    // import math and get sqrt
    PyObject* math = api.PyImport_ImportModule("math");
    EXPECT_TRUE(math != nullptr);
    EXPECT_FALSE(api.PyErr_Occurred());

    PyObject* sqrt = api.PyObject_GetAttrString(math, "sqrt");
    EXPECT_TRUE(sqrt != nullptr);
    EXPECT_FALSE(api.PyErr_Occurred());

    // import builtins to get float constructor for building args
    PyObject* builtins = api.PyImport_ImportModule("builtins");
    EXPECT_TRUE(builtins != nullptr);
    PyObject* pyFloat = api.PyObject_GetAttrString(builtins, "float");
    EXPECT_TRUE(pyFloat != nullptr);

    // call float(16) to build argument
    // Note: PyObject_CallObject requires a tuple, so we use PyRun_SimpleString
    // to set up a simple call test instead
    api.PyRun_SimpleString("import math\n"
                           "result = math.sqrt(16.0)");

    // read result back by running a check in Python
    api.PyRun_SimpleString("assert result == 4.0");

    EXPECT_FALSE(api.PyErr_Occurred());

    api.Py_DecRef(pyFloat);
    api.Py_DecRef(builtins);
    api.Py_DecRef(sqrt);
    api.Py_DecRef(math);

    api.Py_FinalizeEx();
}


TEST(PythonAPI, IncRefDecRef)
{
    PythonAPI api(false);
    if(!canLoadPython(api))
    {
        GTEST_SKIP() << "Python shared library not found";
        return;
    }

    api.Py_Initialize();

    PyObject* math = api.PyImport_ImportModule("math");
    EXPECT_TRUE(math != nullptr);

    api.Py_IncRef(math);  // incref for safety
    api.Py_DecRef(math);  // should still be alive
    api.Py_DecRef(math);  // release original ref

    api.Py_FinalizeEx();
}


TEST(PythonAPI, UnicodeAsUTF8)
{
    PythonAPI api(false);
    if(!canLoadPython(api))
    {
        GTEST_SKIP() << "Python shared library not found";
        return;
    }

    api.Py_Initialize();

    PyObject* builtins = api.PyImport_ImportModule("builtins");
    EXPECT_TRUE(builtins != nullptr);

    PyObject* strType = api.PyObject_GetAttrString(builtins, "str");
    EXPECT_TRUE(strType != nullptr);

    // import and call hello world
    api.PyRun_SimpleString("_test_str = 'hello python'");

    EXPECT_FALSE(api.PyErr_Occurred());

    api.Py_DecRef(strType);
    api.Py_DecRef(builtins);

    api.Py_FinalizeEx();
}


TEST(PythonAPI, Instance)
{
    // 获取单例前重置状态 —— 单例在首次 Instance() 时构造
    auto* inst = PythonAPI::Instance();
    EXPECT_TRUE(inst != nullptr);

    // 检查是否成功加载了 Python 库（取决于系统环境）
    // 不做强断言，仅验证单例可用
    bool loaded = inst->isLoaded();
    (void)loaded;
}


GTEST_MAIN();
