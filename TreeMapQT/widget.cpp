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

        //return;


        // ----------------------------------------------------------------------------------- old style, not oop

        TAreas Values;
        Values = SelectedP()->SelectedA()->Areas;

        if (m_NeedCalculate)
        {
            Calculate2(Values,2,this->width(), this->height());
            //m_NeedCalculate = false;
        }

        //painter.setPen(Qt::yellow);
        painter.setPen(QPen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap));

        for (auto& it : Values)
        {
            painter.setBrush(QBrush(it->paraColor(), Qt::SolidPattern));
            painter.drawRect(it->TopLeft.x(),it->TopLeft.y(), it->BottomRight.x()-it->TopLeft.x(), it->BottomRight.y()-it->TopLeft.y());
        }

        painter.setPen(Qt::black);
        for (auto& it : Values)
        {
            // debug UID
            //painter.drawText(it->TopLeft.x+10, it->TopLeft.y+10, it->UID());
            //painter.drawText(it->TopLeft.x+10, it->TopLeft.y+30, QString::number(it->paraValue()));
            //painter.drawText(it->TopLeft.x+10, it->TopLeft.y+30, it->paraText().c_str());

            QRectF rec = QRectF(QPointF(it->TopLeft.x(),it->TopLeft.y()), QSizeF(it->BottomRight.x()-it->TopLeft.x(), it->BottomRight.y()-it->TopLeft.y()));

            // 1
            QStaticText text;
            text.setTextWidth(rec.width());
            text.setText(it->paraText().c_str());
            painter.setClipRect(rec);
            painter.drawStaticText(rec.topLeft().x()+10,rec.topLeft().y(), text);
            QSizeF s=text.size();
            //qDebug() << " square=" << s.width()*s.height() << " width=" << s.width() << " height=" << s.height();

            //painter.drawLine(it->TopLeft.x,it->TopLeft.y+s.height(),it->TopRight.x,it->TopRight.y+s.height());

            TAreas Values2 = it->Areas;
            if (m_NeedCalculate)
            {
                Calculate2(Values2,2,it->BottomRight.x()-it->TopLeft.x(), it->BottomRight.y()-it->TopLeft.y()-s.height());
                //m_NeedCalculate = false;
            }

            painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
            painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
            for (auto& it2 : Values2)
            {
                painter.drawRect(it2->TopLeft.x()+it->TopLeft.x()+3,it2->TopLeft.y()+it->TopLeft.y()+s.height()+3, it2->BottomRight.x()-it2->TopLeft.x()-6, it2->BottomRight.y()-it2->TopLeft.y()-6);

            }

            painter.setPen(Qt::black);
            for (auto& it2 : Values2)
            {
                QRectF rec2 = QRectF(QPointF(it2->TopLeft.x()+it->TopLeft.x()+3,it2->TopLeft.y()+it->TopLeft.y()+s.height()+3), QSizeF(it2->BottomRight.x()-it2->TopLeft.x()-6, it2->BottomRight.y()-it2->TopLeft.y()-6));

                // 1
                QStaticText text;
                text.setTextWidth(rec2.width());

                text.setText(it2->paraText().c_str());
                painter.setClipRect(rec2);
                painter.drawStaticText(rec2.topLeft().x()+10,rec2.topLeft().y(), text);

                QSizeF s2=text.size();
                //qDebug() << " square=" << s.width()*s.height() << " width=" << s.width() << " height=" << s.height();

                TAreas Values3 = it2->Areas;
                if (m_NeedCalculate)
                {
                    Calculate2(Values3,2,it2->BottomRight.x()-it2->TopLeft.x(), it2->BottomRight.y()-it2->TopLeft.y()-s2.height()/*-s.height()*/);
                    //m_NeedCalculate = false;
                }

                painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
                painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
                for (auto& it3 : Values3)
                {
                    painter.drawRect(it3->TopLeft.x()+it2->TopLeft.x()+it->TopLeft.x()+3,it3->TopLeft.y()+it2->TopLeft.y()+it->TopLeft.y()+s2.height()+s.height()+3, it3->BottomRight.x()-it3->TopLeft.x()-6, it3->BottomRight.y()-it3->TopLeft.y()-6);
                }

                painter.setPen(Qt::black);
                for (auto& it3 : Values3)
                {
                    QRectF rec3 = QRectF(QPointF(it3->TopLeft.x()+it2->TopLeft.x()+it->TopLeft.x()+3,it3->TopLeft.y()+it2->TopLeft.y()+it->TopLeft.y()+s2.height()+s.height()+3), QSizeF(it3->BottomRight.x()-it3->TopLeft.x()-6, it3->BottomRight.y()-it3->TopLeft.y()-6));

                    // 1
                    QStaticText text;
                    text.setTextWidth(rec3.width());
                    text.setText(it3->paraText().c_str());
                    painter.setClipRect(rec3);
                    painter.drawStaticText(rec3.topLeft().x()+10,rec3.topLeft().y(), text);
                }
            }

            //QRectF realRect = ::boundingRect();

            // 2
            //painter.drawText(rec, Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap, it->paraText().c_str());


            // 3
            // draw rich text:
            /*painter.save();
            QTextDocument doc;
            doc.setHtml( it->paraText().c_str() );
            QAbstractTextDocumentLayout::PaintContext context;
            doc.setPageSize( rec.size());
            painter.setClipRect(rec);
            painter.translate(rec.x(), rec.y());
            doc.documentLayout()->draw(&painter, context);
            painter.restore();*/
        }

        m_NeedCalculate=false;
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
        if ((SelectedP()->CanFocus(x,y) && SelectedP()->Focused())&&
            (SelectedP()->CanFocus(x,y)->UID()==SelectedP()->Focused()->UID()))
        {
            SelectedP()->Select(m_mousePressPoint.x(), m_mousePressPoint.y());
            m_NeedCalculate = true;
            repaint();
        }
        else // mousePress and mouseRelease are pointing to different items. Is it drag-n-drop?
        {

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
