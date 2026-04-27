#include "ODEVarStepIntegrator.hpp"

// 自动生成的属性初始化代码
// 警告: 不要手动修改此文件

AST_NAMESPACE_BEGIN

Class ODEVarStepIntegrator::staticType;

static bool ODEVarStepIntegrator_ClassInited = (ODEVarStepIntegrator::ClassInit(&ODEVarStepIntegrator::staticType), true);

void ODEVarStepIntegrator::ClassInit(Class* cls)
{

    cls->setName("ODEVarStepIntegrator");
    cls->addToRegistry();
    cls->setParent<ODEFixedStepIntegrator>();
    cls->setConstructor<ODEVarStepIntegrator>();

    constexpr auto member_useMinStep = &ODEVarStepIntegrator::useMinStep_;
    cls->addProperty("useMinStep", aNewPropertyBool<ODEVarStepIntegrator, member_useMinStep>());
    constexpr auto member_useMaxStep = &ODEVarStepIntegrator::useMaxStep_;
    cls->addProperty("useMaxStep", aNewPropertyBool<ODEVarStepIntegrator, member_useMaxStep>());
    constexpr auto member_warnOnMinStep = &ODEVarStepIntegrator::warnOnMinStep_;
    cls->addProperty("warnOnMinStep", aNewPropertyBool<ODEVarStepIntegrator, member_warnOnMinStep>());
    constexpr auto member_maxStepAttempts = &ODEVarStepIntegrator::maxStepAttempts_;
    cls->addProperty("maxStepAttempts", aNewPropertyInt<ODEVarStepIntegrator, member_maxStepAttempts>());
    constexpr auto member_minStepSize = &ODEVarStepIntegrator::minStepSize_;
    cls->addProperty("minStepSize", aNewPropertyDouble<ODEVarStepIntegrator, member_minStepSize>());
    constexpr auto member_maxStepSize = &ODEVarStepIntegrator::maxStepSize_;
    cls->addProperty("maxStepSize", aNewPropertyDouble<ODEVarStepIntegrator, member_maxStepSize>());
    constexpr auto member_maxAbsErr = &ODEVarStepIntegrator::maxAbsErr_;
    cls->addProperty("maxAbsErr", aNewPropertyDouble<ODEVarStepIntegrator, member_maxAbsErr>());
    constexpr auto member_maxRelErr = &ODEVarStepIntegrator::maxRelErr_;
    cls->addProperty("maxRelErr", aNewPropertyDouble<ODEVarStepIntegrator, member_maxRelErr>());
    constexpr auto member_minStepScaleFactor = &ODEVarStepIntegrator::minStepScaleFactor_;
    cls->addProperty("minStepScaleFactor", aNewPropertyDouble<ODEVarStepIntegrator, member_minStepScaleFactor>());
    constexpr auto member_maxStepScaleFactor = &ODEVarStepIntegrator::maxStepScaleFactor_;
    cls->addProperty("maxStepScaleFactor", aNewPropertyDouble<ODEVarStepIntegrator, member_maxStepScaleFactor>());
    constexpr auto member_safetyCoeffLow = &ODEVarStepIntegrator::safetyCoeffLow_;
    cls->addProperty("safetyCoeffLow", aNewPropertyDouble<ODEVarStepIntegrator, member_safetyCoeffLow>());
    constexpr auto member_safetyCoeffHigh = &ODEVarStepIntegrator::safetyCoeffHigh_;
    cls->addProperty("safetyCoeffHigh", aNewPropertyDouble<ODEVarStepIntegrator, member_safetyCoeffHigh>());
    constexpr auto member_errCtrPowthLow = &ODEVarStepIntegrator::errCtrPowthLow_;
    cls->addProperty("errCtrPowthLow", aNewPropertyDouble<ODEVarStepIntegrator, member_errCtrPowthLow>());
    constexpr auto member_errCtrPowthHigh = &ODEVarStepIntegrator::errCtrPowthHigh_;
    cls->addProperty("errCtrPowthHigh", aNewPropertyDouble<ODEVarStepIntegrator, member_errCtrPowthHigh>());
}

AST_NAMESPACE_END