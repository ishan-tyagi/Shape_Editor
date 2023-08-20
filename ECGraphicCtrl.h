//
//  ECGraphicCtrl.h
//
//  Created by Ishan Tyagi on 11/29/22.
//
//

#ifndef ECGRAPHICCTRL_H
#define ECGRAPHICCTRL_H

#include <vector>
#include "ECCommand.h"

class ECGraphicViewImp;
class ECGraphicDoc;
class ECCommandHistory;
class ECShape;
class ECObserver;

class ECGraphicCtrl{
 public:
  ECGraphicCtrl(ECGraphicViewImp &gIn, ECGraphicDoc &docIn);

  ~ECGraphicCtrl();

  void CreateShape(int x1, int y1, int x2, int y2); 
  void DeleteShape();                               

  void CreateRectangle(int x1, int y1, int x2, int y2);
  void CreateFilledRectangle(int x1, int y1, int x2, int y2);
  void CreateEllipse(int x1, int y1, int x2, int y2);
  void CreateFilledEllipse(int x1, int y1, int x2, int y2);

  void MoveTemp(int xOffset, int yOffset); //creates temporary shape while moving 
  void CreationTemp(int x1, int y1, int x2, int y2); //Creates Temporary shape
  void WhichShapeTempCreation(int x1, int y1, int x2, int y2, int thickness, int color); //helps CreationTemp pick shape
  void RemoveTemp(); 

  void RedrawShapes(); 

  void ChangeMode(); //edit or insertion mode
  bool GetMode() const { return ins_mode; }

  void IsCurSelected(int x1, int y1);
  bool IsCurHeld(int x1, int y1); // if mouse is on the point
  bool IsSelected() { return curSel; } //is a shape is currently selected
  void DeSelect(); 

  bool Undo() { return cmdhist->Undo(); }
  bool Redo() { return cmdhist->Redo(); }

  void preMove(); //changes view of shape
  void NoMove();
  void Move(int xO, int yO);

  void ChangeFilled();  //changes filled or not
  bool IsFilled() const { return filled; }

  void ChangeShapeDraw(); //changes shape to draw (true: rectangle, false: ellipse)
  bool GetShapeDraw() { return rectangle; }

  int NumSelected() { return sel.size(); }
  bool IsInSel(ECShape *ptr);

  void GroupSelectedShapes();
  void UnGroupSelectedShapes();

 private:
  ECGraphicViewImp &gIn;
  ECGraphicDoc &docIn;
  ECCommandHistory *cmdhist;
  std::vector<ECObserver *> obs; 
  std::vector<ECShape *> sel; //vector of shapes that are currently selected
  bool ins_mode; //is it in insert mode?
  bool temp; //is there a temporary shape active?
  bool curSel; //is there a shape currently selected?
  bool rectangle; //making rectangle? (if false, making ellipse)
  bool filled;  //making filled shape? (if false, making unfilled)
};


#endif
