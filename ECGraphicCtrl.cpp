//
//  ECGraphicCtrl.cpp
//
//  Created by Ishan Tyagi on 11/30/22.
//
//

#include "ECGraphicCtrl.h"
#include "ECGraphicViewImp.h"
#include "ECGraphicDoc.h"
#include "ECShape.h"
#include <iostream>

ECGraphicCtrl::ECGraphicCtrl(ECGraphicViewImp &gIn, ECGraphicDoc &docIn): gIn(gIn), docIn(docIn), cmdhist(new ECCommandHistory()){
    ins_mode = false; temp = false; curSel = false; rectangle = true; filled = false;
    ECModeObserver *modeObs = new ECModeObserver(&gIn, this); obs.push_back(modeObs);
    ECCreationEvtObserver *createEventObs = new ECCreationEvtObserver(&gIn, this); obs.push_back(createEventObs);
    ECTimerObserver *timerObs = new ECTimerObserver(&gIn, this); obs.push_back(timerObs);
    ECMultiSelectObserver *multiSelObs = new ECMultiSelectObserver(&gIn, this); obs.push_back(multiSelObs);
    ECSelectObserver *selectObs = new ECSelectObserver(&gIn, this); obs.push_back(selectObs);
    ECDeleteObserver *deleteObs = new ECDeleteObserver(&gIn, this); obs.push_back(deleteObs);
    ECCmdHistoryObserver *cmdHistObs = new ECCmdHistoryObserver(&gIn, this); obs.push_back(cmdHistObs);
    ECMoveObserver *moveObs = new ECMoveObserver(&gIn, this); obs.push_back(moveObs);
    ECShapeToDrawObserver *shapeToDrawObs = new ECShapeToDrawObserver(&gIn, this); obs.push_back(shapeToDrawObs);
    ECFilledModeObserver *filledModeObs = new ECFilledModeObserver(&gIn, this); obs.push_back(filledModeObs);
    ECButtonMoveObserver *buttonMoveObs = new ECButtonMoveObserver(&gIn, this); obs.push_back(buttonMoveObs);
    ECGroupingObserver *groupingObs = new ECGroupingObserver(&gIn, this); obs.push_back(groupingObs);
    for(auto obsvr: obs)
      gIn.Attach(obsvr);
}

ECGraphicCtrl::~ECGraphicCtrl(){
  delete cmdhist;
  for(auto o: obs)
    delete o;
}

void ECGraphicCtrl::CreateShape(int x1, int y1, int x2, int y2){
  if(rectangle && filled)
    CreateFilledRectangle(x1, y1, x2, y2);
  else if(rectangle && !filled)
    CreateRectangle(x1, y1, x2, y2);
  else if(!rectangle && filled)
    CreateFilledEllipse(x1, y1, x2, y2);
  else 
    CreateEllipse(x1, y1, x2, y2);
}

void ECGraphicCtrl::DeleteShape(){
  DeleteShapeCMD *pCmd = new DeleteShapeCMD(&docIn, sel[0]);
  DeSelect();
  cmdhist->ExecuteCmd(pCmd);
  std::cout << "Deleted Shape\n";
}

void ECGraphicCtrl::CreateRectangle(int x1, int y1, int x2, int y2){
  CreateRectangleCMD *pCmd = new CreateRectangleCMD(&docIn, x1, y1, x2, y2);
  cmdhist->ExecuteCmd(pCmd);
  std::cout<<"Creating Rectangle\n";
}

void ECGraphicCtrl::CreateFilledRectangle(int x1, int y1, int x2, int y2){
  CreateFilledRectangleCMD *pCmd = new CreateFilledRectangleCMD(&docIn, x1, y1, x2, y2);
  cmdhist->ExecuteCmd(pCmd);
  std::cout<<"Creating Filled Rectangle\n";
}

void ECGraphicCtrl::CreateEllipse(int x1, int y1, int x2, int y2){
  CreateEllipseCMD *pCmd = new CreateEllipseCMD(&docIn, x1, y1, x2, y2);
  cmdhist->ExecuteCmd(pCmd);
  std::cout<<"Creating Ellipse\n";
}

void ECGraphicCtrl::CreateFilledEllipse(int x1, int y1, int x2, int y2){
  CreateFilledEllipseCMD *pCmd = new CreateFilledEllipseCMD(&docIn, x1, y1, x2, y2);
  cmdhist->ExecuteCmd(pCmd);
  std::cout << "Creating Filled Ellipse\n";
}

void ECGraphicCtrl::MoveTemp(int xOffset, int yOffset){
  std::pair<int, int> p1, p2;
  sel[0]->GetBoundingBox(p1, p2);
  if (!xOffset && !yOffset && !temp){
    sel[0]->MakeMoveTemp(docIn);
    temp = true;
  }
  else
    docIn.EditTemp(p1.first + xOffset, p1.second + yOffset, p2.first + xOffset, p2.second + yOffset, 4);
}

void ECGraphicCtrl::CreationTemp(int x1, int y1, int x2, int y2){
  if(temp)
    docIn.EditTemp(x1, y1, x2, y2, 6, 1);
  else{
    WhichShapeTempCreation(x1, y1, x2, y2, 1, 6); // temps are now purple
    temp = true; 
  }
}

void ECGraphicCtrl::WhichShapeTempCreation(int x1, int y1, int x2, int y2, int thickness, int color){
  if (rectangle && !filled)
    docIn.AddRectangle(x1, y1, x2, y2, thickness, color);
  else if (rectangle && filled)
    docIn.AddFilledRectanlge(x1, y2, x2, y2, color); 
  else if (!rectangle && !filled)
    docIn.AddEllipse(x1, y1, x2, y2, thickness, color);
  else
    docIn.AddFilledEllipse(x1, y1, x2, y2, color); 
}

void ECGraphicCtrl::RemoveTemp(){
  docIn.RemoveTemp();
  temp = false;
}

void ECGraphicCtrl::RedrawShapes(){
  for(int i = 0; i < docIn.GetNumShapes(); i++)
    if(docIn.GetShape(i)->Draw())
      docIn.GetShape(i)->DrawShape(gIn);
}

void ECGraphicCtrl::ChangeMode(){
  ins_mode = !(ins_mode);
  std::cout<<((ins_mode) ? "Insert Mode\n" : "Edit Mode\n");
}

void ECGraphicCtrl::IsCurSelected(int x, int y){
  int tol = 15; //pixel tolerance for selection
  for(int i = 0; i < docIn.GetNumShapes(); i++){
    ECShape* ptr = docIn.GetShape(i);
    if(ptr->IsOnShape(x, y) && !IsInSel(ptr)){
      sel.push_back(ptr);
      ptr->ChangeColor(4); //changes color to blue so its selected
      curSel = true;
      std::cout << "select success\n";
      return ;
    }
  }
}

bool ECGraphicCtrl::IsCurHeld(int x1, int y1){
  for (auto s : sel)
    if (s->IsOnShape(x1, y1))
      return true;
  return false;
}

void ECGraphicCtrl::DeSelect(){
  for(auto s: sel)
    s->ChangeColor(0);
  sel.clear();
  curSel = false;
  std::cout<<"deselected\n";
}


//makes the shape we are moving non visible
void ECGraphicCtrl::preMove(){
  for(auto s: sel)
    s->ChangeDraw(false);
}

//After moving, makes the selected shapes visible
void ECGraphicCtrl::NoMove(){
  for(auto s: sel)
    s->ChangeDraw(true);
}

void ECGraphicCtrl::Move(int xO, int yO){ 
  MoveShapeCMD* pCmd = new MoveShapeCMD(&docIn, sel, xO, yO); 
  for(auto s: sel){
    s->ChangeColor(4);
    s->ChangeDraw(true);
  }
  cmdhist->ExecuteCmd(pCmd);
}

void ECGraphicCtrl::ChangeFilled(){
  filled = !(filled);
  std::cout << ((filled) ? "Filled\n" : "UnFilled\n");
}

void ECGraphicCtrl::ChangeShapeDraw(){
  rectangle = !(rectangle);
  std::cout<< ((rectangle) ? "Rectangle\n" : "Ellipse\n");
}

bool ECGraphicCtrl::IsInSel(ECShape *ptr){
  for(auto s: sel)
    if(s == ptr)
      return true;
  return false;
}

void ECGraphicCtrl::GroupSelectedShapes(){
  GroupShapesCMD* pCmd = new GroupShapesCMD(&docIn, sel);
  cmdhist->ExecuteCmd(pCmd);
  sel.clear();
  sel.push_back(pCmd->GetComp());
  std::cout<<"grouping shapes\n";
}

void ECGraphicCtrl::UnGroupSelectedShapes(){
  int size;
  ECShape* ptr = sel[0];
  if(dynamic_cast<ECCompositeShape *>(ptr) != nullptr){
    sel.clear();
    size = ptr->GetListSize();
    for(int i = 0; i < size; i++)
      sel.push_back(ptr->GetChild(i));
    UnGroupShapeCMD* pCmd = new UnGroupShapeCMD(&docIn, ptr);
    cmdhist->ExecuteCmd(pCmd);
    std::cout<<"ungrouping composite\n";
  }
}