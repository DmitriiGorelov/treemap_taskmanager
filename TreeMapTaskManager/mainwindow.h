#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QScreen>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAbstractTableModel>
#include <QAbstractItemModel>

#include "../TreeMapQT/widget.h"
#include "../TreeMapTextEdit/textedit.h"
#include "dialogedittext.h"

#define Use3dTextEdit 1
#define UseGroupButtons 0

using namespace global_namespace;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class cGroupButtons : public QWidget
{
    Q_OBJECT
public:
    cGroupButtons(QWidget *parent);
    ~cGroupButtons()
    {

    }

protected:
    void focusInEvent(QFocusEvent *event) final;
    void resizeEvent(QResizeEvent* event) final;
    void paintEvent(QPaintEvent* event) final {}

private slots:
    void bAddTaskClicked();
    void bLevelUpClicked();

private:
signals:
    void FocusIn();
    void AddTask();
    void LevelUp();

private:
    std::vector<QPushButton*> m_b;
};

//----------------------------------------------------------------------------------------------------------------------TreeModel
class ProjectTableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    ProjectTableModel(QObject* parent);
    int rowCount(const QModelIndex& parent) const final;
    int columnCount(const QModelIndex& parent) const final;
    QVariant data(const QModelIndex& index, int role) const final;
private:
    TProjects Projects;
};

struct ProjectMetaType
{
    pTProject project;
};
Q_DECLARE_METATYPE(ProjectMetaType);

class cViewProjects : public QWidget
{
    Q_OBJECT
public:
    cViewProjects(QWidget *parent = nullptr);
    ~cViewProjects(){}

    void setData(const TProjects& projects);

protected:
signals:
    void projectSelected(const QString& uid);

private slots:
    void tvClicked(const QModelIndex& index);
    void tvPressed(const QModelIndex &index);
    void tvDoubleClicked(const QModelIndex &index);
    void tvActivated(const QModelIndex &index);
    void tvEntered(const QModelIndex &index);
    void tvViewportEntered();

private:
    QTreeView* tv;

    QVBoxLayout* m_vLayoutMain;
    QHBoxLayout* m_hLayoutMain;

    QStandardItemModel* treeModel;
    //ProjectTableModel* treeModel;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* event) final;    
    void paintEvent(QPaintEvent* event) final;

private slots:
    void primaryOrientationChanged(Qt::ScreenOrientation orientation);

    // Model
    void projectSelected(const QString& uid);

    // Widget
    void MapMousePressed(int x, int y, bool& canContinue);
    void FocusedTaskPopUp(int x, int y);

    // File
    void DataBaseNew();
    void DataBaseOpen();
    void DataBaseSave();
    void DataBaseSaveAs();

    // View
    void ViewProjects();
    void ViewTasks();

    // Projects
    void AddProject();

    // GroupButtons
    void FocusInGroupButtons();
    void AddTaskSelected();
    void LevelUp();
    void AddTaskFocused();
    void TaskVolumeChanged(int percent);

    // TextEdit
    void TextEditBeforeClose();

private:
    void ShowWindowEditTask();
    void ShowFocusedTaskPopUp(int x, int y);

    int ButtonSize();
    void SaveLastPath(const QString &fname);
    bool LoadLastPath(QString& fname);

private:
    Ui::MainWindow *ui;

    //cFocusedTaskPopUp* m_wFocusedTaskPopUp;

#if Use3dTextEdit
    TextEdit* m_wTextEdit;
#else
    cDialogEditText* m_wTextEdit;
#endif

    cViewProjects* wProjects;
    Widget* wTasks;
    int widthWasBigger;

    QVBoxLayout* m_vLayoutMain;
    QHBoxLayout* m_hLayoutMain;
#if UseGroupButtons
    cGroupButtons* m_wGroupButtons;
#else
    QToolBar *tbView;
    QToolBar *tbProjects;
    QToolBar *tbTasks;
#endif
    QScreen *s;
};
#endif // MAINWINDOW_H
