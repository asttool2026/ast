#include "ShooterControl.hpp"

// 自动生成的属性初始化代码
// 警告: 不要手动修改此文件

AST_NAMESPACE_BEGIN

Class ShooterControl::staticType;

static bool ShooterControl_ClassInited = (ShooterControl::ClassInit(&ShooterControl::staticType), true);

void ShooterControl::ClassInit(Class* cls)
{

    cls->setName("ShooterControl");
    cls->setDesc(u8R"(控制变量，用于定义微分修正中的控制变量)");
    cls->addToRegistry();
    cls->setParent<ObjectNamed>();
    cls->setConstructor<ShooterControl>();

}

AST_NAMESPACE_END