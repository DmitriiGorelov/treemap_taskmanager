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
    , Projects()
    , pSelectedP(nullptr)
    , m_mousePressPoint(0,0)
    , m_mousePressStarted()
    , m_mousePressTimer(Q_NULLPTR)
    , m_NeedCalculate(true)
    , m_bMousePressed(false)
{
    ui->setupUi(this);

    m_mousePressTimer=new QTimer(this);
    connect(m_mousePressTimer, &QTimer::timeout, this, &Widget::MousePressTimer);

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
    repaint();
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
        repaint();
    }
}

void Widget::AddTaskFocused()
{
    if (SelectedP())
    {
        SelectedP()->AddTaskFocused(1.001);
        WriteXML();

        m_NeedCalculate=true;
        repaint();
    }
}

void Widget::LevelUp()
{
    if (SelectedP())
    {
        SelectedP()->LevelUp();
        m_NeedCalculate=true;
        repaint();
    }
}

void Widget::DeleteFocused()
{
    SelectedP()->DeleteFocused();
    WriteXML();

    m_NeedCalculate=true;
    repaint();
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
    repaint();
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
    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(0,0, this->width()-1, this->height()-1);

    if (SelectedP())
    {
        if (m_NeedCalculate)
        {
            SelectedP()->SelectedA()->Calculate(QRectF(0,0,this->width(), this->height()));
        }

        SelectedP()->SelectedA()->Paint(painter);

        return;
    }

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    m_bMousePressed = true;

    m_mousePressTimer->stop();
    m_mousePressPoint = event->pos();
    SelectedP()->Focus(m_mousePressPoint.x(), m_mousePressPoint.y());
    m_mousePressStarted = QTime::currentTime();

    bool canContinue(true);
    emit MousePressed(m_mousePressPoint.x(), m_mousePressPoint.y(), canContinue);

#ifndef Q_OS_ANDROID
    if (canContinue)
    {
        if (event->button() == Qt::MouseButton::LeftButton)
        {
            //m_mousePressTimer->start(500);
        }
        else
        {
            SelectedP()->Focus(m_mousePressPoint.x(), m_mousePressPoint.y());            
        }
    }
    else
    {
        m_bMousePressed=false;
    }
#else
    if (canContinue)
    {
        m_mousePressTimer->start(500);
    }
    else
    {
        m_bMousePressed=false;
    }
#endif
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    m_mousePressTimer->stop();

    if (!m_bMousePressed)
    {
        return;
    }

    QTime mousePressFinished = QTime::currentTime();

    int x=event->pos().x();
    int y=event->pos().y();

#ifndef Q_OS_ANDROID
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        if (SelectedP()->CanFocus(x,y) && SelectedP()->Focused())
        {
            if (SelectedP()->CanFocus(x,y)->UID()==SelectedP()->Focused()->UID())
            {
                SelectedP()->Select(m_mousePressPoint.x(), m_mousePressPoint.y());
                m_NeedCalculate = true;
                repaint();
            }
            else // mousePress and mouseRelease are pointing to different items. Is it drag-n-drop?
            {
                auto focusedOld = SelectedP()->Focused();
                auto selectedNew = SelectedP()->CanSelect(x, y);
                TArea::PasteTo(focusedOld, selectedNew);
                m_NeedCalculate = true;
                repaint();
            }
        }
    }
    else
    {
        if ((SelectedP()->CanFocus(x,y) && SelectedP()->Focused())&&
            (SelectedP()->CanFocus(x,y)->UID()==SelectedP()->Focused()->UID()))
        {
            SelectedP()->Focused()->Highlight(true);
            repaint();
            emit FocusedTaskPopUp(x,y);
        }
        else // mousePress and mouseRelease are pointing to different items. Is it drag-n-drop?
        {

        }
    }

#else
    if ((SelectedP()->CanFocus(x,y) && SelectedP()->Focused())&&
        (SelectedP()->CanFocus(x,y)->UID()==SelectedP()->Focused()->UID()))
    {
        if (m_mousePressStarted.msecsTo(mousePressFinished)<500)
        {
            SelectedP()->Select(m_mousePressPoint.x(), m_mousePressPoint.y());
            m_NeedCalculate = true;
            repaint();
        }
        /*else
        { // TODO: show pop-up window to select next action for the focused task
            emit FocusedTaskPopUp(x,y);
        }*/
    }
    else // mousePress and mouseRelease are pointing to different items. Is it drag-n-drop?
    {

    }
#endif
}

void Widget::MousePressTimer()
{
    m_mousePressTimer->stop();

    int x=this->mapFromGlobal(QCursor::pos()).x();
    int y=this->mapFromGlobal(QCursor::pos()).y();
    if ((SelectedP()->CanFocus(x,y) && SelectedP()->Focused())&&
        (SelectedP()->CanFocus(x,y)->UID()==SelectedP()->Focused()->UID()))
    {
        emit FocusedTaskPopUp(x,y);
    }
    else // mousePress and mouseRelease are pointing to different items. Is it drag-n-drop?
    {

    }

    //QMouseEvent *mEvnRelease = new QMouseEvent(QEvent::MouseButtonRelease, QCursor::pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    //QCoreApplication::sendEvent(QWidget::focusWidget(),mEvnRelease);
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

void Widget::ChangeFocusedVolume(double value)
{
    if (SelectedP()->Focused())
        SelectedP()->Focused()->SetValue(value);

    WriteXML();

    m_NeedCalculate = true;
    repaint();
}
