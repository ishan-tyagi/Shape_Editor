#include "ECShape.h"
#include "ECGraphicViewImp.h"
#include <cmath>
#include <iostream>

void ECShape::GetBoundingBox(std::pair<int, int> &np1, std::pair<int, int> &np2) const{
  np1.first = _p1.first; np1.second = _p1.second;
  np2.first = _p2.first; np2.second = _p2.second;
}

//***************************************

bool ECRectangle::IsOnShape(int x, int y) const
{
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    return (x >= p1.first && x <= p2.first && y >= p1.second && y <= p2.second);
}

void ECRectangle::DrawShape(ECGraphicViewImp &gIn) const{
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    gIn.DrawRectangle(p1.first, p1.second, p2.first, p2.second, GetThickness(), static_cast<ECGVColor>(GetColor()));
    gIn.SetRedraw(true);
}

ECShape *ECRectangle::MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec){
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    return docIn.AddRectangle(p1.first, p1.second, p2.first, p2.second, 3, 4, pushToVec); //is blue
}

void ECRectangle::SaveShape(std::ofstream &outfile) const{
  std::pair<int, int> p1, p2;
  GetBoundingBox(p1, p2);
  outfile<<"0 4 ";
  outfile<<p1.first<<" "<<p1.second<<" ";
  outfile<<p1.first<<" "<<p2.second<<" ";
  outfile<<p2.first<<" "<<p2.second<<" ";
  outfile<<p2.first<<" "<<p1.second<<" ";
  outfile<<"0\n";
}

//***************************************

bool ECEllipse::IsOnShape(int x, int y) const{
    double xRad, yRad;
    std::pair<int, int> c;
    GetCenter(c);
    GetRadii(xRad, yRad);
    double right = pow(x - c.first, 2) / (xRad * xRad);
    double left = pow(y - c.second, 2) / (yRad * yRad);
    return right + left <= 1;
}

void ECEllipse::ChangeBoundingBox(std::pair<int, int> np1, std::pair<int, int> np2){
    ECShape::ChangeBoundingBox(np1, np2);
    SetRadii(xRadius, yRadius);
    SetCenter(center);
}

void ECEllipse::SetCenter(std::pair<int, int> &C)
{
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    C.first = (p1.first + p2.first) / 2;
    C.second = (p1.second + p2.second) / 2;
}

void ECEllipse::SetRadii(double &xRadius, double &yRadius)
{
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    xRadius = fabs((p2.first - p1.first) / 2);
    yRadius = fabs((p2.second - p1.second) / 2);
}

void ECEllipse::DrawShape(ECGraphicViewImp &gIn) const{
    gIn.DrawEllipse(center.first, center.second, xRadius, yRadius, GetThickness(), static_cast<ECGVColor>(GetColor()));
    gIn.SetRedraw(true);
}

ECShape *ECEllipse::MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec){
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    return docIn.AddEllipse(p1.first, p1.second, p2.first, p2.second, 3, 4, pushToVec);
}

void ECEllipse::SaveShape(std::ofstream &outfile) const{
    double xRad, yRad;
    std::pair<int, int> centr;
    GetCenter(centr);
    GetRadii(xRad, yRad);
    outfile << "1 " << centr.first << " " << centr.second << " ";
    outfile << int(xRad) << " " << int(yRad) << " 0\n";
}

//***************************************

void ECFilledRectangle::DrawShape(ECGraphicViewImp &gIn) const{
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    gIn.DrawFilledRectangle(p1.first, p1.second, p2.first, p2.second, static_cast<ECGVColor>(GetColor()));
    gIn.SetRedraw(true);
}

ECShape *ECFilledRectangle::MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec){
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    return docIn.AddFilledRectanlge(p1.first, p1.second, p2.first, p2.second, 4, pushToVec);
}

void ECFilledRectangle::SaveShape(std::ofstream &outfile) const{
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    outfile << "2 4 ";
    outfile << p1.first << " " << p1.second << " ";
    outfile << p1.first << " " << p2.second << " ";
    outfile << p2.first << " " << p2.second << " ";
    outfile << p2.first << " " << p1.second << " ";
    outfile << "0\n";
}

//***************************************

void ECFilledEllipse::DrawShape(ECGraphicViewImp &gIn) const
{
    std::pair<int, int> center;
    double xRadius, yRadius;
    GetCenter(center);
    GetRadii(xRadius, yRadius);
    gIn.DrawFilledEllipse(center.first, center.second, xRadius, yRadius, static_cast<ECGVColor>(GetColor()));
    gIn.SetRedraw(true);
}

ECShape *ECFilledEllipse::MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec){
    std::pair<int, int> p1, p2;
    GetBoundingBox(p1, p2);
    return docIn.AddFilledEllipse(p1.first, p1.second, p2.first, p2.second, 4, pushToVec);
}

void ECFilledEllipse::SaveShape(std::ofstream &outfile) const{
  double xRad, yRad;
  std::pair<int, int> centr;
  GetCenter(centr); GetRadii(xRad, yRad);
  outfile << "3 " << centr.first << " " << centr.second << " ";
  outfile << int(xRad) << " " << int(yRad) << " 0\n";
}

//***************************************

ECCompositeShape::~ECCompositeShape(){
    for(auto s: listShapes)
      delete s;
}

bool ECCompositeShape::IsOnShape(int x, int y) const{
  for (auto s : listShapes)
    if (s->IsOnShape(x, y))
      return true;
  return false;
}

void ECCompositeShape::RemoveChild(ECShape *child){
  for (int i = 0; i < listShapes.size(); i++)
    if (listShapes[i] == child){
      listShapes.erase(listShapes.begin() + i);
      break;
    }
}

void ECCompositeShape::DrawShape(ECGraphicViewImp &gIn) const{
  for(auto s: listShapes)
    s->DrawShape(gIn);
}

void ECCompositeShape::ChangeBoundingBox(std::pair<int, int> np1, std::pair<int, int> np2){
  std::pair<int, int> p1, p2, offset; GetBoundingBox(p1, p2);
  offset.first = np1.first - p1.first; offset.second = np1.second - p1.second; //true offset
  ECShape::ChangeBoundingBox(np1, np2);
  for(auto s: listShapes){
    s->GetBoundingBox(p1, p2);
    p1.first += offset.first; p1.second += offset.second;
    p2.first += offset.first; p2.second += offset.second;
    s->ChangeBoundingBox(p1, p2);
  }
}

ECShape *ECCompositeShape::MakeMoveTemp(ECGraphicDoc &docIn, bool pushToVec){
  std::vector<ECShape *> moveTempShapes;
  std::pair<int, int> p1, p2; GetBoundingBox(p1, p2);
  for(auto s: listShapes)
    moveTempShapes.push_back(s->MakeMoveTemp(docIn, false));
  return docIn.AddComposite(moveTempShapes, pushToVec, p1.first, p1.second);
}

void ECCompositeShape::ChangeColor(int newColor){
  for(auto s: listShapes)
    s->ChangeColor(newColor);
}

void ECCompositeShape::ChangeDraw(bool cd){
  ECShape::ChangeDraw(cd);
  for(auto s: listShapes)
    s->ChangeDraw(cd);
}

void ECCompositeShape::SaveShape(std::ofstream &outfile) const{
  outfile<<"4 "<<GetListSize()<<"\n";
  for(auto s: listShapes)
    s->SaveShape(outfile);
}