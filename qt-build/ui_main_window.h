/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
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
    QWidget *mainLayout;
    QGridLayout *gridLayout;
    QStackedWidget *frameViews;
    QWidget *viewMenu;
    QGridLayout *gridLayout_5;
    QPushButton *menuButtonSpeed;
    QPushButton *menuButtonEndurance;
    QPushButton *menuButtonLap;
    QPushButton *menuButtonDebug;
    QWidget *viewDrive;
    QHBoxLayout *horizontalLayout;
    QProgressBar *barBrake;
    QWidget *frameCenterOuter;
    QVBoxLayout *verticalLayout;
    QFrame *frameCenterInner;
    QGridLayout *gridLayout_4;
    QProgressBar *barRegen;
    QProgressBar *barTorque;
    QStackedWidget *frameSubViews;
    QWidget *subViewSpeed;
    QHBoxLayout *frameHorizontal;
    QFrame *frameSpeed;
    QLabel *statSpeed;
    QLabel *labelSpeed;
    QWidget *subViewEndurance;
    QHBoxLayout *horizontalLayout_4;
    QFrame *frameCharge;
    QLabel *statCharge;
    QLabel *labelCharge;
    QProgressBar *barRpm;
    QWidget *buttonBar;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *driveButtonSpeed;
    QPushButton *driveButtonEndurance;
    QPushButton *driveButtonLap;
    QPushButton *driveButtonMenu;
    QProgressBar *barThrottle;
    QWidget *viewDebug;
    QGridLayout *gridLayout_3;
    QLabel *labelDebug;
    QPushButton *debugButtonMenu;

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
        mainLayout = new QWidget(MainWindow);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        gridLayout = new QGridLayout(mainLayout);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        frameViews = new QStackedWidget(mainLayout);
        frameViews->setObjectName(QString::fromUtf8("frameViews"));
        viewMenu = new QWidget();
        viewMenu->setObjectName(QString::fromUtf8("viewMenu"));
        QFont font;
        font.setFamily(QString::fromUtf8("ExcludedItalic"));
        font.setPointSize(18);
        font.setItalic(true);
        viewMenu->setFont(font);
        gridLayout_5 = new QGridLayout(viewMenu);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        menuButtonSpeed = new QPushButton(viewMenu);
        menuButtonSpeed->setObjectName(QString::fromUtf8("menuButtonSpeed"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(menuButtonSpeed->sizePolicy().hasHeightForWidth());
        menuButtonSpeed->setSizePolicy(sizePolicy);

        gridLayout_5->addWidget(menuButtonSpeed, 0, 0, 1, 1);

        menuButtonEndurance = new QPushButton(viewMenu);
        menuButtonEndurance->setObjectName(QString::fromUtf8("menuButtonEndurance"));
        sizePolicy.setHeightForWidth(menuButtonEndurance->sizePolicy().hasHeightForWidth());
        menuButtonEndurance->setSizePolicy(sizePolicy);

        gridLayout_5->addWidget(menuButtonEndurance, 0, 1, 1, 1);

        menuButtonLap = new QPushButton(viewMenu);
        menuButtonLap->setObjectName(QString::fromUtf8("menuButtonLap"));
        sizePolicy.setHeightForWidth(menuButtonLap->sizePolicy().hasHeightForWidth());
        menuButtonLap->setSizePolicy(sizePolicy);

        gridLayout_5->addWidget(menuButtonLap, 1, 0, 1, 1);

        menuButtonDebug = new QPushButton(viewMenu);
        menuButtonDebug->setObjectName(QString::fromUtf8("menuButtonDebug"));
        sizePolicy.setHeightForWidth(menuButtonDebug->sizePolicy().hasHeightForWidth());
        menuButtonDebug->setSizePolicy(sizePolicy);

        gridLayout_5->addWidget(menuButtonDebug, 1, 1, 1, 1);

        frameViews->addWidget(viewMenu);
        viewDrive = new QWidget();
        viewDrive->setObjectName(QString::fromUtf8("viewDrive"));
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
        QFont font1;
        font1.setFamily(QString::fromUtf8("ExcludedItalic"));
        font1.setPointSize(8);
        font1.setItalic(true);
        barBrake->setFont(font1);
        barBrake->setStyleSheet(QString::fromUtf8("QProgressBar {\n"
"    border: 2px solid grey;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"    background-color: #05B8CC;\n"
"    width: 20px;\n"
"}"));
        barBrake->setValue(24);
        barBrake->setTextVisible(false);
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
        barRegen = new QProgressBar(frameCenterInner);
        barRegen->setObjectName(QString::fromUtf8("barRegen"));
        barRegen->setMinimumSize(QSize(0, 0));
        barRegen->setMaximumSize(QSize(16777215, 10));
        barRegen->setValue(24);
        barRegen->setTextVisible(false);

        gridLayout_4->addWidget(barRegen, 4, 0, 1, 1);

        barTorque = new QProgressBar(frameCenterInner);
        barTorque->setObjectName(QString::fromUtf8("barTorque"));
        barTorque->setMinimumSize(QSize(0, 0));
        barTorque->setMaximumSize(QSize(16777215, 10));
        QPalette palette2;
        QBrush brush6(QColor(99, 204, 92, 170));
        brush6.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        barTorque->setPalette(palette2);
        barTorque->setValue(24);
        barTorque->setTextVisible(false);

        gridLayout_4->addWidget(barTorque, 3, 0, 1, 1);

        frameSubViews = new QStackedWidget(frameCenterInner);
        frameSubViews->setObjectName(QString::fromUtf8("frameSubViews"));
        subViewSpeed = new QWidget();
        subViewSpeed->setObjectName(QString::fromUtf8("subViewSpeed"));
        subViewSpeed->setLayoutDirection(Qt::LeftToRight);
        frameHorizontal = new QHBoxLayout(subViewSpeed);
        frameHorizontal->setObjectName(QString::fromUtf8("frameHorizontal"));
        frameSpeed = new QFrame(subViewSpeed);
        frameSpeed->setObjectName(QString::fromUtf8("frameSpeed"));
        frameSpeed->setMaximumSize(QSize(260, 140));
        frameSpeed->setFrameShape(QFrame::NoFrame);
        frameSpeed->setFrameShadow(QFrame::Plain);
        frameSpeed->setLineWidth(0);
        statSpeed = new QLabel(frameSpeed);
        statSpeed->setObjectName(QString::fromUtf8("statSpeed"));
        statSpeed->setGeometry(QRect(0, 0, 260, 140));
        QFont font2;
        font2.setPointSize(112);
        statSpeed->setFont(font2);
        statSpeed->setAlignment(Qt::AlignCenter);
        labelSpeed = new QLabel(frameSpeed);
        labelSpeed->setObjectName(QString::fromUtf8("labelSpeed"));
        labelSpeed->setGeometry(QRect(200, 120, 60, 20));
        labelSpeed->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        frameHorizontal->addWidget(frameSpeed);

        frameSubViews->addWidget(subViewSpeed);
        subViewEndurance = new QWidget();
        subViewEndurance->setObjectName(QString::fromUtf8("subViewEndurance"));
        horizontalLayout_4 = new QHBoxLayout(subViewEndurance);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        frameCharge = new QFrame(subViewEndurance);
        frameCharge->setObjectName(QString::fromUtf8("frameCharge"));
        frameCharge->setMaximumSize(QSize(260, 140));
        frameCharge->setFrameShape(QFrame::NoFrame);
        frameCharge->setFrameShadow(QFrame::Plain);
        frameCharge->setLineWidth(0);
        statCharge = new QLabel(frameCharge);
        statCharge->setObjectName(QString::fromUtf8("statCharge"));
        statCharge->setGeometry(QRect(0, 0, 260, 140));
        statCharge->setFont(font2);
        statCharge->setAlignment(Qt::AlignCenter);
        labelCharge = new QLabel(frameCharge);
        labelCharge->setObjectName(QString::fromUtf8("labelCharge"));
        labelCharge->setGeometry(QRect(200, 120, 60, 20));
        labelCharge->setFont(font);
        labelCharge->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_4->addWidget(frameCharge);

        frameSubViews->addWidget(subViewEndurance);

        gridLayout_4->addWidget(frameSubViews, 1, 0, 1, 1);

        barRpm = new QProgressBar(frameCenterInner);
        barRpm->setObjectName(QString::fromUtf8("barRpm"));
        barRpm->setStyleSheet(QString::fromUtf8("border-radius : 0px"));
        barRpm->setValue(24);

        gridLayout_4->addWidget(barRpm, 0, 0, 1, 1);


        verticalLayout->addWidget(frameCenterInner);

        buttonBar = new QWidget(frameCenterOuter);
        buttonBar->setObjectName(QString::fromUtf8("buttonBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonBar->sizePolicy().hasHeightForWidth());
        buttonBar->setSizePolicy(sizePolicy1);
        buttonBar->setMaximumSize(QSize(16777215, 80));
        buttonBar->setFont(font);
        horizontalLayout_2 = new QHBoxLayout(buttonBar);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        driveButtonSpeed = new QPushButton(buttonBar);
        driveButtonSpeed->setObjectName(QString::fromUtf8("driveButtonSpeed"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(driveButtonSpeed->sizePolicy().hasHeightForWidth());
        driveButtonSpeed->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(driveButtonSpeed);

        driveButtonEndurance = new QPushButton(buttonBar);
        driveButtonEndurance->setObjectName(QString::fromUtf8("driveButtonEndurance"));
        sizePolicy2.setHeightForWidth(driveButtonEndurance->sizePolicy().hasHeightForWidth());
        driveButtonEndurance->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(driveButtonEndurance);

        driveButtonLap = new QPushButton(buttonBar);
        driveButtonLap->setObjectName(QString::fromUtf8("driveButtonLap"));
        sizePolicy2.setHeightForWidth(driveButtonLap->sizePolicy().hasHeightForWidth());
        driveButtonLap->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(driveButtonLap);

        driveButtonMenu = new QPushButton(buttonBar);
        driveButtonMenu->setObjectName(QString::fromUtf8("driveButtonMenu"));
        sizePolicy2.setHeightForWidth(driveButtonMenu->sizePolicy().hasHeightForWidth());
        driveButtonMenu->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(driveButtonMenu);


        verticalLayout->addWidget(buttonBar);


        horizontalLayout->addWidget(frameCenterOuter);

        barThrottle = new QProgressBar(viewDrive);
        barThrottle->setObjectName(QString::fromUtf8("barThrottle"));
        barThrottle->setMinimumSize(QSize(30, 0));
        QPalette palette3;
        QBrush brush7(QColor(114, 204, 104, 170));
        brush7.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        barThrottle->setPalette(palette3);
        barThrottle->setFont(font1);
        barThrottle->setStyleSheet(QString::fromUtf8("border-radius: 0px"));
        barThrottle->setValue(24);
        barThrottle->setTextVisible(false);
        barThrottle->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(barThrottle);

        frameViews->addWidget(viewDrive);
        viewDebug = new QWidget();
        viewDebug->setObjectName(QString::fromUtf8("viewDebug"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Source Code Pro"));
        font3.setPointSize(10);
        viewDebug->setFont(font3);
        gridLayout_3 = new QGridLayout(viewDebug);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        labelDebug = new QLabel(viewDebug);
        labelDebug->setObjectName(QString::fromUtf8("labelDebug"));

        gridLayout_3->addWidget(labelDebug, 0, 0, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);

        debugButtonMenu = new QPushButton(viewDebug);
        debugButtonMenu->setObjectName(QString::fromUtf8("debugButtonMenu"));

        gridLayout_3->addWidget(debugButtonMenu, 1, 0, 1, 1);

        frameViews->addWidget(viewDebug);

        gridLayout->addWidget(frameViews, 0, 0, 1, 1);

        MainWindow->setCentralWidget(mainLayout);

        retranslateUi(MainWindow);

        frameViews->setCurrentIndex(1);
        frameSubViews->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        menuButtonSpeed->setText(QCoreApplication::translate("MainWindow", "Speed View", nullptr));
        menuButtonEndurance->setText(QCoreApplication::translate("MainWindow", "Endurance View", nullptr));
        menuButtonLap->setText(QString());
        menuButtonDebug->setText(QCoreApplication::translate("MainWindow", "Debug Menu", nullptr));
        statSpeed->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        labelSpeed->setText(QCoreApplication::translate("MainWindow", "MPH", nullptr));
        statCharge->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        labelCharge->setText(QCoreApplication::translate("MainWindow", "SoC", nullptr));
        driveButtonSpeed->setText(QCoreApplication::translate("MainWindow", "Speed\n"
"View", nullptr));
        driveButtonEndurance->setText(QCoreApplication::translate("MainWindow", "Endurance\n"
"View", nullptr));
        driveButtonLap->setText(QString());
        driveButtonMenu->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        labelDebug->setText(QCoreApplication::translate("MainWindow", "Debug Menu", nullptr));
        debugButtonMenu->setText(QCoreApplication::translate("MainWindow", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
