#include "Areas.h"
#if QT
#include <QDebug>
#endif
#include <QPainter>

#define ShowCurrentTask 1

using namespace std;

namespace global_namespace
{
// base data for claculation method 1
class TData {
public:
    TData(pTArea _area)
        : area(_area)
	{

	}

	bool operator <(const TData & o) const
	{
        return area->paraValue() < o.area->paraValue();
	}

    void Calculate(const pTArea& gArea)
	{
        ax = (area->TopRight.x() - area->TopLeft.x());
        by = (area->BottomLeft.y() - area->TopLeft.y());

        ratioToArea = area->paraValue() / (1.0*gArea->paraValue());

		place = ax*by;
	}

	double ratioSize()
	{
		if (ax == 0.0 || by == 0.0)
		{
			return 0.0;
		}
		if (ax > by)
			return ax / (1.0*by);
		else
			return by / (1.0*ax);

	}

    pTArea area;
	double ratioToArea;
	double place;
	double ax;
	double by;
};

typedef std::list< TData > tref;

// calculation method 1
tref srt(tref& aref, pTArea& aArea)
{
    if (aArea->paraValue() < 1)
    {
        return tref();
    }

    auto fmin=[](double a, double b){if (a<b) return a; else return b;};
    auto fmax=[](double a, double b){if (a>b) return a; else return b;};

    double X(aArea->TopRight.x() - aArea->TopLeft.x());
    double Y(aArea->BottomRight.y() - aArea->TopRight.y());
    if (X > Y)
    {
        double dx(0);
        //auto it = aref.begin();
        //for (it = aref.begin(); it != aref.end(); it++)
        for (auto& it : aref)
        {
            (it).area->TopRight = { aArea->TopRight.x() - dx, aArea->TopRight.y() };
            (it).area->BottomRight = { aArea->BottomRight.x() - dx, aArea->BottomRight.y() };

            dx += X*(it).area->paraValue() / (1.0*aArea->paraValue());

            (it).area->TopLeft = { aArea->TopRight.x() - dx, aArea->TopRight.y() };
            (it).area->BottomLeft = { aArea->BottomRight.x() - dx,  aArea->BottomLeft.y() };

            (it).Calculate(aArea); // Recalc
        }
    }
    else
    {
        double dy(0);
        auto it = aref.begin();
        for (it = aref.begin(); it != aref.end(); it++)
        {
            (*it).area->BottomRight = { aArea->BottomRight.x(), aArea->BottomRight.y() - dy };
            (*it).area->BottomLeft = { aArea->BottomLeft.x(), aArea->BottomLeft.y() - dy};

            dy += Y*(*it).area->paraValue() / (1.0*aArea->paraValue());

            (*it).area->TopRight = { (*it).area->BottomRight.x(), aArea->BottomRight.y() - dy };
            (*it).area->TopLeft = { (*it).area->BottomLeft.x(),  aArea->BottomLeft.y() - dy};

            (*it).Calculate(aArea); // Recalc
        }
    }

    //return tref();

    pTArea gArea=boost::make_shared<TArea>(*aArea);
    gArea->paraValue(0);
    tref ref2;

    bool found(false);
    auto it = aref.begin();
    for (it = aref.begin(); it != aref.end(); it++)
    {
        if ((*it).ratioSize() > 2)
        {
            if (gArea->paraValue() == 0)
            {
                gArea = boost::make_shared<TArea>(0, (*it).area->TopLeft, (*it).area->TopRight, (*it).area->BottomLeft, (*it).area->BottomRight );
            }

            found = true;
            gArea->paraValue(gArea->paraValue()+(*it).area->paraValue());

            gArea->TopLeft={ fmin((*it).area->TopLeft.x(), gArea->TopLeft.x()), fmax((*it).area->TopLeft.y(), gArea->TopLeft.y()) };
            gArea->TopRight = { fmax((*it).area->TopRight.x(), gArea->TopRight.x()), fmin((*it).area->TopRight.y(), gArea->TopRight.y()) };
            gArea->BottomLeft = { fmin((*it).area->BottomLeft.x(), gArea->BottomLeft.x()), fmax((*it).area->BottomLeft.y(), gArea->BottomLeft.y()) };
            gArea->BottomRight = { fmax((*it).area->BottomRight.x(), gArea->BottomRight.x()), fmax((*it).area->BottomRight.y(), gArea->BottomRight.y())};

            ref2.push_back((*it));
        }
        else
        {
            if (found)
            {
                aArea = gArea;
                // calculate with ref2
                return ref2;
            }
        }
    }

    return ref2;
}

void Calculate(TAreas& Values, double width, double height)
{
	tref ref;    
	for (auto& it : Values)
	{
        ref.emplace_back(TData(it));
	}

	//auto b = [](TData a, TData b) {return a.aArea->Value < b.aArea->Value; };

	ref.sort();

	double sum(0.0);
	for (auto& it : ref)
	{
        sum += it.area->paraValue();
	}

    pTArea gArea = boost::make_shared<TArea>(TArea(sum, { 0, 0 }, { width - 1, 0 }, { 0, height - 1 }, { width - 1, height - 1 }));

	size_t prevSize = ref.size();
	tref r = srt(ref, gArea);	
	while (r.size() < prevSize)
	{
		prevSize = r.size();
		r = srt(r, gArea);
	}
}

//------------------------------------------------------------------------------------------------------------------

// Base data for calcultion method 2
class TData2 {
public:
	TData2(tref& _ref, bool _vertical)
		: ref(_ref)
        , area(boost::make_shared<TArea>(TArea(0.0)))
		, vertical(_vertical)
	{
		double sum(0.0);
		for (auto& ait2 : ref)
		{
            sum += ait2.area->paraValue();
		}

		if (ref.size() > 0)
		{
			if (!vertical)
			{
                area = boost::make_shared<TArea>(TArea(sum, { ref.back().area->TopLeft.x(), ref.back().area->TopLeft.y() },
                { ref.back().area->TopRight.x(), ref.back().area->TopRight.y() },
                { ref.front().area->BottomLeft.x(), ref.front().area->BottomLeft.y() },
                { ref.front().area->BottomRight.x(), ref.front().area->BottomRight.y() }));
			}
			else
			{
                area = boost::make_shared<TArea>(TArea(sum, { ref.back().area->TopLeft.x(), ref.back().area->TopLeft.y() },
                { ref.front().area->TopRight.x(), ref.front().area->TopRight.y() },
                { ref.back().area->BottomLeft.x(), ref.back().area->BottomLeft.y() },
                { ref.front().area->BottomRight.x(), ref.front().area->BottomRight.y() }));
			}
		}				
	}

    TData2(tref& _ref, pTArea& _area)
		: ref(_ref)
		, area(_area)
		, vertical(true)
	{
		double sum(0.0);
		for (auto& ait2 : ref)
		{
            sum += ait2.area->paraValue();
		}

        double X(area->TopRight.x() - area->TopLeft.x());
        double Y(area->BottomRight.y() - area->TopRight.y());

		vertical = X > Y;
	}

	void cutLeft(tref::iterator& it)
	{
		if(it == ref.end())
		{
			return;
		}
		//it++;

        auto it2=ref.begin();
        for (it2 = ref.begin(); it2!=it; it2++)
		{
            area->cutLeft(*(*it2).area);
		}
		ref.erase(ref.begin(), it);
	}

	void cutTop(tref::iterator& it)
	{
		if (it == ref.end())
		{
			return;
		}
		//it++;

        auto it2=ref.begin();
        for (it2 = ref.begin(); it2 != it; it2++)
		{
            area->cutTop(*(*it2).area);
		}
		ref.erase(ref.begin(), it);
	}

	tref ref;
    pTArea area;
	bool vertical;
};

typedef std::list< TData2 > tref2;


void srt2(TData2& data) // result should contain 2 elements - result of splitting Area onto 2 pcs.
{	
    double X(data.area->TopRight.x() - data.area->TopLeft.x());
    double Y(data.area->BottomRight.y() - data.area->TopRight.y());
	if (data.vertical)
	{
		double dx(0);
        //for (auto& it = data.ref.begin(); it != data.ref.end(); it++)
        for (auto& it : data.ref)
		{
            (it).area->TopRight = { data.area->TopRight.x() - dx, data.area->TopRight.y() };
            (it).area->BottomRight = { data.area->BottomRight.x() - dx, data.area->BottomRight.y() };

            dx += X*(it).area->paraValue() / (1.0*data.area->paraValue());

            (it).area->TopLeft = { data.area->TopRight.x() - dx, data.area->TopRight.y() };
            (it).area->BottomLeft = { data.area->BottomRight.x() - dx,  data.area->BottomLeft.y() };

            (it).Calculate(data.area); // Recalc
		}
	}
	else
	{
		double dy(0);
        //for (auto& it = data.ref.begin(); it != data.ref.end(); it++)
        for (auto& it : data.ref)
		{
            (it).area->BottomRight = { data.area->BottomRight.x(), data.area->BottomRight.y() - dy };
            (it).area->BottomLeft = { data.area->BottomLeft.x(), data.area->BottomLeft.y() - dy };

            dy += Y*(it).area->paraValue() / (1.0*data.area->paraValue());

            (it).area->TopRight = { (it).area->BottomRight.x(), data.area->BottomRight.y() - dy };
            (it).area->TopLeft = { (it).area->BottomLeft.x(),  data.area->BottomLeft.y() - dy };

            (it).Calculate(data.area); // Recalc
        }
    }
}

void Calculate2(TAreas& Values, double ratio, double width, double height)
{
	tref ref;

    if (Values.size()<1)
        return;

	for (auto& it : Values)
	{
        ref.emplace_back(TData(it));
	}

    auto b = [](TData a, TData b) {return a.area->paraValue() < b.area->paraValue(); };

	ref.sort(b);

	double sum(0.0);
	for (auto& it : ref)
	{
        sum += it.area->paraValue();
	}

    pTArea gArea=boost::make_shared<TArea>(TArea(sum, { 0, 0 }, { width - 1, 0 }, { 0, height - 1 }, { width - 1, height - 1 }));

	tref2 ref2;
	ref2.emplace_back(TData2(ref, gArea));	
	size_t prevSize = ref2.size();

	auto it = ref2.begin();
	while (it!=ref2.end())
	{
        double X((*it).area->TopRight.x() - (*it).area->TopLeft.x());
        double Y((*it).area->BottomRight.y() - (*it).area->TopRight.y());

		(*it).vertical = X > Y;

		srt2(*it);

		auto ait = (*it).ref.begin();
		std::advance(ait, (*it).ref.size() - 1);

		if ((*ait).ratioToArea > 0.5)
		{
			/*if ((*it).vertical)
			{
				(*it).cutLeft(ait);
			}
			else
			{
				(*it).cutTop(ait);
			}*/

			if ((*it).ref.size() > 1)
			{
				// keep to iterate one more time
			}
			else
			{
				auto it2 = it;
				it++;
				ref2.erase(it2);
				continue;
			}
		}
		//else
		{
			if ((*it).vertical)
			{
                X /= ratio;
				for (auto ait= (*it).ref.begin(); ait!= (*it).ref.end(); ait++)
				{
                    if ((*ait).area->TopLeft.x() <= (*it).area->TopRight.x() - X) // we split here
					{
						if (ait == (*it).ref.begin())
						{
							// ??
						}

						tref ref = { (*it).ref.begin(), ait }; // right side
						
						(*it).cutLeft(ait);
						
						if (ref.size() > 0)
						{
							ref2.emplace(it, TData2(ref, (*it).vertical));
							it--;
						}
						else
						{
							it++;
						}

						break;
					}
				}
			}
			else
			{
                Y /= ratio;
				for (auto ait = (*it).ref.begin(); ait != (*it).ref.end(); ait++)
				{
                    if ((*ait).area->TopRight.y() <= (*it).area->BottomRight.y() - Y) // we split here
					{
						if (ait == (*it).ref.begin())
						{
							// ??
						}

						tref ref = { (*it).ref.begin(), ait }; // right side

						(*it).cutTop(ait);
						if (ref.size() > 0)
						{
							ref2.emplace(it, TData2(ref, (*it).vertical));
							it--;
						}
						else
						{
							it++;
						}

						break;
					}
				}
			}
		}
	}
}

QRectF TArea::textRec()
{
    return QRectF();
}

inline int border(int level)
{
    switch(level)
    {
        case 0: return 0;
        case 1:return 0;
        default:return 3;
    }
}

inline int line(int level)
{
    switch(level)
    {
        case 0: return 4;
        case 1:return 4;
        default:return 1;
    }
}

inline int wid(int level)
{
    switch(level)
    {
        case 0: return 0;
        case 1:return 0;
        default:return 2;
    }
}

QString colorizeText(const char* text, QColor col)
{
    //auto colorString=QString("%1").arg(QColor(col).rgb(), 6, 16);
    auto colorString=col.name().remove("#");

    QString result = R"bracket(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
<html><head><meta name="qrichtext" content="1" /><style type="text/css">
p, li { white-space: pre-wrap; }
</style></head><body style=" font-family:'Helvetica'; font-size:8.25pt; font-weight:400; font-style:normal;">
<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" color:#)bracket"+
colorString+
R"bracket(;">
)bracket";
    result+=text;
    result+="</span></p></body></html>";
    return result;
}

qreal TArea::DrawText(const QRectF& rect, QPainter& painter, qreal border)
{
    //QRectF rec = QRectF(rect.topLeft(), QSizeF(rect.bottomRight().x()-rect.topLeft().x(), rect.bottomRight().y()-rect.topLeft().y()-2*border));

    painter.setPen(Qt::black);
    QStaticText text;
    text.setTextWidth(rect.bottomRight().y()-rect.topLeft().y()-2*border-6/*rec.width()-2*border-6*/);
    //qInfo() << "-------------------------------------------------";
    //qInfo() << paraText().c_str();

    // draw prefix
    QSizeF s0{0,0};
    if (!m_Prefix.isEmpty())
    {
        auto rec0 = QRectF(QPointF(TopLeft.x(), TopLeft.y()), QSizeF(BottomRight.x()-TopLeft.x(), BottomRight.y()-TopLeft.y()-2*border));
        painter.setPen(Qt::black);
        text.setText(colorizeText(m_Prefix.toStdString().c_str(), Qt::blue).toStdString().c_str());
        painter.setClipRect(rec0);
        painter.drawStaticText(rec0.topLeft().x()+border+3, rec0.topLeft().y()+border, text);
        s0=text.size();
    }

    // draw paraText
    auto c=TopLeft.y()+s0.height();
    auto rec1 = QRectF(QPointF(TopLeft.x(), c), QSizeF(BottomRight.x()-TopLeft.x(), BottomRight.y()-c-2*border));
    text.setText(paraText().c_str());
    painter.setClipRect(rec1);
    painter.drawStaticText(rec1.topLeft().x()+border+3, rec1.topLeft().y()+border, text);
    QSizeF s1=text.size();

    // draw UserName
    QSizeF s2{0,0};
    auto userName=paraUser();
    if (!userName.empty())
    {
        auto c=TopLeft.y()+s1.height();
        auto rec2 = QRectF(QPointF(TopLeft.x(), c), QSizeF(BottomRight.x()-TopLeft.x(), BottomRight.y()-c-2*border));
        painter.setPen(Qt::black);
        text.setText(colorizeText(userName.c_str(), Qt::red).toStdString().c_str());
        painter.setClipRect(rec2);
        painter.drawStaticText(rec2.topLeft().x()+border+3, rec2.topLeft().y()+border, text);
        s2=text.size();
    }

    return s0.height()+s1.height()+s2.height();
}

void TArea::Paint(QPainter& painter, int level)
{
    if (rect().height()<1)
    {
        return;
    }

    int b(border(level));
    int l(line(level));
    int w(wid(level));

    if (Highlighted())
    {
        painter.setPen(QPen(Qt::darkMagenta, l, Qt::SolidLine, Qt::RoundCap));
        painter.setBrush(QBrush(QColor(150,150,150), Qt::SolidPattern));
    }
    else
    {
        painter.setPen(QPen(Qt::darkGray, l, Qt::SolidLine, Qt::RoundCap));
        painter.setBrush(QBrush(GetColor(), Qt::SolidPattern));
    }

    painter.setClipRect(rect());
    painter.drawRect(TopLeft.x()+b,TopLeft.y()+b, BottomRight.x()-TopLeft.x()-2*b-w, BottomRight.y()-TopLeft.y()-2*b-w);

#if ShowCurrentTask
    if (level==0)
        DrawText(QRectF(TopLeft, BottomRight), painter,b);
#endif

    if (level>0)
       DrawText(QRectF(TopLeft, BottomRight), painter,b+2);

    level++;
    for (auto& it : Areas)
    {
        it->Paint(painter, level);
    }
}

void TArea::Highlight(bool value)
{
    m_Highlight=value;
}

bool TArea::Highlighted()
{
    return m_Highlight;
}

void TArea::resetRect()
{
    TopLeft=QPointF(0,0);
    TopRight=QPointF(0,0);
    BottomLeft=QPointF(0,0);
    BottomRight=QPointF(0,0);
    for (auto& it : Areas)
    {
       it->resetRect();
    }
}

void TArea::Calculate(const QString& prefix, size_t index, QRectF rect, int level)
{
    m_Prefix=prefix;

    TopLeft=rect.topLeft();
    TopRight=rect.topRight();
    BottomLeft=rect.bottomLeft();
    BottomRight=rect.bottomRight();

#if ShowCurrentTask
    if (ParentA() && level==0)
    {
        QPainter painter;
        qreal h = DrawText(rect, painter,border(level));
        if (rect.height()<h)
        {
            for (auto& it : Areas)
            {
               it->resetRect();
            }
            return;
        }
        else
        {
            qDebug()<< rect.topLeft().y();
            rect.setTop(rect.topLeft().y()+h);
            qDebug() << rect.topLeft().y();
        }
    }
#endif

    if (level>0)
    {
        QPainter painter;
        qreal h = DrawText(rect, painter,border(level));
        if (rect.height()<h)
        {
            for (auto& it : Areas)
            {
               it->resetRect();
            }
            return;
        }
        else
        {
            rect.setTop(rect.topLeft().y()+h);
        }
    }

    auto rows=paraRows();
    if (rows>0)
    {
        size_t n = Areas.size();
        size_t cols=n/rows+(n%rows==0?0:1);
        int colw=rect.width()/cols;
        int rowh=rect.height()/rows;
        int icol=0;
        int irow=0;
        for (auto& it : Areas)
        {
            it->TopLeft = { rect.topLeft().x()+icol*colw, rect.topLeft().y()+irow*rowh };
            it->BottomRight = { it->TopLeft.x()+colw, it->TopLeft.y()+rowh };
            icol++;
            if (icol==cols)
            {
                irow++;
                icol=0;
            }
        }
    }
    else
    {
        // for subordinate areas we give space reduced by the space, required for the text of this area.


        // for the moment we need this intermediate buffer as a copy of original.
        auto Values = Areas;

        tref ref;

        if (Values.size()<1)
            return;

        for (auto& it : Values)
        {
            ref.emplace_back(TData(it));
        }

        auto b = [](TData a, TData b) {return a.area->paraValue() < b.area->paraValue(); };

        ref.sort(b);

        double sum(0.0);
        for (auto& it : ref)
        {
            sum += it.area->paraValue();
        }

        /*pTArea gArea=boost::make_shared<TArea>(TArea(sum, { 0, 0 },
                            { rect.width() - 1, 0 },
                            { 0, rect.height() - 1 },
                            { rect.width() - 1, rect.height() - 1 }));*/

        pTArea gArea=boost::make_shared<TArea>(TArea(sum, rect.topLeft(), rect.topRight(), rect.bottomLeft(), rect.bottomRight()));

        tref2 ref2;
        ref2.emplace_back(TData2(ref, gArea));
        size_t prevSize = ref2.size();

        auto it = ref2.begin();
        while (it!=ref2.end())
        {
            double X((*it).area->TopRight.x() - (*it).area->TopLeft.x());
            double Y((*it).area->BottomRight.y() - (*it).area->TopRight.y());

            (*it).vertical = X > Y;

            srt2(*it);

            auto ait = (*it).ref.begin();
            std::advance(ait, (*it).ref.size() - 1);

            if ((*ait).ratioToArea > 0.5)
            {
                /*if ((*it).vertical)
                {
                    (*it).cutLeft(ait);
                }
                else
                {
                    (*it).cutTop(ait);
                }*/

                if ((*it).ref.size() > 1)
                {
                    // keep to iterate one more time
                }
                else
                {
                    auto it2 = it;
                    it++;
                    ref2.erase(it2);
                    continue;
                }
            }
            //else
            {
                if ((*it).vertical)
                {
                    X /= 2.0;
                    for (auto ait= (*it).ref.begin(); ait!= (*it).ref.end(); ait++)
                    {
                        if ((*ait).area->TopLeft.x() <= (*it).area->TopRight.x() - X) // we split here
                        {
                            if (ait == (*it).ref.begin())
                            {
                                // ??
                            }

                            tref ref = { (*it).ref.begin(), ait }; // right side

                            (*it).cutLeft(ait);

                            if (ref.size() > 0)
                            {
                                ref2.emplace(it, TData2(ref, (*it).vertical));
                                it--;
                            }
                            else
                            {
                                it++;
                            }

                            break;
                        }
                    }
                }
                else
                {
                    Y /= 2.0;
                    for (auto ait = (*it).ref.begin(); ait != (*it).ref.end(); ait++)
                    {
                        if ((*ait).area->TopRight.y() <= (*it).area->BottomRight.y() - Y) // we split here
                        {
                            if (ait == (*it).ref.begin())
                            {
                                // ??
                            }

                            tref ref = { (*it).ref.begin(), ait }; // right side

                            (*it).cutTop(ait);
                            if (ref.size() > 0)
                            {
                                ref2.emplace(it, TData2(ref, (*it).vertical));
                                it--;
                            }
                            else
                            {
                                it++;
                            }

                            break;
                        }
                    }
                }
            }
        }
    }

    QString prefixNext("");
    int indexNext=0;
    level++;
    for (auto& it : Areas)
    {
        indexNext++;

        if (rows>0)
            prefixNext=QString::number(indexNext);

        it->Calculate(prefixNext, indexNext, it->rect(), level);
    }
}

}
