#include "UiThirdBodyForce.hpp"
#include "AstCore/ThirdBodyForce.hpp"
#include "AstCore/CelestialBody.hpp"
#include <QGroupBox>
#include <QTableWidget>
#include <QHeaderView>

AST_NAMESPACE_BEGIN

UiThirdBodyForce::UiThirdBodyForce(Object* object, QWidget *parent)
    : UiObject(parent)
{
    setObjectName("UiThirdBodyForce");
    setupUi();
    if (object) {
        setObject(object);
        setThirdBodyForce(dynamic_cast<ThirdBodyForce*>(object));
    }
}

UiThirdBodyForce::UiThirdBodyForce(QWidget *parent)
    : UiObject(parent)
{
    setObjectName("UiThirdBodyForce");
    setupUi();
}

void UiThirdBodyForce::setupUi()
{
    // 主布局
    mainLayout_ = new QVBoxLayout(this);
    
    // 三体引力配置
    thirdBodyGroup_ = new QGroupBox("三体引力", this);
    thirdBodyLayout_ = new QVBoxLayout(thirdBodyGroup_);
    
    thirdBodyTable_ = new QTableWidget(this);
    thirdBodyTable_->setColumnCount(4);
    thirdBodyTable_->setHorizontalHeaderLabels({"名称", "使用", "引力模型", "星历来源"});
    thirdBodyTable_->horizontalHeader()->setStretchLastSection(true);
    
    thirdBodyLayout_->addWidget(thirdBodyTable_);
    mainLayout_->addWidget(thirdBodyGroup_);
    mainLayout_->addStretch();
    
    // 连接信号槽
    connect(thirdBodyTable_, &QTableWidget::cellChanged, this, &UiThirdBodyForce::onBodySelectionChanged);
    
    // 刷新UI
    refreshCelestialBodies();
}

ThirdBodyForce* UiThirdBodyForce::getThirdBodyForce() const
{
    return thirdBodyForce_;
}

void UiThirdBodyForce::setThirdBodyForce(ThirdBodyForce* thirdBody)
{
    thirdBodyForce_ = thirdBody;
    refreshUi();
}

void UiThirdBodyForce::refreshUi()
{
    if (!thirdBodyForce_) return;
    
    // 这里需要根据实际的ThirdBodyForce类结构添加相应的刷新逻辑
}

void UiThirdBodyForce::apply()
{
    if (thirdBodyForce_) {
        applyTo(thirdBodyForce_);
    }
}

void UiThirdBodyForce::applyTo(ThirdBodyForce* thirdBody)
{
    if (!thirdBody) return;
    
    // 这里需要根据实际的ThirdBodyForce类结构添加相应的参数设置
}

void UiThirdBodyForce::refreshEphemerisSource()
{
    // 这里需要根据实际的EEphemerisSource枚举添加相应的选项
}

void UiThirdBodyForce::refreshAttractionType()
{
    // 这里需要根据实际的EBodyAttractionType枚举添加相应的选项
}

void UiThirdBodyForce::refreshCelestialBodies()
{
    thirdBodyTable_->setRowCount(0);
    
    // 示例天体列表
    QStringList bodies = {"太阳", "月球", "水星", "金星", "火星", "木星", "土星", "天王星", "海王星"};
    
    for (const QString& bodyName : bodies) {
        int row = thirdBodyTable_->rowCount();
        thirdBodyTable_->insertRow(row);
        
        // 名称
        QTableWidgetItem* nameItem = new QTableWidgetItem(bodyName);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        thirdBodyTable_->setItem(row, 0, nameItem);
        
        // 使用
        QCheckBox* useCheck = new QCheckBox();
        if (bodyName == "太阳" || bodyName == "月球") {
            useCheck->setChecked(true);
        }
        thirdBodyTable_->setCellWidget(row, 1, useCheck);
        
        // 引力模型
        QComboBox* attractionCombo = new QComboBox();
        attractionCombo->addItem("点质量");
        attractionCombo->addItem("引力场");
        thirdBodyTable_->setCellWidget(row, 2, attractionCombo);
        
        // 星历来源
        QComboBox* ephemerisCombo = new QComboBox();
        ephemerisCombo->addItem("天体星历");
        ephemerisCombo->addItem("JPL DE");
        ephemerisCombo->addItem("JPL SPICE");
        thirdBodyTable_->setCellWidget(row, 3, ephemerisCombo);
    }
}

void UiThirdBodyForce::onBodySelectionChanged(int row, int column)
{
    // 处理天体选择变化
}

void UiThirdBodyForce::onAttractionTypeChanged(int row, int column)
{
    // 处理引力模型类型变化
}

AST_NAMESPACE_END