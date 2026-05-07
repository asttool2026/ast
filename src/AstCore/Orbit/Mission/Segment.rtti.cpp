#include "Segment.hpp"

// 自动生成的属性初始化代码
// 警告: 不要手动修改此文件

AST_NAMESPACE_BEGIN

Class Segment::staticType;

static bool Segment_ClassInited = (Segment::ClassInit(&Segment::staticType), true);

void Segment::ClassInit(Class* cls)
{

    cls->setName("Segment");
    cls->setDesc(u8R"(轨道段（虚基类），用于描述任务序列中的轨道段，例如初始状态段、轨道机动段、轨道预报段等)");
    cls->addToRegistry();
    cls->setParent<MissionCommand>();
    cls->setConstructor<Segment>();

    cls->addProperty("InitialState", aNewPropertyObject<Segment, SpacecraftState, &Segment::getInitialState>());
    cls->addProperty("FinalState", aNewPropertyObject<Segment, SpacecraftState, &Segment::getFinalState>());
}

AST_NAMESPACE_END