#include "DifferentialCorrectorProfile.hpp"

// 自动生成的属性初始化代码
// 警告: 不要手动修改此文件

AST_NAMESPACE_BEGIN

Class DifferentialCorrectorProfile::staticType;

static bool DifferentialCorrectorProfile_ClassInited = (DifferentialCorrectorProfile::ClassInit(&DifferentialCorrectorProfile::staticType), true);

void DifferentialCorrectorProfile::ClassInit(Class* cls)
{

    cls->setName("DifferentialCorrectorProfile");
    cls->setDesc(u8R"(微分修正器配置器，用于求解非线性方程，可以用于修正轨道参数以满足约束条件要求)");
    cls->addToRegistry();
    cls->setParent<TargeterProfile>();
    cls->setConstructor<DifferentialCorrectorProfile>();

}

AST_NAMESPACE_END