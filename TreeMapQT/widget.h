#ifndef WIDGET_H
#define WIDGET_H

#include "Areas.h"
#include "Projects.h"
#include "DataProvider/OscXmlInfo.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QTime>
#include <QPushButton>


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

protected:    
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent *) final;
    void mousePressEvent(QMouseEvent *event) final;
    void mouseReleaseEvent(QMouseEvent *event) final;
    pXMLParametrised AddOsc(const std::string& uid, pXMLParametrised& osc) final;
    std::list<pXMLParametrised> getRootOsc() final;

private slots:
    void MousePressTimer();

private:
signals:
    void MousePressed(int x, int y, bool& canContinue);

    void FocusedTaskPopUp(int x, int y);

private:
    Ui::Widget *ui;

    //TAreas Values;    
    TProjects Projects;
    pTProject pSelectedP;
    QPoint m_mousePressPoint;
    QTime m_mousePressStarted;

    QTimer* m_mousePressTimer;

    bool m_NeedCalculate;
    bool m_bMousePressed;

    //pTArea pSelectedA;

};

#endif // WIDGET_H

