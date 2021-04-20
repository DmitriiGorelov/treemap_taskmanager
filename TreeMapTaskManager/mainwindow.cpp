#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QGuiApplication>
#include <QMessageBox>
#include <QPainter>
#include <QPoint>
#include <QDir>
#include <QIcon>
#include <QSizePolicy>
#include <QToolBar>
#include <QInputDialog>
#include <QFileDialog>
#include <QSettings>

#ifndef QT_NO_DEBUG
#define CHECK_TRUE(instruction) Q_ASSERT(instruction)
#else
#define CHECK_TRUE(instruction) (instruction)
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_wFocusedTaskPopUp(Q_NULLPTR)
    , m_wTextEdit(Q_NULLPTR)
    , wProjects(Q_NULLPTR)
    , wTasks(Q_NULLPTR)
    , widthWasBigger(-1)
    , m_vLayoutMain(Q_NULLPTR)
    , m_hLayoutMain(Q_NULLPTR)
#if UseGroupButtons
    , m_wGroupButtons(Q_NULLPTR)
#else
    , tbView(Q_NULLPTR)
    , tbProjects(Q_NULLPTR)
    , tbTasks(Q_NULLPTR)
#endif
    , s(nullptr)
{
    ui->setupUi(this);

    wProjects = new cViewProjects(this);
    connect(wProjects, &cViewProjects::projectSelected, this, &MainWindow::projectSelected);

    QString fname("");
    if (LoadLastPath(fname))
    {
        wTasks = new Widget(fname,this);
    }
    else
    {
        wTasks = new Widget(QString(std::string(backupFolder() + std::string("/OscInfo.xml")).c_str()),this);
    }
    connect(wTasks, &Widget::MousePressed, this, &MainWindow::MapMousePressed);
    connect(wTasks, &Widget::FocusedTaskPopUp, this, &MainWindow::FocusedTaskPopUp);

#if UseGroupButtons
    m_wGroupButtons = new cGroupButtons(this);    
    connect(m_wGroupButtons, &cGroupButtons::AddTask,this,&MainWindow::AddTask);
    connect(m_wGroupButtons, &cGroupButtons::LevelUp,this,&MainWindow::LevelUp);
    connect(m_wGroupButtons, &cGroupButtons::FocusIn, this, &MainWindow::FocusInGroupButtons);
#else

    // -------------------------------------------------------------------------------------------------File
    QMenu* mFile = menuBar()->addMenu(tr("&File"));
    {
        const QIcon Icon = QIcon::fromTheme("document-new", QIcon(":/file/images/fileopen.png"));
        QAction* a = mFile->addAction(Icon,  tr("&Open"), this, &MainWindow::DataBaseOpen);
        a->setPriority(QAction::LowPriority);
        a->setShortcut(QKeySequence::Open);
    }

    {
    const QIcon Icon = QIcon::fromTheme("document-new", QIcon(":/file/images/filenew.png"));
    QAction *a = mFile->addAction(Icon,  tr("&New"), this, &MainWindow::DataBaseNew);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::New);
    }

    {
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/file/images/filesave.png"));
    QAction* a = mFile->addAction(saveIcon, tr("&Save"), this, &MainWindow::DataBaseSave);
    a->setShortcut(QKeySequence::Save);
    }

    {
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/file/images/filesaveas.png"));
    QAction* a = mFile->addAction(tr("Save &As..."), this, &MainWindow::DataBaseSaveAs);
    a->setPriority(QAction::LowPriority);
    }

    // -------------------------------------------------------------------------------------------------View
    tbView= addToolBar(tr("View"));
    QMenu* mView = menuBar()->addMenu(tr("&View"));

    {
        const QIcon Icon = QIcon::fromTheme("document-new", QIcon(":/view/images/projects.png"));
        QAction *a = mView->addAction(Icon,  tr("&Projects"), this, &MainWindow::ViewProjects);
        tbView->addAction(a);
        a->setPriority(QAction::LowPriority);
        a->setShortcut(QKeySequence::New);
    }

    {
        const QIcon Icon = QIcon::fromTheme("document-new", QIcon(":/view/images/workout.png"));
        QAction *a = mView->addAction(Icon,  tr("&Tasks"), this, &MainWindow::ViewTasks);
        tbView->addAction(a);
        a->setPriority(QAction::LowPriority);
        a->setShortcut(QKeySequence::Back);
    }


    // -------------------------------------------------------------------------------------------------Projects
    tbProjects= addToolBar(tr("Projects"));
    QMenu* mProjects = menuBar()->addMenu(tr("&Projects"));

    {
        const QIcon Icon = QIcon::fromTheme("document-new", QIcon(":/task/images/add.png"));
        QAction *a = mProjects->addAction(Icon,  tr("New Project"), this, &MainWindow::AddProject);
        tbProjects->addAction(a);
        a->setPriority(QAction::LowPriority);
        a->setShortcut(QKeySequence::New);
    }



// -----------------------------------------------------------------------------------------------------------------Tasks
    tbTasks = addToolBar(tr("Tasks"));
    QMenu *mTasks = menuBar()->addMenu(tr("&Tasks"));

    {
        const QIcon Icon = QIcon::fromTheme("document-new", QIcon(":/task/images/add.png"));
        QAction *a = mTasks->addAction(Icon,  tr("&New Task"), this, &MainWindow::AddTaskSelected);
        tbTasks->addAction(a);
        a->setPriority(QAction::LowPriority);
        a->setShortcut(QKeySequence::New);
    }

    {
        const QIcon Icon = QIcon::fromTheme("document-new", QIcon(":/task/images/levelup.png"));
        QAction *a = mTasks->addAction(Icon,  tr("&Up"), this, &MainWindow::LevelUp);
        tbTasks->addAction(a);
        a->setPriority(QAction::LowPriority);
        a->setShortcut(QKeySequence::Back);
    }
#endif

    statusBar()->hide();

    s = QGuiApplication::primaryScreen();
    connect(s, &QScreen::primaryOrientationChanged, this, &MainWindow::primaryOrientationChanged);

//#if defined(Q_OS_WIN) || defined(Q_OS_MAC)

#ifdef Q_OS_ANDROID
    ui->menubar->hide();
#else
        ui->menubar->show();
#endif







    //QMessageBox::information(this, "" , tr("Orientation CONSTR")+QString::number(s->primaryOrientation()), QMessageBox::Ok);

    //QDir pathDir(backupFolder().c_str());
    //QMessageBox::information(this,(pathDir.exists()?"Path FOUND":"Path NOT FOUND"),backupFolder().c_str(),QMessageBox::Ok);


    ViewProjects();
}

void MainWindow::primaryOrientationChanged(Qt::ScreenOrientation orientation)
{
    //QMessageBox::information(this, "" , tr("Orientation SLOT")+QString::number(orientation), QMessageBox::Ok);
}

int MainWindow::ButtonSize()
{
    static int rx = logicalDpiX();
    static int ry = logicalDpiY();
    static float isize = 0.5; //0.5 inch
    return rx*isize;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   // Your code here.

   if (width()>height())
   {
        if (widthWasBigger!=1)
        {
            //Main----------------------------------------------------------------------------------
            if (this->centralWidget()->layout()==m_vLayoutMain)
            {
                if (m_hLayoutMain)
                    delete m_hLayoutMain;
                if (m_vLayoutMain)
                    delete m_vLayoutMain;
            }
            else
            {
                if (m_vLayoutMain)
                    delete m_vLayoutMain;
                if (m_hLayoutMain)
                    delete m_hLayoutMain;
            }

            // Create vertical layout
            m_vLayoutMain = new QVBoxLayout();
            // Set vertical layout margin to zero
            m_vLayoutMain->setMargin(0);

            // Create horizontal layout
            m_hLayoutMain = new QHBoxLayout();
            // Set horizontal layout to zero
            m_hLayoutMain->setMargin(0);
            m_hLayoutMain->setSpacing(0);

            // Add horizontal layout to vertical layout
            m_vLayoutMain->addLayout(m_hLayoutMain);

            m_hLayoutMain->addWidget(wTasks);
            m_hLayoutMain->addWidget(wProjects);
#if UseGroupButtons
            m_hLayoutMain->addWidget(m_wGroupButtons);
#endif

            this->centralWidget()->setLayout(m_vLayoutMain);            
        }

#if UseGroupButtons
        //m_wGroupButtons->setMinimumWidth(ButtonSize());
        //m_wGroupButtons->setMinimumHeight(height());
        m_wGroupButtons->setMaximumWidth(ButtonSize());
        m_wGroupButtons->setMaximumHeight(height());
#endif
        widthWasBigger = 1;
   }
   else
   {
       if (widthWasBigger!=0)
       {
           //Main----------------------------------------------------------------------------------
           if (this->centralWidget()->layout()==m_vLayoutMain)
           {
               if (m_hLayoutMain)
                   delete m_hLayoutMain;
               if (m_vLayoutMain)
                   delete m_vLayoutMain;
           }
           else
           {
               if (m_vLayoutMain)
                   delete m_vLayoutMain;
               if (m_hLayoutMain)
                   delete m_hLayoutMain;
           }

           // Create vertical layout
           m_vLayoutMain = new QVBoxLayout();
           // Set vertical layout margin to zero
           m_vLayoutMain->setMargin(0);

           // Create horizontal layout
           m_hLayoutMain = new QHBoxLayout();
           // Set horizontal layout to zero
           m_hLayoutMain->setMargin(0);
           m_hLayoutMain->setSpacing(0);

           // Add horizontal layout to vertical layout
           m_hLayoutMain->addLayout(m_vLayoutMain);

           m_vLayoutMain->addWidget(wTasks);
           m_vLayoutMain->addWidget(wProjects);
#if UseGroupButtons
           m_vLayoutMain->addWidget(m_wGroupButtons);
#endif

           this->centralWidget()->setLayout(m_hLayoutMain);           
       }
#if UseGroupButtons
       //m_wGroupButtons->setMinimumWidth(width());
       //m_wGroupButtons->setMinimumHeight(ButtonSize());
       m_wGroupButtons->setMaximumWidth(width());
       m_wGroupButtons->setMaximumHeight(ButtonSize());
#endif
       widthWasBigger = 0;
   }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    /*QPainter painter(this);
    painter.setBrush(QBrush(Qt::black));
    painter.setPen(Qt::red);

    painter.drawText(10, 100, "w="+QString::number(width()));
    painter.drawText(10, 200, "h="+QString::number(height()));
    //QScreen*s=QGuiApplication::primaryScreen();
    painter.drawText(10, 300, "o="+QString::number(s->primaryOrientation()));*/
}

MainWindow::~MainWindow()
{
    delete wTasks;
#if UseGroupButtons
    delete m_wGroupButtons;
#endif
    delete ui;
}

void MainWindow::projectSelected(const QString& uid)
{
    if (wTasks->SelectProject(uid))
    {

    }
}

void MainWindow::MapMousePressed(int x, int y, bool& canContinue)
{
    canContinue = !HideFocusedTaskPopUp();
}

void MainWindow::FocusedTaskPopUp(int x, int y)
{
    ShowFocusedTaskPopUp(x, y);
}

void MainWindow::ShowFocusedTaskPopUp(int x, int y)
{
    if (!m_wFocusedTaskPopUp)
    {
        m_wFocusedTaskPopUp=new cFocusedTaskPopUp(this);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::EditTask, this, &MainWindow::EditTask);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::DeleteTask, this, &MainWindow::DeleteTask);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::ViewTask, this, &MainWindow::ViewTask);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::AddTask, this, &MainWindow::AddTaskFocused);
    }

    //QPoint pos = mapToGlobal(QPoint(x, y));
    QPoint pos(x, y);
    m_wFocusedTaskPopUp->UpdateGeometry(pos, ButtonSize(), width(), height());
    m_wFocusedTaskPopUp->show();
}

bool MainWindow::HideFocusedTaskPopUp()
{
    bool result(false);
    if (m_wFocusedTaskPopUp)
    {
        result = m_wFocusedTaskPopUp->isVisible();
        m_wFocusedTaskPopUp->hide();
    }
    wTasks->repaint();
    return result;
}

void MainWindow::FocusInGroupButtons()
{
    HideFocusedTaskPopUp();
}

void MainWindow::ShowWindowEditTask()
{
#if Use3dTextEdit
    if (!m_wTextEdit)
    {
        m_wTextEdit = new TextEdit(this);
        connect(m_wTextEdit, &TextEdit::BeforeClose, this, &MainWindow::TextEditBeforeClose);
    }
    m_wTextEdit->ShowIt(wTasks->GetFocusedText());

#else
    if (!m_wTextEdit)
    {
        m_wTextEdit = new cDialogEditText(this);
        //connect(m_wTextEdit, &cDialogEditText::BeforeClose, this, &MainWindow::TextEditBeforeClose);
    }
    m_wTextEdit->SetContent(w->GetFocusedText());
    m_wTextEdit->setWindowModality(Qt::WindowModality::WindowModal);
    int res = m_wTextEdit->ShowIt();
    switch (res)
    {
        case QDialog::Accepted:
        {
            w->SetFocusedText(m_wTextEdit->GetContent());
            break;
        }
    }
#endif
}

void MainWindow::TextEditBeforeClose()
{
    if (m_wTextEdit && m_wTextEdit->Accepted())
    {
        wTasks->SetFocusedText(m_wTextEdit->GetContent());
    }
}

void MainWindow::AddTaskSelected()
{
    HideFocusedTaskPopUp();
    wTasks->AddTaskSelected();
    ShowWindowEditTask();
}

void MainWindow::AddTaskFocused()
{
    HideFocusedTaskPopUp();
    wTasks->AddTaskFocused();
    ShowWindowEditTask();
}

void MainWindow::LevelUp()
{
    HideFocusedTaskPopUp();
    wTasks->LevelUp();
}

void MainWindow::ViewTask()
{
    HideFocusedTaskPopUp();
}

void MainWindow::EditTask()
{
    HideFocusedTaskPopUp();

    ShowWindowEditTask();
}

void MainWindow::DeleteTask()
{
    HideFocusedTaskPopUp();
    wTasks->DeleteFocused();
}

void MainWindow::DataBaseOpen()
{
    QString fold;
    LoadLastPath(fold);

    QString fname = QFileDialog::getOpenFileName(this, tr("Open Database file"), QFileInfo(fold).dir().path(), tr("Database (*.xml)"));
    if (!fname.isEmpty())
    {
        if (wTasks->ReadXML(fname))
        {
           SaveLastPath(fname);
        }
    }
}

void MainWindow::DataBaseNew()
{
    QString fold;
    LoadLastPath(fold);

    QString fname = QFileDialog::getSaveFileName(this, tr("New Database file"), QFileInfo(fold).dir().path(), tr("Database (*.xml)"));
    if (!fname.isEmpty())
    {
        wTasks->clear();
        if (wTasks->WriteXML(fname))
        {
            SaveLastPath(fname);
        }
    }
}

void MainWindow::DataBaseSave()
{
    wTasks->WriteXML();
}

void MainWindow::DataBaseSaveAs()
{
    QString fold;
    LoadLastPath(fold);

    QString fname = QFileDialog::getSaveFileName(this, tr("Open Database file"), QFileInfo(fold).dir().path(),tr("Database (*.xml)"));
    if (!fname.isEmpty())
    {
        if (wTasks->WriteXML(fname))
        {
           SaveLastPath(fname);
        }
    }
}

void MainWindow::SaveLastPath(const QString& fname)
{
    QSettings set("GDV","TreeMapTaskManager");
    set.beginGroup("Paths");
    set.setValue("LastFile",fname);
    set.endGroup();
}

bool MainWindow::LoadLastPath(QString& fname)
{
    QSettings set("GDV","TreeMapTaskManager");
    set.beginGroup("Paths");
    fname = set.value("LastFile","").toString();
    set.endGroup();
    return !fname.isEmpty();
}

void MainWindow::ViewProjects()
{
    wTasks->hide();
    tbTasks->hide();

    wProjects->setData(wTasks->GetProjects());
    wProjects->show();
    tbProjects->show();
}

void MainWindow::ViewTasks()
{
    wProjects->hide();
    tbProjects->hide();

    wTasks->show();
    tbTasks->show();

}

void MainWindow::AddProject()
{
    bool ok(false);
    QString capt = QInputDialog::getText(this, tr("New Project"), tr("Name:"), QLineEdit::EchoMode::Normal,"",&ok);
    if (ok && !capt.isEmpty())
    {
        wTasks->AddProject(capt);
    }
}

//---------------------------------------------------------------------------------------------------cGroupButtons
cGroupButtons::cGroupButtons(QWidget *parent)
    : QWidget(parent)
    , m_b()
{
    QPushButton* b = new QPushButton(this);
    m_b.push_back(b);
    connect(b, &QPushButton::clicked,this, &cGroupButtons::bAddTaskClicked);
    b->setIcon(QIcon(QPixmap(":/task/images/add.png")));

    b = new QPushButton(this);
    m_b.push_back(b);
    connect(b, &QPushButton::clicked,this, &cGroupButtons::bLevelUpClicked);
    b->setIcon(QIcon(QPixmap(":/task/images/levelup.png")));
}

void cGroupButtons::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);

    emit FocusIn();
}

void cGroupButtons::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    // Your code here.

    int width=event->size().width();
    int height=event->size().height();
    if (width<height)
    {
        int top(0);
        int left(0);
        for (auto& it : m_b)
        {
            it->setGeometry(left,top,width,width);
            top+=width+1;
        }
    }
    else
    {
        int top(0);
        int left(width-height-1);
        for (auto& it : m_b)
        {
            it->setGeometry(left,top,height,height);
            left-=height-1;
        }
    }
    for (auto& it : m_b)
    {
        it->setIconSize(QSize(it->width(), it->height()));
    }
}

void cGroupButtons::bAddTaskClicked()
{
    emit AddTask();
}

void cGroupButtons::bLevelUpClicked()
{
    emit LevelUp();
}

//---------------------------------------------------------------------------------------------------cFocusedTaskPopUp

cFocusedTaskPopUp::cFocusedTaskPopUp(QWidget *parent)
    : QWidget(parent)
    , m_b()
{
    {
        QPushButton* b = new QPushButton(this);
        m_b.push_back(b);
        connect(b, &QPushButton::clicked,this, &cFocusedTaskPopUp::bDeleteTaskClicked);
        b->setIcon(QIcon(QPixmap(":/task/images/delete.png")));
    }
    {
        QPushButton* b = new QPushButton(this);
        m_b.push_back(b);
        connect(b, &QPushButton::clicked,this, &cFocusedTaskPopUp::bEditTaskClicked);
        b->setIcon(QIcon(QPixmap(":/task/images/edit.png")));
    }
    {
        QPushButton* b = new QPushButton(this);
        m_b.push_back(b);
        connect(b, &QPushButton::clicked,this, &cFocusedTaskPopUp::bAddTaskClicked);
        b->setIcon(QIcon(QPixmap(":/task/images/add.png")));
    }

    {
        QPushButton* b = new QPushButton(this);
        m_b.push_back(b);
        connect(b, &QPushButton::clicked,this, &cFocusedTaskPopUp::bViewTaskClicked);
        b->setIcon(QIcon(QPixmap(":/task/images/browser.png")));
    }
}

void cFocusedTaskPopUp::UpdateGeometry(QPoint point, int newHeight, int parentWidth, int parentHeight)
{
    /*QScreen* s=QGuiApplication::primaryScreen();
    s->primaryOrientation();
    qreal r = s->devicePixelRatio();
    int ah = 0.25 / r; // 0.25 inch ~ 1sm*/
    int width=newHeight * m_b.size() + m_b.size();
    if (point.x()+width > parentWidth)
    {
        point.setX(parentWidth - width - 10);
    }
    if (point.y() + newHeight > parentHeight)
    {
        point.setY(parentHeight - newHeight - 10);
    }
    this->setGeometry(point.x(), point.y(), width, newHeight);
    //move(point);
}

void cFocusedTaskPopUp::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    // Your code here.

    int width=event->size().width();
    int height=event->size().height();
    if (width<height)
    {
        int top(0);
        int left(0);
        for (auto& it : m_b)
        {
            it->setGeometry(left,top,width,width);
            top+=width+1;
        }
    }
    else
    {
        int top(0);
        int left(width-height-1);
        for (auto& it : m_b)
        {
            it->setGeometry(left,top,height,height);
            left-=height-1;
        }
    }
    for (auto& it : m_b)
    {
        it->setIconSize(QSize(it->width(), it->height()));
    }
}

void cFocusedTaskPopUp::bViewTaskClicked()
{
    emit ViewTask();
}

void cFocusedTaskPopUp::bEditTaskClicked()
{
    emit EditTask();
}

void cFocusedTaskPopUp::bDeleteTaskClicked()
{
    emit DeleteTask();
}

void cFocusedTaskPopUp::bAddTaskClicked()
{
    emit AddTask();
}
// -----------------------------------------------------------------------------------------ProjectModel
ProjectTableModel::ProjectTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

int ProjectTableModel::rowCount(const QModelIndex& parent) const
{
    return Projects.size();
}

int ProjectTableModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ProjectTableModel::data(const QModelIndex &index, int role) const
{
    if (role = Qt::DisplayRole)
    {

        QString value = QString::number(index.row());
        return value;
    }
    return QVariant();
}

// --------------------------------------------------------------------------------------cViewProjects
cViewProjects::cViewProjects(QWidget *parent)
    : QWidget(parent)
    , tv(Q_NULLPTR)
    , m_vLayoutMain()
    , m_hLayoutMain()
    , treeModel()
{
    tv=new QTreeView();

    // Create vertical layout
    m_vLayoutMain = new QVBoxLayout();
    // Set vertical layout margin to zero
    m_vLayoutMain->setMargin(0);

    // Create horizontal layout
    m_hLayoutMain = new QHBoxLayout();
    // Set horizontal layout to zero
    m_hLayoutMain->setMargin(0);
    m_hLayoutMain->setSpacing(0);

    // Add horizontal layout to vertical layout
    m_hLayoutMain->addLayout(m_vLayoutMain);

    m_vLayoutMain->addWidget(tv);
    //m_vLayoutMain->addWidget(m_wGroupButtons);

    setLayout(m_hLayoutMain);

    hide();

    connect(tv, &QTreeView::clicked, this, &cViewProjects::tvClicked);

    treeModel = new QStandardItemModel(this);

    tv->setModel(treeModel);
}

void cViewProjects::tvClicked(const QModelIndex& index)
{
    qDebug() << __FUNCTION__;
    QString s = index.data().toString();
    QStringList sl = s.split(" -> ");
    if (sl.size()>1)
    {
        emit projectSelected(sl[1]);
    }
}

void cViewProjects::tvPressed(const QModelIndex &index)
{
qDebug() << __FUNCTION__;
}

void cViewProjects::tvDoubleClicked(const QModelIndex &index)
{
qDebug() << __FUNCTION__;
}

void cViewProjects::tvActivated(const QModelIndex &index)
{
qDebug() << __FUNCTION__;
}

void cViewProjects::tvEntered(const QModelIndex &index)
{
qDebug() << __FUNCTION__;
}

void cViewProjects::tvViewportEntered()
{
qDebug() << __FUNCTION__;
}

void cViewProjects::setData(const TProjects& projects)
{
    treeModel->clear();

    QModelIndex rootIndex = QModelIndex();
    //treeModel->insertRow(0,rootIndex);
    treeModel->insertColumn(0,rootIndex);

    QModelIndex parentIndex = treeModel->index(0,0,rootIndex); // new item inside existing

    int r(-1);
    for (auto it : projects)
    {
        r++;
        treeModel->insertRow(r,rootIndex);
        //treeModel->insertColumn(r,rootIndex);

        parentIndex=treeModel->index(r,0,rootIndex);

        ProjectMetaType t;
        t.project = it;
        QVariant stored;
        stored.setValue(t);

        QString data=QString(it->RootArea()->paraCaption().c_str())+" -> "+it->RootArea()->UID();
        //treeModel->setData(parentIndex,stored);
        treeModel->setData(parentIndex, QVariant(data));

    }
    tv->setModel(treeModel);
}


