#ifndef WIDGET_H
#define WIDGET_H

#include "Areas.h"
#include "Projects.h"
#include "DataProvider/OscXmlInfo.h"
#include "FocusedTaskPopUp.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QTime>
#include <QPushButton>

#include "../TreeMapTextEdit/textedit.h"

#define Use3dTextEdit 1
#define UseGroupButtons 0

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

using namespace global_namespace;
class Widget : public QWidget, GUI::cOscXmlInfo
{
    Q_OBJECT

public:
    Widget(const QString& fname, QWidget *parent = nullptr);
    ~Widget();

    pTProject SelectedP();

    void clear();
    bool ReadXML(const QString& fname);
    void ReadXML();
    bool WriteXML(const QString& fname);
    void WriteXML();

    void AddProject(const QString& caption);

    void AddTaskSelected();
    void AddTaskFocused();
    void LevelUp();
    void DeleteFocused();

    QString GetFocusedText();
    void SetFocusedText(const QString& str);

    TProjects GetProjects() {return Projects;}
    bool SelectProject(const QString& uid);

    void SetFocusedVolume(double value);

    bool HideFocusedTaskPopUp();

protected:    
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent *) final;
    void mousePressEvent(QMouseEvent *event) final;
    void mouseReleaseEvent(QMouseEvent *event) final;
    virtual pXMLParametrised AddOsc(const std::string& uid, pXMLParametrised& osc) final;
    virtual std::list<pXMLParametrised> getRootOsc() final;
    virtual std::list<std::string> getUsers() final;
    virtual void addUser(const std::string& user) final;

private:
    void ShowFocusedTaskPopUp(int x, int y);
    void ShowWindowEditTask();

    int ButtonSize();

private slots:
    void MousePressTimer();

    void ViewTask();
    void EditTask();
    void DeleteTask();

    void TextEditBeforeClose();
    void TaskVolumeChanged(int percent);
    void UserOfTaskChanged(const QString value);

private:
signals:
    void MousePressed(int x, int y, bool& canContinue);

    //void FocusedTaskPopUp(int x, int y);

private:
    Ui::Widget *ui;

#if Use3dTextEdit
    TextEdit* m_wTextEdit;
#else
    cDialogEditText* m_wTextEdit;
#endif

    //TAreas Values;    
    TProjects Projects;
    pTProject pSelectedP;
    QPoint m_mousePressPoint;
    QTime m_mousePressStarted;

    QTimer* m_mousePressTimer;

    bool m_NeedCalculate;
    bool m_bMousePressed;
    cFocusedTaskPopUp* m_wFocusedTaskPopUp;

    QStringList m_Users;

    //pTArea pSelectedA;

};

#endif // WIDGET_H

