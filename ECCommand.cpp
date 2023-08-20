#include "ECCommand.h"
#include <iostream>

//****************************
//CreateShapeCMD

CreateShapeCMD::~CreateShapeCMD(){
  if(killShape && shape != nullptr)
    delete shape;
}

void CreateShapeCMD::Execute(){
  SetKillShape(false);
  getDoc()->AddShape(GetShape());
}

void CreateShapeCMD::UnExecute(){
  SetKillShape(true);
  getDoc()->DeleteShape(GetShape());
}

void CreateRectangleCMD::Execute(){
  if(GetShape() != nullptr)
    CreateShapeCMD::Execute();
  else{
    std::pair<int, int> p1, p2;
    GetPoints(p1, p2);
    SetKillShape(false);
    ChangeShape(getDoc()->AddRectangle(p1.first, p1.second, p2.first, p2.second));
  }
}

void CreateEllipseCMD::Execute(){
  if(GetShape() != nullptr)
    CreateShapeCMD::Execute();
  else{
    std::pair<int, int> p1, p2;
    GetPoints(p1, p2);
    SetKillShape(false);
    ChangeShape(getDoc()->AddEllipse(p1.first, p1.second, p2.first, p2.second));
  }
}

void CreateFilledRectangleCMD::Execute(){
  if(GetShape() != nullptr)
    CreateShapeCMD::Execute();
  else{
    std::pair<int, int> p1, p2;
    GetPoints(p1, p2);
    SetKillShape(false);
    ChangeShape(getDoc()->AddFilledRectanlge(p1.first, p1.second, p2.first, p2.second));
  }
}

void CreateFilledEllipseCMD::Execute(){
  if (GetShape() != nullptr)
    CreateShapeCMD::Execute();
  else{
    std::pair<int, int> p1, p2;
    GetPoints(p1, p2);
    SetKillShape(false);
    ChangeShape(getDoc()->AddFilledEllipse(p1.first, p1.second, p2.first, p2.second));
  }
}

void DeleteShapeCMD::Execute(){
  CreateShapeCMD::UnExecute();
}

void DeleteShapeCMD::UnExecute(){
  CreateShapeCMD::Execute();
}

//***************************************
//MoveShapeCMD

MoveShapeCMD::MoveShapeCMD(ECGraphicDoc *docIn, std::vector<ECShape *> shape, int xOffset, int yOffset) : ECCommand(docIn), shape(shape), xOffset(xOffset), yOffset(yOffset){
  std::pair<int, int> p1, p2;
  for (auto s : shape){
    s->GetBoundingBox(p1, p2);
    points.push(p1);
    points.push(p2);
  }
}

void MoveShapeCMD::Execute(){
  std::pair<int, int> p1, p2;
  for(auto s: shape){
    p1 = points.front(); points.pop();
    p2 = points.front(); points.pop();
    p1.first += xOffset; p1.second += yOffset;
    p2.first += xOffset; p2.second += yOffset;
    getDoc()->MoveShape(s, p1, p2);
    points.push(p1);
    points.push(p2);
  }
}

void MoveShapeCMD::UnExecute(){
  std::pair<int, int> p1, p2;
  for(auto s: shape){
    p1 = points.front(); points.pop();
    p2 = points.front(); points.pop();
    p1.first -= xOffset; p1.second -= yOffset;
    p2.first -= xOffset; p2.second -= yOffset;
    getDoc()->MoveShape(s, p1, p2);
    points.push(p1);
    points.push(p2);
  }
}

//***************************************
//GroupShapesCMD

GroupShapesCMD::~GroupShapesCMD(){
  if(killShape && composite != nullptr)
    delete composite;
}

void GroupShapesCMD::Execute(){
  for(auto s: shapes)
    getDoc()->DeleteShape(s);
  if(composite == nullptr)
    composite = getDoc()->AddComposite(shapes);
  else{
    for(auto s: shapes)
      composite->AddChild(s);
    getDoc()->AddShape(composite);
    killShape = false;
  }
}

void GroupShapesCMD::UnExecute(){
  getDoc()->DeleteShape(composite);
  composite->ClearShapes();
  for(auto s: shapes)
    getDoc()->AddShape(s);
  killShape = true;
}

//***************************************
//UnGroupShapeCMD

UnGroupShapeCMD::UnGroupShapeCMD(ECGraphicDoc *docIn, ECShape *composite): GroupShapesCMD(docIn, std::vector<ECShape *> {}){
  std::vector<ECShape *> shapes;
  for(int i = 0; i < composite->GetListSize(); i++)
    shapes.push_back(composite->GetChild(i));
  SetShapeList(shapes);
  SetComp(composite);
}

void UnGroupShapeCMD::Execute(){
  GroupShapesCMD::UnExecute();
}

void UnGroupShapeCMD::UnExecute(){
  GroupShapesCMD::Execute();
}

//*****************************
//	Command History 

ECCommandHistory :: ECCommandHistory() {}

ECCommandHistory :: ~ECCommandHistory()
{
  while(!history.empty()){
    delete history.top();
    history.pop();
  }
  while(!rhistory.empty()){
    delete rhistory.top();
    rhistory.pop();
  } 
}

bool ECCommandHistory::Undo(){
  if(history.empty()) return false;
  ECCommand* pCmd = history.top();
  history.pop();
  pCmd->UnExecute();
  rhistory.push(pCmd);
  return true;
}


bool ECCommandHistory::Redo(){
  if(rhistory.empty()) return false;
  ECCommand* pCmd = rhistory.top();
  rhistory.pop();
  pCmd->Execute();
  history.push(pCmd);
  return true;
}

void ECCommandHistory::ExecuteCmd(ECCommand *pCmd){
  while(!rhistory.empty()){
    delete rhistory.top();
    rhistory.pop();
  }
  pCmd->Execute();
  history.push(pCmd);
}
