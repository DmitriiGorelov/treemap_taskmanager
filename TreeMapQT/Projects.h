#ifndef PROJECTS_H
#define PROJECTS_H

#include <QDebug>
#include "Areas.h"
#include "UID.h"
#include "DataProvider/XMLParameters/XMLParametrised.h"

namespace global_namespace
{

class TProject //: public cUID, public cXMLParametrised // Project is just a container. All its properties are preserved inside RootArea.
{
public:
    static boost::shared_ptr<TProject> Create(const std::string& _uid)
    {
        return boost::make_shared<TProject>(_uid);
    }    

    static boost::shared_ptr<TProject> Create()
    {
        return boost::make_shared<TProject>();
    }

    TProject()
        : pRootArea(boost::make_shared<TArea>(1))
        , pSelectedA(pRootArea)
        , pFocusedA(nullptr)
    {
        pRootArea->paraCaption("Project "+std::to_string(counter()));
    }

    TProject(TAreas areas)
        : pRootArea(boost::make_shared<TArea>(1))
        , pSelectedA(pRootArea)
        , pFocusedA(nullptr)
    {
        pRootArea->paraCaption("Project "+std::to_string(counter()));
        pRootArea->setAreas(areas);
    }

    TProject(const std::string& _uid)
      : pRootArea(boost::make_shared<TArea>(_uid.c_str()))
      , pSelectedA(pRootArea)
      , pFocusedA(nullptr)
    {
        //RootArea->Areas = areas;
    }

    ~TProject() {    }

    void AddTaskSelected(pTArea area)
    {
        //SelectedA()->Areas.emplace_back(area);
        TArea::PasteTo(area, SelectedA());
    }

    void AddTaskSelected(double part)
    {
        pTArea area=TArea::Create(MaxValue()*part,nullptr);
        //SelectedA()->Areas.push_back(area);
        TArea::PasteTo(area, SelectedA());
        pFocusedA = area;
    }

    void AddTaskFocused(double part)
    {
        if (!Focused())
            Focus(SelectedA());

        pTArea area=TArea::Create(Focused()->Max()*part,nullptr);

        //Focused()->Areas.push_back(area);
        TArea::PasteTo(area, Focused());
        pFocusedA = area;
    }

    void LevelUp()
    {
        Select(SelectedA()->ParentA());
    }

    pTArea& SelectedA()
    {
        if (!pSelectedA)
        {
            pSelectedA = pRootArea;
        }
        return pSelectedA;
    }

    double SelectedSum()
    {
        return SelectedA()->Sum();
    }

    double MaxValue()
    {
        return SelectedA()->Max();
    }

    /*pTArea CanFocus(int x, int y)
    {
        return SelectedA()->Select(x,y);
    }*/

    pTArea CanFocus(QPoint point)
    {
        return SelectedA()->Selectable(point);
    }

    pTArea Focus(QPoint point)
    {
        UnFocus();
        pTArea area = SelectedA()->Selectable(point);
        if (area)
        {
            qInfo() << __FUNCTION__ << area->paraCaption().c_str();

            pFocusedA=area;
            return area;
            //pFocusedA->Highlight(true);
        }
    }

    void Focus(pTArea area)
    {
        if (area)
            qInfo() << __FUNCTION__ << area->paraCaption().c_str();

        UnFocus();
        if (area)
        {
            pFocusedA=area;
            //pFocusedA->Highlight(true);
        }
    }

    void UnFocus()
    {
        if (pFocusedA)
            pFocusedA->Highlight(false);
        pFocusedA=nullptr;
    }

    pTArea Focused()
    {
        if (pFocusedA)
            qInfo() << pFocusedA->paraCaption().c_str();
        return pFocusedA;
    }

    /*pTArea CanSelect(int x, int y)
    {
        return SelectedA()->Select(x,y);
    }*/

    pTArea CanSelect(QPoint point)
    {
        return SelectedA()->Selectable(point);
    }

    /*void Select(int x, int y)
    {
        pTArea area =SelectedA()->Select(x,y);
        if (area)
            pSelectedA=area;
    }    */

    void Select(QPoint point)
    {
        pTArea area =SelectedA()->Selectable(point);
        if (area)
            pSelectedA=area;
    }

    void Select (pTArea area)
    {
        if (area)
        {
            pSelectedA=area;
        }
    }

    pTArea RootArea()
    {
        return pRootArea;
    }

    void DeleteFocused()
    {
        if (pSelectedA && pFocusedA)
        {
            auto p = pFocusedA->ParentA();
            if (p)
            {
                /*auto it = std::find(p->Areas.begin(), p->Areas.end(), pFocusedA);
                if (it!=p->Areas.end())
                    p->Areas.erase(it);*/
                p->Remove(pFocusedA);
            }
        }
    }

    QStringList getUsers();

private:
    static int counter() { static int i=1; return i++; }

    pTArea pRootArea;
    pTArea pSelectedA;
    pTArea pFocusedA;
};

typedef boost::shared_ptr<TProject> pTProject;
typedef std::list<pTProject> TProjects;
}
#endif // PROJECTS_H
