//
//  ECObserver.h
//
//
//  Created by Yufeng Wu on 2/27/20.
//  Contributions by Ishan Tyagi
//
//

#ifndef ECOBERVER_H
#define ECOBERVER_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "ECGraphicCtrl.h"

//********************************************
// Observer design pattern: observer interface

class ECGraphicCtrl;
class ECGraphicViewImp;

class ECObserver
{
 public:
    ECObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): GVImp(GVImp), GCtrl(GCtrl){}

    virtual ~ECObserver() {}
    ECGraphicViewImp* getImp() { return GVImp; }
    ECGraphicCtrl* getCtrl() { return GCtrl; }
    bool IsOnShape();
    virtual void Update() = 0;

 private:
  ECGraphicViewImp* GVImp; 
  ECGraphicCtrl* GCtrl;
};

class ECCreationEvtObserver: public ECObserver{
 public:
  ECCreationEvtObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): ECObserver(GVImp, GCtrl) { drawing = false; ctemp = false;}
  virtual void Update();

 private:
  bool drawing;
  bool ctemp;
  int x1;
  int y1;
  int x2;
  int y2;
};


class ECModeObserver: public ECObserver{
 public:
  ECModeObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): 
    ECObserver(GVImp, GCtrl) {}

  virtual void Update();
};


class ECTimerObserver: public ECObserver{
 public:
  ECTimerObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): 
    ECObserver(GVImp, GCtrl) {}
  
  virtual void Update();
};


class ECSelectObserver: public ECObserver{
 public:
  ECSelectObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): 
    ECObserver(GVImp, GCtrl) {}
  
  virtual void Update();
};

class ECMultiSelectObserver: public ECObserver{
 public:
  ECMultiSelectObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl):
    ECObserver(GVImp, GCtrl) { selecting = false; }

  virtual void Update();
 private:
  bool selecting;
};


class ECDeleteObserver: public ECObserver{
 public:
  ECDeleteObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): 
    ECObserver(GVImp, GCtrl) {}

  virtual void Update();
};


class ECCmdHistoryObserver: public ECObserver{
 public:
  ECCmdHistoryObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): 
    ECObserver(GVImp, GCtrl) {}

  virtual void Update();
};

class ECMoveObserver: public ECObserver{
 public:
  ECMoveObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): 
    ECObserver(GVImp, GCtrl) { moving = false; multisel = false; }

  virtual void Update();
 private:
  int orig_x1;
  int orig_y1;
  int x_offset;
  int y_offset;
  bool moving;
  bool multisel;
};

class ECShapeToDrawObserver: public ECObserver{
 public:
  ECShapeToDrawObserver(ECGraphicViewImp* GVImp, ECGraphicCtrl* GCtrl): ECObserver(GVImp, GCtrl) {}
  virtual void Update();
};

class ECFilledModeObserver : public ECObserver{
 public:
  ECFilledModeObserver(ECGraphicViewImp *GVImp, ECGraphicCtrl *GCtrl) : ECObserver(GVImp, GCtrl) {}
  virtual void Update();
};

class ECButtonMoveObserver: public ECObserver{
 public:
  ECButtonMoveObserver(ECGraphicViewImp *GVImp, ECGraphicCtrl *GCtrl) : ECObserver(GVImp, GCtrl) {}
  virtual void Update();
};

class ECGroupingObserver: public ECObserver{
 public:
  ECGroupingObserver(ECGraphicViewImp *GVImp, ECGraphicCtrl *GCtrl) : ECObserver(GVImp, GCtrl) {} 
  virtual void Update();
};

//********************************************
// Observer design pattern: subject

class ECObserverSubject
{
 public:
    ECObserverSubject() {}
    virtual ~ECObserverSubject() {}

    void Attach( ECObserver *pObs ){
        //std::cout << "Adding an observer.\n";
        listObservers.push_back(pObs);
    }

    int GetNumObservers() const{
      return listObservers.size();
    }

    ECObserver* GetObserver(int x) const{
      return listObservers[x];
    }

    void Detach( ECObserver *pObs ){
        listObservers.erase(std::remove(listObservers.begin(), listObservers.end(), pObs), listObservers.end());
    }

    void Notify(){
        //std::cout << "Notify: number of observer: " << listObservers.size() << std::endl;
        for(unsigned int i=0; i<listObservers.size(); ++i){
            listObservers[i]->Update();
        }
    }
    
 private:
    std::vector<ECObserver *> listObservers;
};


#endif
