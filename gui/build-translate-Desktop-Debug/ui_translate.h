/********************************************************************************
** Form generated from reading UI file 'translate.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSLATE_H
#define UI_TRANSLATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_translate
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_2;
    QComboBox *comboBox;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *translate)
    {
        if (translate->objectName().isEmpty())
            translate->setObjectName(QStringLiteral("translate"));
        translate->resize(700, 504);
        centralWidget = new QWidget(translate);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 0, 681, 441));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        comboBox = new QComboBox(verticalLayoutWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        verticalLayout->addWidget(comboBox);

        plainTextEdit = new QPlainTextEdit(verticalLayoutWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        verticalLayout->addWidget(plainTextEdit);

        plainTextEdit_2 = new QPlainTextEdit(verticalLayoutWidget);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));

        verticalLayout->addWidget(plainTextEdit_2);

        translate->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(translate);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 700, 20));
        translate->setMenuBar(menuBar);
        mainToolBar = new QToolBar(translate);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        translate->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(translate);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        translate->setStatusBar(statusBar);

        retranslateUi(translate);

        QMetaObject::connectSlotsByName(translate);
    } // setupUi

    void retranslateUi(QMainWindow *translate)
    {
        translate->setWindowTitle(QApplication::translate("translate", "translate", 0));
        pushButton_2->setText(QApplication::translate("translate", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class translate: public Ui_translate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSLATE_H
