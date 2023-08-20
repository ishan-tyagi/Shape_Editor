//  ECGraphicDoc.h
//
//
//  Created by Ishan Tyagi on 11/29/22.
//
//

#ifndef ECGRAPHICDOC_H
#define ECGRAPHICDOC_H

#include <vector>
#include <string>
#include <fstream>

class ECGraphicViewImp;
class ECShape;
class ECRectangle;


class ECGraphicDoc{
 public:
  ECGraphicDoc(ECGraphicViewImp &gvi, std::fstream &infile, std::string filename, bool saving, bool loading);
  ~ECGraphicDoc();

  ECShape* LoadRectangle(bool addToVec=true);
  ECShape* LoadEllipse(bool addToVec=true);
  ECShape* LoadFilledRectangle(bool addToVec=true);
  ECShape* LoadFilledEllipse(bool addToVec=true);
  ECShape* LoadComposite(bool addToVec=true);

  void Load();
  void Save() const;

  ECShape* AddRectangle(int x1, int y1, int x2, int y2, int thickness=3, int color=0, bool pushToVec=true);
  ECShape* AddEllipse(int x1, int y1, int x2, int y2, int thickness = 3, int color = 0, bool pushToVec=true);
  ECShape *AddFilledRectanlge(int x1, int y1, int x2, int y2, int color = 0, bool pushToVec = true);
  ECShape *AddFilledEllipse(int x1, int y1, int x2, int y2, int color = 0, bool pushToVec = true);
  ECShape *AddComposite(std::vector<ECShape *> moveTempShapes, bool pushToVec = true, int x=0, int y=0);

  ECShape* GetShape(int i) const { return listShapes[i]; } 
  int GetNumShapes() const { return listShapes.size(); }
  void AddShape(ECShape* shape) { listShapes.push_back(shape); }
  void DeleteShape(ECShape* shape);

  void EditTemp(int x1, int y1, int x2, int y2, int col=0, int thickness=3);
  void RemoveTemp();

  void MoveShape(ECShape* shape, std::pair<int, int> np1, std::pair<int, int> np2);

private:
  bool saving, loading;
  std::fstream &infile;
  std::string filename;
  ECGraphicViewImp &gvi;
  std::vector<ECShape *> listShapes;
};

#endif


