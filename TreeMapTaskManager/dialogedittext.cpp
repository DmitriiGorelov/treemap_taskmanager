#include <QToolBar>
#include <QIcon>
#include <QPixmap>
#include <QAction>
#include <QDesktopWidget>

#include "dialogedittext.h"
#include "ui_cdialogedittext.h"
#include "mainwindow.h"

cDialogEditText::cDialogEditText(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cDialogEditText)
{
    ui->setupUi(this);

    QToolBar* toolBar = new QToolBar("Filter Bar");
    QAction* confirm = toolBar->addAction(tr(""));
    QAction* cancel = toolBar->addAction(tr(""));
    ui->verticalLayout->addWidget(toolBar);
    //ui->verticalLayout->SetFixedSize(1);
    cancel->setIcon(QIcon(QPixmap(":/task/images/cancel.png")));
    confirm->setIcon(QIcon(QPixmap(":/task/images/confirm.png")));
    connect(confirm, &QAction::triggered, this, [confirm, this](){accept();});
    connect(cancel, &QAction::triggered, this, [cancel, this](){reject();});
    //toolBar->setau
}

cDialogEditText::~cDialogEditText()
{
    delete ui;
}


void cDialogEditText::SetContent(const QString& str)
{
    //textEdit->setHtml(str);
}

QString cDialogEditText::GetContent()
{
    //return textEdit->toHtml();
    return QString("");
}

int cDialogEditText::ShowIt()
{
    setWindowFlags(Qt::Window); // this shows the empty menu

    //1
    //showMaximized();

    //2
    //showFullScreen();

    //3
    //setWindowState(windowState() | Qt::WindowMaximized);

    //4
    QDesktopWidget desk;
    QRect screenres = desk.screenGeometry(0);
    setGeometry(QRect(screenres.x(),screenres.y(),screenres.width(),screenres.height()));


    return exec();
}
