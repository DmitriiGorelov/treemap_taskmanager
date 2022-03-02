#include "FocusedTaskPopUp.h"

#include <QColorDialog>

cFocusedTaskPopUp::cFocusedTaskPopUp(QWidget *parent)
    : QWidget(parent)
    , m_b()
    , comboboxes(0)
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

    /*{
        QPushButton* b = new QPushButton(this);
        m_b.push_back(b);
        connect(b, &QPushButton::clicked,this, &cFocusedTaskPopUp::bViewTaskClicked);
        b->setIcon(QIcon(QPixmap(":/task/images/browser.png")));
    }*/

    {
        QSlider* b = new QSlider(this);
        m_b.push_back(b);
        b->setMinimum(1);
        b->setMaximum(200);
        b->setValue(100);

        connect(b, &QSlider::sliderMoved,this, &cFocusedTaskPopUp::sTaskVolumeChanged);
        //b->setIcon(QIcon(QPixmap(":/task/images/browser.png")));
    }

    {
        comboboxes++;
        QComboBox* b = new QComboBox(this);
        m_b.push_back(b);        
        connect(b, &QComboBox::currentTextChanged,this, &cFocusedTaskPopUp::sUserOfTaskChanged);
    }

    {
        QPushButton* b=new QPushButton(this);
        m_b.push_back(b);
        connect(b, &QPushButton::clicked, this, &cFocusedTaskPopUp::bTaskColorClicked);
        b->setIcon(QIcon(QPixmap(":/task/images/color.png")));
    }
}

void cFocusedTaskPopUp::UpdateGeometry(QPoint point, int newHeight, int parentWidth, int parentHeight)
{
    /*QScreen* s=QGuiApplication::primaryScreen();
    s->primaryOrientation();
    qreal r = s->devicePixelRatio();
    int ah = 0.25 / r; // 0.25 inch ~ 1sm*/
    int width=newHeight * (m_b.size()+comboboxes) + m_b.size();
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

void cFocusedTaskPopUp::SetValueRange(int min, int max, int value)
{
    for (auto& it : m_b)
    {
        auto o=qobject_cast<QSlider*>(it);
        if (o)
        {
            o->setMinimum(min);
            o->setMaximum(max);
            o->setValue(value);
            break;
        }
    }
}

void cFocusedTaskPopUp::SetUsers(const QString& user, const QStringList& allUsers)
{
    for (auto& it : m_b)
    {
        auto o=qobject_cast<QComboBox*>(it);
        if (o)
        {
            o->clear();
            o->addItems(allUsers);
            o->setCurrentText(user);
            break;
        }
    }
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
            qobject_cast<QWidget*>(it)->setGeometry(left,top,width,width);
            top+=width+1;
        }
    }
    else
    {
        int top(0);
        int left(0);
        auto cont=m_b;
        std::reverse(cont.begin(), cont.end());
        for (auto& it : cont)
        {
            auto mult=(qobject_cast<QComboBox*>(it))?2:1;
            qobject_cast<QWidget*>(it)->setGeometry(left,top,height*mult,height);
            left+=height*mult-1;
        }
    }
    for (auto& it : m_b)
    {
        auto o=qobject_cast<QPushButton*>(it);
        if (o)
            o->setIconSize(QSize(o->width(), o->height()));
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

void cFocusedTaskPopUp::bTaskColorClicked()
{
    QColor col = QColorDialog::getColor(Qt::white, this);
    if (!col.isValid())
        return;
    emit ColorTask(col);
}

void cFocusedTaskPopUp::sTaskVolumeChanged(int position)
{
    emit TaskVolumeChanged(position);
}

void cFocusedTaskPopUp::sUserOfTaskChanged(const QString & value)
{
    emit UserOfTaskChanged(value);
}
