#pragma once

#include "AstGlobal.h"
#include "AstGUI/UiObject.hpp"
#include "AstGUI/UiDouble.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QTableWidget>
#include <QGroupBox>

AST_NAMESPACE_BEGIN

class ThirdBodyForce;
class CelestialBody;

class AST_GUI_API UiThirdBodyForce: public UiObject
{
    Q_OBJECT
public:
    UiThirdBodyForce(Object* object, QWidget *parent = nullptr);
    UiThirdBodyForce(QWidget *parent = nullptr);
    ~UiThirdBodyForce() = default;

    ThirdBodyForce* getThirdBodyForce() const;
    void setThirdBodyForce(ThirdBodyForce* thirdBody);
    void refreshUi();
    void apply();
protected:
    void setupUi();
    void refreshEphemerisSource();
    void refreshAttractionType();
    void refreshCelestialBodies();
private slots:
    void applyTo(ThirdBodyForce* thirdBody);
    void onBodySelectionChanged(int row, int column);
    void onAttractionTypeChanged(int row, int column);
private:
    // 布局
    QVBoxLayout* mainLayout_{nullptr};
    QGroupBox* thirdBodyGroup_{nullptr};
    QVBoxLayout* thirdBodyLayout_{nullptr};
    QTableWidget* thirdBodyTable_{nullptr};
    
    // 数据
    ThirdBodyForce* thirdBodyForce_{nullptr};
};

AST_NAMESPACE_END