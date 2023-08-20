//
//
//  Created by Ishan Tyagi on 11/29/22.
//
//

#ifndef ECCOMMAND_H
#define ECCOMMAND_H

#include "ECShape.h"
#include "ECGraphicDoc.h"
#include <vector>
#include <queue>
#include <iostream>
#include <stack>

class ECGraphicDoc;

class ECCommand{
 public:
  ECCommand(ECGraphicDoc *docIn): docIn(docIn) {}
  virtual ~ECCommand() {}
  virtual void Execute() = 0;
  virtual void UnExecute() = 0;
 protected:
  ECGraphicDoc *getDoc() { return docIn; }
 private:
  ECGraphicDoc *docIn;
};

class CreateShapeCMD: public ECCommand{
 public:
  CreateShapeCMD(ECGraphicDoc *docIn, int x1, int y1, int x2, int y2): ECCommand(docIn), shape(nullptr), p1(std::make_pair(x1, y1)), p2(std::make_pair(x2, y2)), killShape(false) {}
  virtual ~CreateShapeCMD();
  virtual void Execute();
  virtual void UnExecute();

protected:
  ECShape* GetShape() const { return shape; }
  void ChangeShape(ECShape* newshape) { shape = newshape; }
  void GetPoints(std::pair<int, int> &np1, std::pair<int, int> &np2) const { np1 = p1; np2 = p2;}
  void SetKillShape(bool b) { killShape = b; }

 private:
  ECShape* shape;
  std::pair<int, int> p1;
  std::pair<int, int> p2;
  bool killShape;
};

class CreateRectangleCMD: public CreateShapeCMD{
 public:
  CreateRectangleCMD(ECGraphicDoc *docIn, int x1, int y1, int x2, int y2): CreateShapeCMD(docIn, x1, y1, x2, y2) {}
  virtual void Execute();
};

class CreateEllipseCMD: public CreateShapeCMD{
 public:
  CreateEllipseCMD(ECGraphicDoc *docIn, int x1, int y1, int x2, int y2): CreateShapeCMD(docIn, x1, y1, x2, y2) {}
  virtual void Execute();
};

class CreateFilledRectangleCMD : public CreateShapeCMD{
 public:
  CreateFilledRectangleCMD(ECGraphicDoc *docIn, int x1, int y1, int x2, int y2) : CreateShapeCMD(docIn, x1, y1, x2, y2) {}
  virtual void Execute();
};

class CreateFilledEllipseCMD : public CreateShapeCMD{
 public:
  CreateFilledEllipseCMD(ECGraphicDoc *docIn, int x1, int y1, int x2, int y2): CreateShapeCMD(docIn, x1, y1, x2, y2) {}
  virtual void Execute();
};

class DeleteShapeCMD: public CreateShapeCMD{
 public:
  DeleteShapeCMD(ECGraphicDoc *docIn, ECShape *shape): CreateShapeCMD(docIn, 0, 0, 0, 0){ ChangeShape(shape);}

  void Execute();
  void UnExecute();
};

class MoveShapeCMD: public ECCommand{
public:
  MoveShapeCMD(ECGraphicDoc *docIn, std::vector<ECShape *> shape, int xOffset, int yOffset);
  
  void Execute();
  void UnExecute();

 private:
  std::vector<ECShape *> shape;
  int xOffset; int yOffset;
  std::queue<std::pair<int, int> > points;
};


class GroupShapesCMD: public ECCommand{
 public:
  GroupShapesCMD(ECGraphicDoc *docIn, std::vector<ECShape *> shapes): ECCommand(docIn), shapes(shapes), killShape(false), composite(nullptr) {}
  virtual ~GroupShapesCMD();

  void Execute();
  void UnExecute();
  ECShape* GetComp() { return composite; }

 protected:
  void SetComp(ECShape *shape) { composite = shape; }
  void SetShapeList(std::vector<ECShape *> nlist) { shapes = nlist; }

 private:
  std::vector<ECShape *> shapes;
  bool killShape;
  ECShape* composite;
};

class UnGroupShapeCMD: public GroupShapesCMD{
 public:
  UnGroupShapeCMD(ECGraphicDoc *docIn, ECShape *composite);
  void Execute();
  void UnExecute();
};

// ***********************************************

class ECCommandHistory{
 public:
  ECCommandHistory();
  ~ECCommandHistory();
  bool Undo();
  bool Redo();
  void ExecuteCmd( ECCommand *pCmd );
 private:
  std::stack<ECCommand *> history;
  std::stack<ECCommand *> rhistory;
};

#endif
