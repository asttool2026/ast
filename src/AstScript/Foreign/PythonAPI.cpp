///
/// @file      PythonAPI.cpp
/// @brief     Python C API 动态加载封装实现
/// @details   参考 SpiceAPI 模式，动态加载 Python 共享库
/// @author    axel
/// @date      2026-05-16
/// @copyright 版权所有 (C) 2026-present, ast项目.
///
/// SpaceAST项目（https://github.com/space-ast/ast）
/// 本项目基于 Apache 2.0 开源许可证分发。

#include "PythonAPI.hpp"
#include "AstUtil/StringView.hpp"
#include "AstUtil/LibraryLoader.hpp"
#include "AstUtil/Logger.hpp"

AST_NAMESPACE_BEGIN

// Python C API 函数原型声明（用于 decltype 推导函数指针类型）
namespace pythonproto{

void        Py_Initialize(void);
int         Py_FinalizeEx(void);
int         Py_IsInitialized(void);
int         PyRun_SimpleString(const char* command);
PyObject*   PyImport_ImportModule(const char* name);
PyObject*   PyObject_GetAttrString(PyObject* o, const char* attr_name);
PyObject*   PyObject_CallObject(PyObject* callable, PyObject* args);
void        Py_DecRef(PyObject* o);
void        Py_IncRef(PyObject* o);
void        PyErr_Print(void);
PyObject*   PyErr_Occurred(void);
const char* PyUnicode_AsUTF8(PyObject* unicode);

} // namespace pythonproto


static int countLoadedFuncs(const PythonAPI::funcarray& funcs)
{
    int count = 0;
    for(auto& f : funcs)
        if(f) count++;
    return count;
}


PythonAPI* PythonAPI::Instance()
{
    static PythonAPI instance_{true};
    return &instance_;
}


PythonAPI::PythonAPI(bool shouldLoadDynamicLib)
{
    if(shouldLoadDynamicLib)
    {
#if defined(_WIN32) || defined(_WIN64)
        if(load("python3") != eNoError)
        {
            static const char* vers[] = {
                "python314", "python313", "python312", "python311",
                "python310", "python39", "python38",
            };
            for(auto* v : vers)
                if(load(v) == eNoError)
                    break;
        }
#else
        static const char* vers[] = {
            "libpython3.14", "libpython3.13", "libpython3.12",
            "libpython3.11", "libpython3.10", "libpython3.9",
            "libpython3.8", "libpython3",
        };
        for(auto* v : vers)
            if(load(v) == eNoError)
                break;
#endif
    }
}


PythonAPI::~PythonAPI()
{
    if(Py_IsInitialized())
        Py_FinalizeEx();
    unload();
}


errc_t PythonAPI::load(StringView dirpath)
{
    void* lib = aLoadLibrary(std::string(dirpath).c_str());
    if(!lib)
        return eErrorInvalidFile;

    funcarray funcs{};
    funcs[iPy_Initialize]          = aGetProcAddress(lib, "Py_Initialize");
    funcs[iPy_FinalizeEx]          = aGetProcAddress(lib, "Py_FinalizeEx");
    funcs[iPy_IsInitialized]       = aGetProcAddress(lib, "Py_IsInitialized");
    funcs[iPyRun_SimpleString]     = aGetProcAddress(lib, "PyRun_SimpleString");
    funcs[iPyImport_ImportModule]  = aGetProcAddress(lib, "PyImport_ImportModule");
    funcs[iPyObject_GetAttrString] = aGetProcAddress(lib, "PyObject_GetAttrString");
    funcs[iPyObject_CallObject]    = aGetProcAddress(lib, "PyObject_CallObject");
    funcs[iPy_DecRef]              = aGetProcAddress(lib, "Py_DecRef");
    funcs[iPy_IncRef]              = aGetProcAddress(lib, "Py_IncRef");
    funcs[iPyErr_Print]            = aGetProcAddress(lib, "PyErr_Print");
    funcs[iPyErr_Occurred]         = aGetProcAddress(lib, "PyErr_Occurred");
    funcs[iPyUnicode_AsUTF8]       = aGetProcAddress(lib, "PyUnicode_AsUTF8");

    int n = countLoadedFuncs(funcs);
    if(n < numfunctions)
    {
        aError("PythonAPI: expected %d functions, loaded %d", numfunctions, n);
        aFreeLibrary(lib);
        return eErrorInvalidFile;
    }
    library_ = lib;
    functions_ = funcs;
    return eNoError;
}


errc_t PythonAPI::tryload(const std::vector<std::string>& libpaths)
{
    for(auto& p : libpaths)
        if(load(p) == eNoError)
            return eNoError;
    return eErrorInvalidFile;
}


errc_t PythonAPI::unload()
{
    if(library_)
    {
        aFreeLibrary(library_);
        library_ = nullptr;
    }
    functions_ = funcarray{};
    return eNoError;
}


#define PYTHONAPI_LOCK    std::lock_guard<std::mutex> lk(mutex_)
#define PYTHONAPI_GET(fn, idx) \
    auto fn = reinterpret_cast<decltype(&pythonproto::idx)>(functions_[i##idx]); \
    if(!fn){ aError("PythonAPI: library not loaded"); return; }
#define PYTHONAPI_GET_RET(fn, idx, ret) \
    auto fn = reinterpret_cast<decltype(&pythonproto::idx)>(functions_[i##idx]); \
    if(!fn){ aError("PythonAPI: library not loaded"); return ret; }


void PythonAPI::Py_Initialize()
{
    PYTHONAPI_GET(fn, Py_Initialize);
    PYTHONAPI_LOCK;
    fn();
    checkerror();
}


int PythonAPI::Py_FinalizeEx()
{
    PYTHONAPI_GET_RET(fn, Py_FinalizeEx, -1);
    PYTHONAPI_LOCK;
    int rc = fn();
    return rc;
}


int PythonAPI::Py_IsInitialized()
{
    PYTHONAPI_GET_RET(fn, Py_IsInitialized, 0);
    return fn();
}


int PythonAPI::PyRun_SimpleString(const char* command)
{
    PYTHONAPI_GET_RET(fn, PyRun_SimpleString, -1);
    PYTHONAPI_LOCK;
    int rc = fn(command);
    if(rc != 0) checkerror();
    return rc;
}


PyObject* PythonAPI::PyImport_ImportModule(const char* name)
{
    PYTHONAPI_GET_RET(fn, PyImport_ImportModule, nullptr);
    PYTHONAPI_LOCK;
    PyObject* r = fn(name);
    checkerror();
    return r;
}


PyObject* PythonAPI::PyObject_GetAttrString(PyObject* o, const char* attr_name)
{
    PYTHONAPI_GET_RET(fn, PyObject_GetAttrString, nullptr);
    PYTHONAPI_LOCK;
    PyObject* r = fn(o, attr_name);
    checkerror();
    return r;
}


PyObject* PythonAPI::PyObject_CallObject(PyObject* callable, PyObject* args)
{
    PYTHONAPI_GET_RET(fn, PyObject_CallObject, nullptr);
    PYTHONAPI_LOCK;
    PyObject* r = fn(callable, args);
    checkerror();
    return r;
}


void PythonAPI::Py_DecRef(PyObject* o)
{
    PYTHONAPI_GET(fn, Py_DecRef);
    fn(o);
}


void PythonAPI::Py_IncRef(PyObject* o)
{
    PYTHONAPI_GET(fn, Py_IncRef);
    fn(o);
}


void PythonAPI::PyErr_Print()
{
    PYTHONAPI_GET(fn, PyErr_Print);
    fn();
}


PyObject* PythonAPI::PyErr_Occurred()
{
    PYTHONAPI_GET_RET(fn, PyErr_Occurred, nullptr);
    return fn();
}


const char* PythonAPI::PyUnicode_AsUTF8(PyObject* unicode)
{
    PYTHONAPI_GET_RET(fn, PyUnicode_AsUTF8, nullptr);
    return fn(unicode);
}


void PythonAPI::checkerror()
{
    if(PyErr_Occurred())
        PyErr_Print();
}


AST_NAMESPACE_END
