#include "AstGlobal.h"
#include "AstCore/ThirdBodyForce.hpp"
#include "AstGUI/UiThirdBodyForce.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 创建三体引力模型对象
    auto thirdBodyForce = new ast::ThirdBodyForce();
    
    // 创建界面
    ast::UiThirdBodyForce ui(thirdBodyForce);
    ui.show();
    
    int result = app.exec();
    
    // 清理内存
    delete thirdBodyForce;
    
    return result;
}