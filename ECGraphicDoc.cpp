#include "ECGraphicDoc.h"
#include "ECGraphicViewImp.h"
#include "ECShape.h"
#include <iostream>
#include <string>
#include <fstream>

ECGraphicDoc::ECGraphicDoc(ECGraphicViewImp &gvi, std::fstream &infile, std::string filename, bool saving, bool loading): saving(saving), loading(loading), infile(infile), filename(filename), gvi(gvi){
  if(loading) Load();
}

ECGraphicDoc::~ECGraphicDoc(){
  if(saving) Save();
  for(auto s: listShapes)
    delete s;
}

ECShape* ECGraphicDoc::LoadRectangle(bool addToVec){
  int sides, color, boundingBox[8];
  infile>>sides;
  for(int i = 0; i < 8; i++)
    infile>>boundingBox[i];
  infile>>color;
  return AddRectangle(boundingBox[0], boundingBox[1], boundingBox[4], boundingBox[5], 3, color, addToVec);
}

ECShape* ECGraphicDoc::LoadEllipse(bool addToVec){
  int points[2], radii[2];
  int color, x1, y1, x2, y2;
  for(int i = 0; i < 2; i++)
    infile>>points[i];
  for(int i = 0; i < 2; i++)
    infile>>radii[i];
  infile>>color;
  x1 = points[0] - radii[0]; y1 = points[1] - radii[1];
  x2 = points[0] + radii[0]; y2 = points[1] + radii[1];
  return AddEllipse(x1, y1, x2, y2, 3, color, addToVec);
}

ECShape* ECGraphicDoc::LoadFilledRectangle(bool addToVec){
  int sides, color, boundingBox[8];
  infile >> sides;
  for (int i = 0; i < 8; i++)
    infile >> boundingBox[i];
  infile >> color;
  return AddFilledRectanlge(boundingBox[0], boundingBox[1], boundingBox[4], boundingBox[5], color, addToVec);
}

ECShape* ECGraphicDoc::LoadFilledEllipse(bool addToVec){
  int points[2], radii[2];
  int color, x1, y1, x2, y2;
  for (int i = 0; i < 2; i++)
    infile >> points[i];
  for (int i = 0; i < 2; i++)
    infile >> radii[i];
  infile >> color;
  x1 = points[0] - radii[0];
  y1 = points[1] - radii[1];
  x2 = points[0] + radii[0];
  y2 = points[1] + radii[1];
  return AddFilledEllipse(x1, y1, x2, y2, color, addToVec);
}

ECShape* ECGraphicDoc::LoadComposite(bool addToVec){
  int numShapes, typeShape;
  infile>>numShapes;
  std::vector<ECShape *> childShapes;
  for(int i = 0; i < numShapes; i++){
    infile>>typeShape;
    switch(typeShape){
      case(0): childShapes.push_back(LoadRectangle(false)); break;
      case(1): childShapes.push_back(LoadEllipse(false)); break;
      case(2): childShapes.push_back(LoadFilledRectangle(false)); break;
      case(3): childShapes.push_back(LoadFilledEllipse(false)); break;
      case(4): childShapes.push_back(LoadComposite(false)); break;
    }
  }
  return AddComposite(childShapes, addToVec);
}

void ECGraphicDoc::Load(){
  int numShapes, typeShape;
  infile>>numShapes;
  for(int i = 0; i < numShapes; i++){
    infile>>typeShape;
    switch(typeShape){
      case(0): LoadRectangle(); break;
      case(1): LoadEllipse(); break;
      case(2): LoadFilledRectangle(); break;
      case(3): LoadFilledEllipse(); break;
      case(4): LoadComposite(); break;
    }
  }
  infile.close();
}

void ECGraphicDoc::Save() const {
  std::ofstream outfile(filename, std::ofstream::out | std::ofstream::trunc);
  outfile<<GetNumShapes()<<"\n";
  for(auto s: listShapes)
    s->SaveShape(outfile);
  outfile.close();
}


ECShape* ECGraphicDoc::AddRectangle(int x1, int y1, int x2, int y2, int thickness, int color, bool pushToVec)
{
  std::pair<int, int> p1 = std::make_pair(x1, y1), p2 = std::make_pair(x2, y2);
	ECShape* shape = new ECRectangle(p1, p2, color, thickness);
	if(pushToVec) listShapes.push_back(shape);
  shape->DrawShape(gvi);
	return shape;
}

ECShape *ECGraphicDoc::AddEllipse(int x1, int y1, int x2, int y2, int thickness, int color, bool pushToVec)
{
  std::pair<int, int> p1 = std::make_pair(x1, y1), p2 = std::make_pair(x2, y2);
  ECShape* shape = new ECEllipse(p1, p2, color, thickness);
  if(pushToVec) listShapes.push_back(shape);
  shape->DrawShape(gvi);
  return shape;
}

ECShape *ECGraphicDoc::AddFilledRectanlge(int x1, int y1, int x2, int y2, int color, bool pushToVec)
{
  std::pair<int, int> p1 = std::make_pair(x1, y1), p2 = std::make_pair(x2, y2);
  ECShape* shape = new ECFilledRectangle(p1, p2, color);
  if(pushToVec) listShapes.push_back(shape);
  shape->DrawShape(gvi);
  return shape;
}

ECShape *ECGraphicDoc::AddFilledEllipse(int x1, int y1, int x2, int y2, int color, bool pushToVec)
{
  std::pair<int, int> p1 = std::make_pair(x1, y1), p2 = std::make_pair(x2, y2);
  ECShape* shape = new ECFilledEllipse(p1, p2, color);
  if(pushToVec) listShapes.push_back(shape);
  shape->DrawShape(gvi);
  return shape;
}

ECShape *ECGraphicDoc::AddComposite(std::vector<ECShape *> moveTempShapes, bool pushToVec, int x, int y)
{
  ECShape* shape = new ECCompositeShape(moveTempShapes, x, y);
  if(pushToVec) listShapes.push_back(shape);
  shape->DrawShape(gvi);
  return shape;
}

void ECGraphicDoc::DeleteShape(ECShape *shape){
  bool found = false;
  for (int i = 0; i < GetNumShapes(); i++)
    if (listShapes[i] == shape){
      found = true;
      listShapes.erase(listShapes.begin() + i);
      break;
    }
  gvi.SetRedraw(true);
}

void ECGraphicDoc::EditTemp(int x1, int y1, int x2, int y2, int col, int thickness){
  ECShape* temp = listShapes[GetNumShapes() - 1];  
  temp->ChangeColor(col);
  temp->ChangeThickness(thickness);
  temp->ChangeBoundingBox(std::make_pair(x1, y1), std::make_pair(x2, y2));
}

void ECGraphicDoc::RemoveTemp(){
  ECShape* temp = listShapes[GetNumShapes() - 1];
  listShapes.pop_back();
  delete temp;
}

void ECGraphicDoc::MoveShape(ECShape* shape, std::pair<int, int> np1, std::pair<int, int> np2){
  shape->ChangeBoundingBox(np1, np2);
  gvi.SetRedraw(true);
}
