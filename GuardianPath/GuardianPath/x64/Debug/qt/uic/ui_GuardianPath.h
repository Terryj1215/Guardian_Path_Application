/********************************************************************************
** Form generated from reading UI file 'GuardianPath.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUARDIANPATH_H
#define UI_GUARDIANPATH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GuardianPathClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GuardianPathClass)
    {
        if (GuardianPathClass->objectName().isEmpty())
            GuardianPathClass->setObjectName("GuardianPathClass");
        GuardianPathClass->resize(600, 400);
        menuBar = new QMenuBar(GuardianPathClass);
        menuBar->setObjectName("menuBar");
        GuardianPathClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GuardianPathClass);
        mainToolBar->setObjectName("mainToolBar");
        GuardianPathClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GuardianPathClass);
        centralWidget->setObjectName("centralWidget");
        GuardianPathClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GuardianPathClass);
        statusBar->setObjectName("statusBar");
        GuardianPathClass->setStatusBar(statusBar);

        retranslateUi(GuardianPathClass);

        QMetaObject::connectSlotsByName(GuardianPathClass);
    } // setupUi

    void retranslateUi(QMainWindow *GuardianPathClass)
    {
        GuardianPathClass->setWindowTitle(QCoreApplication::translate("GuardianPathClass", "GuardianPath", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GuardianPathClass: public Ui_GuardianPathClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUARDIANPATH_H
