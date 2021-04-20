#ifndef AREAS_H
#define AREAS_H

#if MSFT
#else
#define QT 1
#endif
#include <list>
#include <boost/smart_ptr.hpp>
#include "UID.h"
#include "DataProvider/XMLParametrisedOsc.h"
#include <QString>
#include <QPainter>
#include <QPoint>
#include <QStaticText>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

namespace global_namespace
{

struct TPoint{
        double x;
        double y;
    };

    using namespace GUI;

    class TArea : public cUID , public cXMLParametrisedOsc
    {
        //TArea(const TArea&&) = delete;
        TArea() = delete;
        //TArea& operator=(const TArea&) = delete;

    public:

        typedef boost::shared_ptr<TArea> pTArea;
        typedef std::list<pTArea> TAreas; // key is a value

        static boost::shared_ptr<TArea> Create(const std::string& uid, const boost::shared_ptr<TArea> parent)
        {
            return boost::make_shared<TArea>(uid.c_str(), parent);
        }

        static boost::shared_ptr<TArea> Create(double value, const boost::shared_ptr<TArea> parent)
        {
            return boost::make_shared<TArea>(value, parent);
        }

        TArea(QString uid, const boost::shared_ptr<TArea>& parent=nullptr)
            : cUID(uid)
            , cXMLParametrisedOsc(uid.toStdString())
            , TopLeft(0,0)
            , TopRight(0,0)
            , BottomLeft(0,0)
            , BottomRight(0,0)
            , Areas()
            , pParentArea(parent)
            , m_Highlight(false)
        {
            paraValue(100);
        }

        TArea(double _value, const boost::shared_ptr<TArea>& parent=nullptr)
            : cUID()
            , cXMLParametrisedOsc(UID().toStdString())
            , TopLeft(0,0)
            , TopRight(0,0)
            , BottomLeft(0,0)
            , BottomRight(0,0)
            , Areas()
            , pParentArea(parent)
            , m_Highlight(false)
        {
            if (_value==0.0)
                _value=100;
            paraValue(_value);
        }

        /*TArea(pXMLParametrisedOsc& para)
            : cUID(para->uid().c_str())
            , cXMLParametrisedOsc(*(para.get()))
            , Value(para->paraValue())
            , TopLeft({0,0})
            , TopRight({0,0})
            , BottomLeft({0,0})
            , BottomRight({0,0})
            , Areas()
        {
            if (Value==0.0)
                Value=100;
        }*/

        TArea(double _value, QPointF _topLeft, QPointF _topRight, QPointF _bottomLeft, QPointF _bottomRight, const boost::shared_ptr<TArea>& parent=nullptr)
            : cUID()
            , cXMLParametrisedOsc(UID().toStdString())
            , TopLeft(_topLeft)
            , TopRight(_topRight)
            , BottomLeft(_bottomLeft)
            , BottomRight(_bottomRight)
            , Areas()
            , pParentArea(parent)
            , m_Highlight(false)
        {
            if (_value==0.0)
                _value=100;
            paraValue(_value);
        }

        void addArea(pTArea& area)
        {
            Areas.push_back(area);
        }

		void cutLeft(TArea a)
		{
            TopRight.setX(a.TopLeft.x());
            BottomRight.setX(a.BottomLeft.x());
            paraValue(paraValue()-a.paraValue());
		}

		void cutTop(TArea a)
		{
            BottomLeft.setY(a.TopLeft.y());
            BottomRight.setY(a.TopRight.y());
            paraValue(paraValue()-a.paraValue());
		}

        double Sum()
        {
            double sum(0.0);
            for (auto& it : Areas)
            {
                sum+=it->paraValue();
            }
            return sum;
        }

        double Max()
        {
            double max(0.0);
            for (auto& it : Areas)
            {
                max=fmax(max,it->paraValue());
            }
            return max;
        }

        pTArea Select(int x, int y)
        {
            for (auto& it : Areas)
            {
                if (x>=it->TopLeft.x() && x<=it->TopRight.x() && y>=it->TopLeft.y() && y<=it->BottomRight.y())
                {
                    pTArea p=it->Select(x,y);
                    if (p)
                        return p;
                    else
                        return it;
                    break;
                }
            }
            return pTArea();
        }

        pTArea ParentA()
        {
            pTArea pa=pParentArea.lock();
            if (pa)
            {
                return pa;
            }
            else
                return pTArea();
        }

        void PasteTo(pTArea area)
        {
            if (area)
            {
                pParentArea=area;
            }
        }

        QString GetText()
        {
            return cXMLParametrisedOsc::paraText().c_str();
        }

        void SetText(const QString& str)
        {
            cXMLParametrisedOsc::paraText(str.toStdString());
        }

        std::list<pXMLParametrised> getChildrenOsc() final
        {
            std::list<pXMLParametrised> l;
            for (auto& it : Areas)
            {
                l.emplace_back(boost::dynamic_pointer_cast<cXMLParametrised>(it));
            }
            return l;
        }

        QRectF rect()
        {
            return QRectF(TopLeft,BottomRight);
        }

        void Calculate(QRectF rect, int level=0);
        void Paint(QPainter& painter, int level=0);

        void Highlight(bool value);
        bool Highlighted();

    protected:
        void resetRect();
        qreal DrawText(QPainter& painter, qreal border);
        QRectF textRec();
    public:

        //double Value;        
        QPointF TopLeft;
        QPointF TopRight;
        QPointF BottomLeft;
        QPointF BottomRight;

    //private:
        TAreas Areas;
        boost::weak_ptr<TArea> pParentArea;

        bool m_Highlight;
    };

    typedef TArea::pTArea pTArea;
    typedef TArea::TAreas TAreas;
    //typedef std::list<TArea> TAreas; // key is a value

    //const double X = 800;
    //const double Y = 600;

    void Calculate(TAreas& Values, double width, double height);

    void Calculate2(TAreas& Values, double ratio, double width, double height);
}
#endif // AREAS_H