///
/// @file      PythonAPI.hpp
/// @brief     Python C API 动态加载封装
/// @details   参考 SpiceAPI 模式，动态加载 Python 共享库并提供核心 C API 的包装。
///            公开方法名和签名与 Python C API 保持严格一致。
/// @author    axel
/// @date      2026-05-16
/// @copyright 版权所有 (C) 2026-present, ast项目.
///
/// SpaceAST项目（https://github.com/space-ast/ast）
/// 本项目基于 Apache 2.0 开源许可证分发。

#pragma once

#include "AstGlobal.h"
#include <mutex>
#include <vector>
#include <string>
#include <array>


// Python 类型前置声明（不依赖 Python.h，保证动态加载场景下的编译独立性）
struct _object;
typedef struct _object PyObject;


AST_NAMESPACE_BEGIN


/*!
    @addtogroup Script
    @{
*/

/// @brief Python API — 与 Python C API 接口一致的动态加载包装
/// @details 通过动态加载 Python 共享库的方式提供 Python C API 的核心函数。
/// 公开方法名和签名与 Python C API 严格一致，但底层为运行时动态解析。
class AST_SCRIPT_API PythonAPI
{
public:
    // 函数索引
    enum{
        iPy_Initialize = 0,
        iPy_FinalizeEx,
        iPy_IsInitialized,
        iPyRun_SimpleString,
        iPyImport_ImportModule,
        iPyObject_GetAttrString,
        iPyObject_CallObject,
        iPy_DecRef,
        iPy_IncRef,
        iPyErr_Print,
        iPyErr_Occurred,
        iPyUnicode_AsUTF8,
        numfunctions,
    };
    using funcarray = std::array<void*, numfunctions>;

    /// @brief 获取单例实例
    static PythonAPI* Instance();

    PythonAPI() = default;

    explicit PythonAPI(bool shouldLoadDynamicLib);

    ~PythonAPI();

    /// @brief 加载指定路径的Python动态库
    errc_t load(StringView libpath);

    /// @brief 尝试加载库，按顺序尝试直到成功
    errc_t tryload(const std::vector<std::string>& libpaths);

    /// @brief 卸载库
    errc_t unload();

    /// @brief 检查是否已加载库
    bool isLoaded() const { return library_ != nullptr; }

public: // -- 以下接口名称和签名与 Python C API 完全一致 --

    void Py_Initialize();

    int Py_FinalizeEx();

    int Py_IsInitialized();

    int PyRun_SimpleString(const char* command);

    PyObject* PyImport_ImportModule(const char* name);

    PyObject* PyObject_GetAttrString(PyObject* o, const char* attr_name);

    PyObject* PyObject_CallObject(PyObject* callable, PyObject* args);

    void Py_DecRef(PyObject* o);

    void Py_IncRef(PyObject* o);

    void PyErr_Print();

    PyObject* PyErr_Occurred();

    const char* PyUnicode_AsUTF8(PyObject* unicode);

protected:
    void checkerror();
    A_DISABLE_COPY(PythonAPI);

protected:
    void*  library_{nullptr};
    funcarray functions_{};
    std::mutex mutex_;
};

/*! @} */

AST_NAMESPACE_END
