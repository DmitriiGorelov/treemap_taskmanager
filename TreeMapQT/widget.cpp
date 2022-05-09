#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QPoint>
#include <QTime>
#include <QTimer>
#include <QStaticText>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QDebug>

Widget::Widget(const QString& fname, QWidget *parent)
    : QWidget(parent)
    , cOscXmlInfo(fname.toStdString())
    , ui(new Ui::Widget)
    , m_wTextEdit(Q_NULLPTR)
    , Projects()
    , pSelectedP(nullptr)
    , m_mousePressPoint(0,0)
    , m_mouseReleasePoint(0,0)
    , m_DraggingTask(nullptr)
    , m_NeedCalculate(true)
    , m_bMouseLeftPressed(false)
    , m_bMouseRightPressed(false)
    , m_wFocusedTaskPopUp(Q_NULLPTR)
{
    ui->setupUi(this);

    ReadXML();
}

Widget::~Widget()
{
    WriteXML();
    delete ui;
}

bool Widget::ReadXML(const QString& fname)
{
    clear();
    return readXml(fname.toStdString());
}

void Widget::ReadXML()
{
    clear();
    readXml();
}

void Widget::WriteXML()
{
    modifyXml();
}

bool Widget::WriteXML(const QString& fname)
{
    return modifyXml(fname.toStdString());
}

void Widget::clear()
{
    pSelectedP.reset();
    Projects.clear();
    m_NeedCalculate=true;
    update();
}

void Widget::AddProject(const QString& caption)
{
    pTProject p = TProject::Create();
    p->RootArea()->paraCaption(caption.toStdString());
    Projects.emplace_back(p);
    //pSelectedP = p;
}

void Widget::AddTaskSelected()
{
    if (SelectedP())
    {        
        SelectedP()->AddTaskSelected(1.001);
        WriteXML();

        m_NeedCalculate=true;        
        update();
    }
}

void Widget::AddTaskFocused()
{
    if (SelectedP())
    {
        SelectedP()->Focused()->Highlight(false);
        SelectedP()->AddTaskFocused(1.001);
        WriteXML();                

        m_NeedCalculate=true;        
        update();
    }
}

void Widget::LevelUp()
{
    if (SelectedP())
    {
        SelectedP()->LevelUp();
        m_NeedCalculate=true;
        update();
    }
}

void Widget::DeleteFocused()
{
    SelectedP()->DeleteFocused();
    WriteXML();

    m_NeedCalculate=true;
    update();
}

void Widget::FocusSelected()
{
    auto ptr=SelectedP()->SelectedA();
    if (ptr)
    {
        SelectedP()->Focus(ptr);
        emit FocusedTaskChanged(ptr);
    }
}

pTProject Widget::SelectedP()
{
    if (!pSelectedP)
    {
        if (Projects.size()>0)
        {
            pSelectedP=*Projects.begin();
        }
        else
        {
            pSelectedP=boost::make_shared<TProject>();
            Projects.emplace_back(pSelectedP);
        }
    }
    return pSelectedP;
}

QString Widget::GetFocusedText()
{
    if (SelectedP()->Focused())
        return SelectedP()->Focused()->GetText();
    else
        return "";
}

void Widget::SetFocusedText(const QString& str)
{
    if (SelectedP()->Focused())
        SelectedP()->Focused()->SetText(str);

    WriteXML();

    m_NeedCalculate = true;
    update();
}

void Widget::resizeEvent(QResizeEvent* event)
{
    m_NeedCalculate=true;

    QWidget::resizeEvent(event);
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(0,0, this->width()-1, this->height()-1);

    if (SelectedP())
    {
        if (m_NeedCalculate)
        {
            SelectedP()->SelectedA()->Calculate("",1,QRectF(0,0,this->width(), this->height()));
        }

        SelectedP()->SelectedA()->Paint(painter);

        return;
    }

}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    if (!m_bMouseRightPressed)
        return;

    m_mousePressPoint = event->pos();

    if (SelectedP()->CanFocus(m_mousePressPoint))
    {
        auto ptr = SelectedP()->Focus(m_mousePressPoint);

        if (ptr)
        {
            ptr->Highlight(true);
            update();
            ShowFocusedTaskPopUp(m_mousePressPoint);
        }
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    HideFocusedTaskPopUp();

    m_mousePressPoint = event->pos();

    if (event->button() == Qt::MouseButton::LeftButton)
    {
        m_bMouseLeftPressed = true;
        m_bMouseRightPressed = false;

        auto oldptr = SelectedP()->Focused();
        if (oldptr)
            oldptr->Highlight(false);

        m_DraggingTask = SelectedP()->Focus(m_mousePressPoint);
        if (m_DraggingTask)
        {
            m_DraggingTask->Highlight(true);
            emit FocusedTaskChanged(m_DraggingTask);
        }
    }

    if (event->button() == Qt::MouseButton::RightButton)
    {
        m_bMouseLeftPressed = false;
        m_bMouseRightPressed = true;

        m_DraggingTask = SelectedP()->Focus(m_mousePressPoint);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseReleasePoint=event->pos();

    if (event->button() == Qt::MouseButton::LeftButton && m_bMouseLeftPressed)
    {        
        if (SelectedP()->CanFocus(m_mouseReleasePoint) && SelectedP()->Focused())
        {
            // mousePress and mouseRelease are pointing to different items. Is it drag-n-drop?
            if (SelectedP()->CanFocus(m_mouseReleasePoint)->UID()!=SelectedP()->Focused()->UID())
            {
                auto focusedOld = SelectedP()->Focused();
                auto selectedNew = SelectedP()->CanSelect(m_mouseReleasePoint);
                TArea::PasteTo(focusedOld, selectedNew);
                m_NeedCalculate = true;

                update();
            }
        }

        m_bMouseLeftPressed=false;

        return;
    }

    if (event->button() == Qt::MouseButton::RightButton && m_bMouseRightPressed)
    {
        auto neighbour = SelectedP()->CanFocus(m_mouseReleasePoint);
        auto root = m_DraggingTask->ParentA();
        if (neighbour && m_DraggingTask && neighbour->ParentA() == root)
        {
            if (root)
            {
                root->Reorder(m_DraggingTask, neighbour);
            }
        }

        m_bMouseRightPressed=false;
        m_NeedCalculate=true;
        update();
    }
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_bMouseLeftPressed=false;
    m_bMouseRightPressed=false;

    m_mouseReleasePoint=event->pos();

    SelectedP()->Select(m_mouseReleasePoint);
    m_NeedCalculate = true;
    update();
}

pXMLParametrised Widget::AddOsc(const std::string& uid, pXMLParametrised& osc)
{
    m_NeedCalculate = true;
    if (osc)
    {
        pTArea pa(boost::dynamic_pointer_cast<TArea>(osc));
        if (pa)
        {
            pTArea a=TArea::Create(uid,pa);
            pa->addArea(a);
            return boost::dynamic_pointer_cast<cXMLParametrised>(a);
        }
    }
    else
    {
        pTProject pp=TProject::Create(uid);
        Projects.push_back(pp);
        return boost::dynamic_pointer_cast<cXMLParametrised>(pp->RootArea());
    }
}

std::list<pXMLParametrised> Widget::getRootOsc()
{
    std::list<pXMLParametrised> l;
    for (auto& it : Projects)
    {
        l.emplace_back(boost::dynamic_pointer_cast<cXMLParametrised>(it->RootArea()));
    }
    return l;
}

QStringList Widget::getUsers()
{
    return SelectedP()->getUsers();
}

bool Widget::SelectProject(const QString& uid)
{
    for (auto& it : Projects)
    {
        if (it->RootArea()->UID() == uid)
        {
            pSelectedP=it;
            return true;
        }
    }
    return false;
}

void Widget::FocusedTaskVolume(double value)
{
    if (SelectedP()->Focused())
        SelectedP()->Focused()->SetValue(value);

    WriteXML();

    m_NeedCalculate = true;
    update();
}

void Widget::ShowFocusedTaskPopUp(QPoint point)
{
    if (!m_wFocusedTaskPopUp)
    {
        m_wFocusedTaskPopUp=new cFocusedTaskPopUp(this);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::EditTask, this, &Widget::EditTask);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::DeleteTask, this, &Widget::DeleteTask);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::ColorTask, this, &Widget::FocusedTaskColor);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::ViewTask, this, &Widget::ViewTask);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::AddTask, this, &Widget::AddTaskFocused);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::TaskVolumeChanged, this, &Widget::TaskVolumeChanged);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::TaskRowsChanged, this, &Widget::FousedTaskRows);
        connect(m_wFocusedTaskPopUp, &cFocusedTaskPopUp::UserOfTaskChanged, this, &Widget::FocusedTaskUser);
    }

    //QPoint pos = mapToGlobal(QPoint(x, y));
    m_wFocusedTaskPopUp->UpdateGeometry(point, ButtonSize(), width(), height());
    auto focused = SelectedP()->Focused()->GetValue();
    auto max = SelectedP()->Focused()->ParentA()->Max()*2.0;
    auto min = SelectedP()->Focused()->ParentA()->Max()/10.0;
    m_wFocusedTaskPopUp->SetValueRange(min, max, focused);

    QStringList all=getUsers();
    all.push_front("");
    m_wFocusedTaskPopUp->SetUsers(SelectedP()->Focused()->GetUser(), all);

    m_wFocusedTaskPopUp->SetRows(SelectedP()->Focused()->GetRows());

    m_wFocusedTaskPopUp->show();
}


int Widget::ButtonSize()
{
    static int rx = logicalDpiX();
    static int ry = logicalDpiY();
    static float isize = 0.5; //0.5 inch
    return rx*isize;
}

bool Widget::HideFocusedTaskPopUp()
{
    bool result(false);
    if (m_wFocusedTaskPopUp)
    {
        result = m_wFocusedTaskPopUp->isVisible();
        m_wFocusedTaskPopUp->hide();
    }
    if (SelectedP()->Focused())
        SelectedP()->Focused()->Highlight(false);

    update();
    return result;
}

void Widget::TaskVolumeChanged(int value)
{
    //HideFocusedTaskPopUp();
    FocusedTaskVolume(value);
}

void Widget::FocusedTaskUser(const QString value)
{
    //HideFocusedTaskPopUp();

    if (SelectedP()->Focused())
        SelectedP()->Focused()->SetUser(value);

    WriteXML();

    m_NeedCalculate = true;
    update();
}

void Widget::FousedTaskRows(int value)
{
    if (SelectedP()->Focused())
        SelectedP()->Focused()->SetRows(value);

    WriteXML();

    m_NeedCalculate = true;
    update();
}

void Widget::TextEditBeforeClose()
{
    if (m_wTextEdit && m_wTextEdit->Accepted())
    {
        SetFocusedText(m_wTextEdit->GetContent());
    }

    SelectedP()->Focused()->Highlight(false);
    update();

}

void Widget::ShowWindowEditTask()
{
#if Use3dTextEdit
    if (!m_wTextEdit)
    {
        m_wTextEdit = new TextEdit(this);
        connect(m_wTextEdit, &TextEdit::BeforeClose, this, &Widget::TextEditBeforeClose);
    }
    m_wTextEdit->ShowIt(GetFocusedText());

#else
    if (!m_wTextEdit)
    {
        m_wTextEdit = new cDialogEditText(this);
        //connect(m_wTextEdit, &cDialogEditText::BeforeClose, this, &Widget::TextEditBeforeClose);
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

void Widget::ViewTask()
{
    HideFocusedTaskPopUp();
}

void Widget::EditTask()
{
    HideFocusedTaskPopUp();

    ShowWindowEditTask();
}

void Widget::DeleteTask()
{
    HideFocusedTaskPopUp();
    DeleteFocused();
}

void Widget::FocusedTaskColor(QColor col)
{
    if (SelectedP()->Focused())
        SelectedP()->Focused()->SetColor(col);
}
