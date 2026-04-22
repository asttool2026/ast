#include "UiGravityForce.hpp"
#include "AstCore/GravityForce.hpp"
#include <QGroupBox>

AST_NAMESPACE_BEGIN

UiGravityForce::UiGravityForce(Object* object, QWidget *parent)
    : UiObject(parent)
{
    setObjectName("UiGravityForce");
    setupUi();
    if (object) {
        setObject(object);
        setGravityForce(dynamic_cast<GravityForce*>(object));
    }
}

UiGravityForce::UiGravityForce(QWidget *parent)
    : UiObject(parent)
{
    setObjectName("UiGravityForce");
    setupUi();
}

void UiGravityForce::setupUi()
{
    // 主布局
    mainLayout_ = new QVBoxLayout(this);
    
    // 中心天体重力场配置
    centralBodyGravityGroup_ = new QGroupBox("中心天体重力场", this);
    centralBodyGravityLayout_ = new QGridLayout(centralBodyGravityGroup_);
    
    modelLayout_ = new QHBoxLayout();
    modelLabel_ = new QLabel("模型:", this);
    modelCombo_ = new QComboBox(this);
    modelFileEdit_ = new UiFilePath(this);
    // modelFileEdit_->setFileMode(UiFilePath::FileMode::OpenFile);
    modelFileEdit_->setFilter("Gravity files (*.grv);;All files (*.*)");
    modelLayout_->addWidget(modelLabel_);
    modelLayout_->addWidget(modelCombo_);
    modelLayout_->addWidget(modelFileEdit_);
    centralBodyGravityLayout_->addLayout(modelLayout_, 0, 0, 1, 2);
    
    maxDegreeLayout_ = new QHBoxLayout();
    maxDegreeLabel_ = new QLabel("最大阶数:", this);
    maxDegreeEdit_ = new UiInteger(this);
    maxDegreeEdit_->setValue(2);
    maxDegreeLayout_->addWidget(maxDegreeLabel_);
    maxDegreeLayout_->addWidget(maxDegreeEdit_);
    centralBodyGravityLayout_->addLayout(maxDegreeLayout_, 1, 0);
    
    maxOrderLayout_ = new QHBoxLayout();
    maxOrderLabel_ = new QLabel("最大次数:", this);
    maxOrderEdit_ = new UiInteger(this);
    maxOrderEdit_->setValue(0);
    maxOrderLayout_->addWidget(maxOrderLabel_);
    maxOrderLayout_->addWidget(maxOrderEdit_);
    centralBodyGravityLayout_->addLayout(maxOrderLayout_, 1, 1);
    
    secularVariationsLayout_ = new QHBoxLayout();
    secularVariationsCheck_ = new QCheckBox("包含引力场长期变化", this);
    secularVariationsLayout_->addWidget(secularVariationsCheck_);
    centralBodyGravityLayout_->addLayout(secularVariationsLayout_, 2, 0, 1, 2);
    
    // 固体潮汐配置
    solidTidesGroup_ = new QGroupBox("固体潮汐", this);
    solidTidesLayout_ = new QGridLayout(solidTidesGroup_);
    
    solidTideTypeLayout_ = new QHBoxLayout();
    solidTideTypeLabel_ = new QLabel("类型:", this);
    solidTideTypeCombo_ = new QComboBox(this);
    solidTideTypeLayout_->addWidget(solidTideTypeLabel_);
    solidTideTypeLayout_->addWidget(solidTideTypeCombo_);
    solidTidesLayout_->addLayout(solidTideTypeLayout_, 0, 0, 1, 2);
    
    timeDependentLayout_ = new QHBoxLayout();
    timeDependentCheck_ = new QCheckBox("包含时间依赖项", this);
    timeDependentLayout_->addWidget(timeDependentCheck_);
    solidTidesLayout_->addLayout(timeDependentLayout_, 1, 0, 1, 2);
    
    minAmplitudeLayout_ = new QHBoxLayout();
    minAmplitudeLabel_ = new QLabel("最小振幅:", this);
    minAmplitudeEdit_ = new UiQuantity(this);
    minAmplitudeEdit_->setValueInUnit(0.0, Unit("m"));
    minAmplitudeLayout_->addWidget(minAmplitudeLabel_);
    minAmplitudeLayout_->addWidget(minAmplitudeEdit_);
    solidTidesLayout_->addLayout(minAmplitudeLayout_, 2, 0, 1, 2);
    
    truncateLayout_ = new QHBoxLayout();
    truncateCheck_ = new QCheckBox("截断到引力场大小", this);
    truncateLayout_->addWidget(truncateCheck_);
    solidTidesLayout_->addLayout(truncateLayout_, 3, 0, 1, 2);
    
    // 海洋潮汐配置
    oceanTidesGroup_ = new QGroupBox("海洋潮汐", this);
    oceanTidesLayout_ = new QGridLayout(oceanTidesGroup_);
    
    useOceanTidesLayout_ = new QHBoxLayout();
    useOceanTidesCheck_ = new QCheckBox("使用海洋潮汐", this);
    useOceanTidesLayout_->addWidget(useOceanTidesCheck_);
    oceanTidesLayout_->addLayout(useOceanTidesLayout_, 0, 0, 1, 2);
    
    maxDegreeOceanLayout_ = new QHBoxLayout();
    maxDegreeOceanLabel_ = new QLabel("最大阶数:", this);
    maxDegreeOceanEdit_ = new UiInteger(this);
    maxDegreeOceanEdit_->setValue(2);
    maxDegreeOceanLayout_->addWidget(maxDegreeOceanLabel_);
    maxDegreeOceanLayout_->addWidget(maxDegreeOceanEdit_);
    oceanTidesLayout_->addLayout(maxDegreeOceanLayout_, 1, 0);
    
    maxOrderOceanLayout_ = new QHBoxLayout();
    maxOrderOceanLabel_ = new QLabel("最大次数:", this);
    maxOrderOceanEdit_ = new UiInteger(this);
    maxOrderOceanEdit_->setValue(0);
    maxOrderOceanLayout_->addWidget(maxOrderOceanLabel_);
    maxOrderOceanLayout_->addWidget(maxOrderOceanEdit_);
    oceanTidesLayout_->addLayout(maxOrderOceanLayout_, 1, 1);
    
    minAmplitudeOceanLayout_ = new QHBoxLayout();
    minAmplitudeOceanLabel_ = new QLabel("最小振幅:", this);
    minAmplitudeOceanEdit_ = new UiQuantity(this);
    minAmplitudeOceanEdit_->setValueInUnit(0.0, Unit("m"));
    minAmplitudeOceanLayout_->addWidget(minAmplitudeOceanLabel_);
    minAmplitudeOceanLayout_->addWidget(minAmplitudeOceanEdit_);
    oceanTidesLayout_->addLayout(minAmplitudeOceanLayout_, 2, 0, 1, 2);
    
    // 添加到主布局
    mainLayout_->addWidget(centralBodyGravityGroup_);
    mainLayout_->addWidget(solidTidesGroup_);
    mainLayout_->addWidget(oceanTidesGroup_);
    mainLayout_->addStretch();
    
    // 刷新UI
    refreshModel();
    refreshSolidTideType();
}

GravityForce* UiGravityForce::getGravityForce() const
{
    return gravityForce_;
}

void UiGravityForce::setGravityForce(GravityForce* gravity)
{
    gravityForce_ = gravity;
    refreshUi();
}

void UiGravityForce::refreshUi()
{
    if (!gravityForce_) return;
    
    // 中心天体重力场配置
    modelFileEdit_->setPath(gravityForce_->model_);
    maxDegreeEdit_->setValue(gravityForce_->maxDegree_);
    maxOrderEdit_->setValue(gravityForce_->maxOrder_);
    secularVariationsCheck_->setChecked(gravityForce_->useSecularVariations_);
    
    // 固体潮汐配置
    solidTideTypeCombo_->setCurrentIndex(static_cast<int>(gravityForce_->solidTideType_));
    timeDependentCheck_->setChecked(gravityForce_->includeTimeDependentSolidTides_);
    minAmplitudeEdit_->setValue(gravityForce_->minAmplitudeSolidTides_);
    truncateCheck_->setChecked(gravityForce_->truncateSolidTides_);
    
    // 海洋潮汐配置
    useOceanTidesCheck_->setChecked(gravityForce_->useOceanTides_);
    maxDegreeOceanEdit_->setValue(gravityForce_->maxDegreeOceanTides_);
    maxOrderOceanEdit_->setValue(gravityForce_->maxOrderOceanTides_);
    minAmplitudeOceanEdit_->setValue(gravityForce_->minAmplitudeOceanTides_);
}

void UiGravityForce::apply()
{
    if (gravityForce_) {
        applyTo(gravityForce_);
    }
}

void UiGravityForce::applyTo(GravityForce* gravity)
{
    if (!gravity) return;
    
    // 中心天体重力场配置
    gravity->model_ = modelFileEdit_->path().toUtf8().data();
    gravity->maxDegree_ = maxDegreeEdit_->value();
    gravity->maxOrder_ = maxOrderEdit_->value();
    gravity->useSecularVariations_ = secularVariationsCheck_->isChecked();
    
    // 固体潮汐配置
    gravity->solidTideType_ = static_cast<ESolidTideType>(solidTideTypeCombo_->currentIndex());
    gravity->includeTimeDependentSolidTides_ = timeDependentCheck_->isChecked();
    gravity->minAmplitudeSolidTides_ = minAmplitudeEdit_->value();
    gravity->truncateSolidTides_ = truncateCheck_->isChecked();
    
    // 海洋潮汐配置
    gravity->useOceanTides_ = useOceanTidesCheck_->isChecked();
    gravity->maxDegreeOceanTides_ = maxDegreeOceanEdit_->value();
    gravity->maxOrderOceanTides_ = maxOrderOceanEdit_->value();
    gravity->minAmplitudeOceanTides_ = minAmplitudeOceanEdit_->value();
}

void UiGravityForce::refreshModel()
{
    modelCombo_->clear();
    modelCombo_->addItem("WGS84");
    modelCombo_->addItem("EGM2008");
    modelCombo_->addItem("自定义文件");
}

void UiGravityForce::refreshSolidTideType()
{
    solidTideTypeCombo_->clear();
    solidTideTypeCombo_->addItem("无");
    solidTideTypeCombo_->addItem("仅永久潮汐");
    solidTideTypeCombo_->addItem("完整潮汐");
}

AST_NAMESPACE_END