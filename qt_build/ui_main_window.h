/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *main;
    QGridLayout *gridLayout;
    QStackedWidget *frameViews;
    QWidget *viewDrive;
    QHBoxLayout *horizontalLayout;
    QProgressBar *barBrake;
    QWidget *frameCenterOuter;
    QVBoxLayout *verticalLayout;
    QFrame *frameCenterInner;
    QGridLayout *gridLayout_4;
    QWidget *buttonBar;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonSpeed;
    QPushButton *buttonEndurance;
    QPushButton *buttonLap;
    QPushButton *buttonMenu;
    QProgressBar *barThrottle;
    QWidget *viewDebug;
    QGridLayout *gridLayout_3;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(764, 496);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush2(QColor(255, 255, 220, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        QBrush brush3(QColor(255, 255, 255, 128));
        brush3.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush3);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush3);
#endif
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush3);
#endif
        MainWindow->setPalette(palette);
        main = new QWidget(MainWindow);
        main->setObjectName(QString::fromUtf8("main"));
        gridLayout = new QGridLayout(main);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        frameViews = new QStackedWidget(main);
        frameViews->setObjectName(QString::fromUtf8("frameViews"));
        viewDrive = new QWidget();
        viewDrive->setObjectName(QString::fromUtf8("viewDrive"));
        QFont font;
        font.setFamily(QString::fromUtf8("ExcludedItalic"));
        font.setPointSize(12);
        font.setItalic(true);
        viewDrive->setFont(font);
        horizontalLayout = new QHBoxLayout(viewDrive);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        barBrake = new QProgressBar(viewDrive);
        barBrake->setObjectName(QString::fromUtf8("barBrake"));
        barBrake->setEnabled(true);
        barBrake->setMinimumSize(QSize(30, 0));
        QPalette palette1;
        QBrush brush4(QColor(204, 84, 86, 170));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Highlight, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Highlight, brush4);
        QBrush brush5(QColor(31, 117, 204, 170));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        barBrake->setPalette(palette1);
        barBrake->setValue(24);
        barBrake->setOrientation(Qt::Vertical);
        barBrake->setInvertedAppearance(false);

        horizontalLayout->addWidget(barBrake);

        frameCenterOuter = new QWidget(viewDrive);
        frameCenterOuter->setObjectName(QString::fromUtf8("frameCenterOuter"));
        verticalLayout = new QVBoxLayout(frameCenterOuter);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 6, 0, 0);
        frameCenterInner = new QFrame(frameCenterOuter);
        frameCenterInner->setObjectName(QString::fromUtf8("frameCenterInner"));
        frameCenterInner->setFrameShape(QFrame::StyledPanel);
        frameCenterInner->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frameCenterInner);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));

        verticalLayout->addWidget(frameCenterInner);

        buttonBar = new QWidget(frameCenterOuter);
        buttonBar->setObjectName(QString::fromUtf8("buttonBar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonBar->sizePolicy().hasHeightForWidth());
        buttonBar->setSizePolicy(sizePolicy);
        buttonBar->setMaximumSize(QSize(16777215, 80));
        horizontalLayout_2 = new QHBoxLayout(buttonBar);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        buttonSpeed = new QPushButton(buttonBar);
        buttonSpeed->setObjectName(QString::fromUtf8("buttonSpeed"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonSpeed->sizePolicy().hasHeightForWidth());
        buttonSpeed->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(buttonSpeed);

        buttonEndurance = new QPushButton(buttonBar);
        buttonEndurance->setObjectName(QString::fromUtf8("buttonEndurance"));
        sizePolicy1.setHeightForWidth(buttonEndurance->sizePolicy().hasHeightForWidth());
        buttonEndurance->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(buttonEndurance);

        buttonLap = new QPushButton(buttonBar);
        buttonLap->setObjectName(QString::fromUtf8("buttonLap"));
        sizePolicy1.setHeightForWidth(buttonLap->sizePolicy().hasHeightForWidth());
        buttonLap->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(buttonLap);

        buttonMenu = new QPushButton(buttonBar);
        buttonMenu->setObjectName(QString::fromUtf8("buttonMenu"));
        sizePolicy1.setHeightForWidth(buttonMenu->sizePolicy().hasHeightForWidth());
        buttonMenu->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(buttonMenu);


        verticalLayout->addWidget(buttonBar);


        horizontalLayout->addWidget(frameCenterOuter);

        barThrottle = new QProgressBar(viewDrive);
        barThrottle->setObjectName(QString::fromUtf8("barThrottle"));
        barThrottle->setMinimumSize(QSize(30, 0));
        QPalette palette2;
        QBrush brush6(QColor(114, 204, 104, 170));
        brush6.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        barThrottle->setPalette(palette2);
        barThrottle->setValue(24);
        barThrottle->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(barThrottle);

        frameViews->addWidget(viewDrive);
        viewDebug = new QWidget();
        viewDebug->setObjectName(QString::fromUtf8("viewDebug"));
        gridLayout_3 = new QGridLayout(viewDebug);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(viewDebug);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);

        frameViews->addWidget(viewDebug);

        gridLayout->addWidget(frameViews, 0, 0, 1, 1);

        MainWindow->setCentralWidget(main);

        retranslateUi(MainWindow);

        frameViews->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        buttonSpeed->setText(QCoreApplication::translate("MainWindow", "Speed View", nullptr));
        buttonEndurance->setText(QCoreApplication::translate("MainWindow", "Endurance View", nullptr));
        buttonLap->setText(QCoreApplication::translate("MainWindow", "Lap View", nullptr));
        buttonMenu->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Debug Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
