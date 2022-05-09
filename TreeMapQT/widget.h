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
    void FocusSelected();

    QString GetFocusedText();
    void SetFocusedText(const QString& str);

    TProjects GetProjects() {return Projects;}
    bool SelectProject(const QString& uid);

    void FocusedTaskVolume(double value);
    void FocusedTaskUser(const QString value);
    void FocusedTaskColor(QColor col);
    void FousedTaskRows(int value);

    bool HideFocusedTaskPopUp();

    void ShowWindowEditTask();

    QStringList getUsers();

protected:    
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent *) final;

    void contextMenuEvent(QContextMenuEvent *event) final;
    void mousePressEvent(QMouseEvent *event) final;
    void mouseReleaseEvent(QMouseEvent *event) final;
    void mouseDoubleClickEvent(QMouseEvent *event) final;

    virtual pXMLParametrised AddOsc(const std::string& uid, pXMLParametrised& osc) final;
    virtual std::list<pXMLParametrised> getRootOsc() final;


private:
    void ShowFocusedTaskPopUp(QPoint point);


    int ButtonSize();

private slots:
    void ViewTask();
    void EditTask();
    void DeleteTask();


    void TextEditBeforeClose();
    void TaskVolumeChanged(int percent);        

private:
signals:
    void MousePressed(QPoint point, bool& canContinue);
    void FocusedTaskChanged(pTArea focused);

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
    QPoint m_mouseReleasePoint;
    pTArea m_DraggingTask;

    bool m_NeedCalculate;
    bool m_bMouseLeftPressed;
    bool m_bMouseRightPressed;
    cFocusedTaskPopUp* m_wFocusedTaskPopUp;

    //pTArea pSelectedA;

};

#endif // WIDGET_H

