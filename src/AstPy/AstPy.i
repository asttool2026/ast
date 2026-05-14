%module AstPy

%begin %{
#define Py_LIMITED_API 0x03050000
%}

%{
#include "../../include/AstGlobal.h"
#include "AstUtil/Object.hpp"
#include "AstUtil/ObjectNamed.hpp"
#include "AstMath/Vector.hpp"
#include "AstCore/TimePoint.hpp"
#include "AstCore/CelestialBody.hpp"
#include "AstCore/SolarSystem.hpp"
#include "AstCore/GravityField.hpp"
#include "AstCore/CelestialBody.hpp"
#include "AstCore/SolarSystem.hpp"
%}

%include "std_vector.i"
%include "std_string.i"

%rename(__str__) *::toString;

%rename(__getitem__) *::operator();
%rename(__getitem__) *::operator[];


// %typemap(in) ast::StringView() {
//     const char *data = PyUnicode_AsUTF8($input);
//     Py_ssize_t len  = PyUnicode_GetLength($input);
//     if (!data) {
//         // 如果转换失败，data为NULL，需处理异常
//         SWIG_fail;
//     }
//     $1 = ast::StringView(data, len); 
// }


%include "../../include/AstCompiler.h"
%include "../../include/AstGlobal.h"
%include "../AstUtil/RTTI/Object.hpp"
%include "../AstUtil/RTTI/ObjectNamed.hpp"
%include "../AstUtil/String/StringView.hpp"

%include "../AstMath/Array/Vector.hpp"
%template(Vector3d) ast::VectorN<double, 3>;

%include "../AstCore/Time/TimePoint.hpp"

%include "../AstCore/Geometry/Euclid/Axes.hpp"
%include "../AstCore/Geometry/Euclid/Point.hpp"
%include "../AstCore/Geometry/Euclid/Frame.hpp"
%include "../AstCore/Geometry/Surface/BodyShape/BodyShape.hpp"
%include "../AstCore/Geometry/Surface/BodyShape/BodyShape.hpp"

%include "../AstCore/ForceModel/Gravity/GravityField.hpp"
%include "../AstCore/Data/JplDe.hpp"

%include "../AstCore/SolarSystem/Ephemeris/BodyEphemeris.hpp"
%include "../AstCore/SolarSystem/Orientation/BodyOrientation.hpp"
%import  "../AstCore/SolarSystem/SolarSystem.hpp"
%include "../AstCore/SolarSystem/CelestialBody.hpp"
%include "../AstCore/SolarSystem/SolarSystem.hpp"
%include "../AstCore/Data/Context/RunTimeSolarSystem.hpp"
%include "../AstCore/Data/Context/RunTime.hpp"



