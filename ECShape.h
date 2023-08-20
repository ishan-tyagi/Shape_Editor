//
//
//  Created by Ishan Tyagi on 12/07/22.
//
//

#ifndef __ECSHAPE_H
#define __ECSHAPE_H

#include <vector>

class ECGraphicViewImp;
class ECGraphicDoc;

class ECShape
{
public:
    ECShape(std::pair<int, int> p1, std::pair<int, int> p2, int color = 0, int thickness = 3): _p1(p1), _p2(p2), color(color), thickness(thickness), draw(true){
      if (_p1.first > _p2.first && _p1.second > _p2.second)
        std::swap(_p1, _p2);
    }

    virtual ~ECShape() {}

    virtual bool IsOnShape(int x, int y) const { return false; }

    virtual int GetColor() const { return color; }
    virtual void ChangeColor(int newColor) { color = newColor; }

    virtual int GetThickness() const { return thickness; }
    virtual void ChangeThickness(int newThickness) { thickness = newThickness; }

    virtual void GetBoundingBox(std::pair<int, int> &np1, std::pair<int, int> &np2) const;
    virtual void ChangeBoundingBox(std::pair<int, int> np1, std::pair<int, int> np2){
        _p1 = np1; _p2 = np2;
    }

    virtual void AddChild(ECShape *child){};
    virtual ECShape* GetChild(int i) const { return nullptr; }
    virtual void RemoveChild(ECShape *child) {};
    virtual int GetListSize() const { return 0; }

    virtual void DrawShape(ECGraphicViewImp &gIn) const {}

    virtual ECShape* MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec=true) { return NULL; } 

    virtual void ChangeDraw(bool cd) { draw = cd; }
    bool Draw() { return draw; }

    virtual void ClearShapes() {}

    virtual void SaveShape(std::ofstream &outfile) const {}

private:
    std::pair<int, int> _p1;
    std::pair<int, int> _p2;
    int color;
    int thickness;
    bool draw;
};

class ECRectangle : public ECShape
{
public:
    ECRectangle(std::pair<int, int> p1, std::pair<int, int> p2, int color = 0, int thickness = 3) : ECShape(p1, p2, color, thickness) {}
    virtual ~ECRectangle() {}
    virtual bool IsOnShape(int x, int y) const;
    virtual void DrawShape(ECGraphicViewImp &gIn) const;
    virtual ECShape *MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec = true);
    virtual void SaveShape(std::ofstream &outfile) const;
};

class ECEllipse : public ECShape
{
public:
    ECEllipse(std::pair<int, int> p1, std::pair<int, int> p2, int color = 0, int thickness = 3) : ECShape(p1, p2, color, thickness){
        SetRadii(xRadius, yRadius);
        SetCenter(center);
    }

    virtual ~ECEllipse() {}
    virtual bool IsOnShape(int x, int y) const;

    virtual void ChangeBoundingBox(std::pair<int, int> np1, std::pair<int, int> np2); 

    void GetCenter(std::pair<int, int> &C) const { C = center; }
    void ChangeCenter(std::pair<int, int> newC) { center = newC; }

    void GetRadii(double &xRad, double &yRad) const{
        xRad = xRadius; yRad = yRadius;
    }
    virtual void DrawShape(ECGraphicViewImp &gIn) const;
    virtual ECShape *MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec = true);
    virtual void SaveShape(std::ofstream &outfile) const;

private:
    void SetRadii(double &xRadius, double &yRadius);
    void SetCenter(std::pair<int, int> &C);
    double xRadius;
    double yRadius;
    std::pair<int, int> center;
};

class ECFilledRectangle : public ECRectangle{
public:
    ECFilledRectangle(std::pair<int, int> p1, std::pair<int, int> p2, int color = 0) : ECRectangle(p1, p2, color) {}
    virtual void DrawShape(ECGraphicViewImp &gIn) const;
    virtual ECShape *MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec = true);
    virtual void SaveShape(std::ofstream &outfile) const;
};

class ECFilledEllipse : public ECEllipse{
public:
    ECFilledEllipse(std::pair<int, int> p1, std::pair<int, int> p2, int color = 0) : ECEllipse(p1, p2, color) {}
    virtual void DrawShape(ECGraphicViewImp &gIn) const;
    virtual ECShape *MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec = true);
    virtual void SaveShape(std::ofstream &outfile) const;
};

class ECCompositeShape : public ECShape{
 public:
    ECCompositeShape(const std::vector<ECShape *> &shapes, int x=0, int y=0) : ECShape({x, y}, {x, y}), listShapes(shapes) {} 

    ~ECCompositeShape();

    virtual bool IsOnShape(int x, int y) const;
    virtual void AddChild(ECShape *child) { listShapes.push_back(child); }
    virtual ECShape* GetChild(int i) const { return listShapes[i]; }
    virtual void RemoveChild(ECShape *child);
    virtual void DrawShape(ECGraphicViewImp &gIn) const;
    virtual void ChangeBoundingBox(std::pair<int, int> np1, std::pair<int, int> np2);
    virtual ECShape *MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec = true); // need to implement change bounding box too for this
    virtual int GetListSize() const { return listShapes.size(); }
    virtual void ChangeColor(int newColor);
    virtual void ChangeDraw(bool cd);
    virtual void ClearShapes() { listShapes.clear(); }
    virtual void SaveShape(std::ofstream &outfile) const;

private:
    std::vector<ECShape *> listShapes;
};

#endif