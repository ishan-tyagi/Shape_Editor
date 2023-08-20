#include "ECObserver.h"
#include "ECGraphicViewImp.h"
#include <iostream>

bool ECObserver::IsOnShape(){ //tells if mouse is on a shape or not
  int x1, y1;
  GVImp->GetCursorPosition(x1, y1);
  return GCtrl->IsCurHeld(x1, y1);
}

void ECModeObserver::Update(){ //handles change of mode (insertion or edit)
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(cur == ECGV_EV_KEY_DOWN_SPACE){
    if(getCtrl()->IsSelected() && !getCtrl()->GetMode())
      getCtrl()->DeSelect();
    getCtrl()->ChangeMode();  
  }
}

void ECCreationEvtObserver::Update(){ //handles creation of shapes 
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(!drawing && getCtrl()->GetMode() && cur == ECGV_EV_MOUSE_BUTTON_DOWN){
    drawing = true;
    getImp()->GetCursorPosition(x1, y1);
  }
  else if(drawing && getCtrl()->GetMode() && cur == ECGV_EV_MOUSE_MOVING){
    getImp()->GetCursorPosition(x2, y2);
    getCtrl()->CreationTemp(x1, y1, x2, y2);
    ctemp = true;
  }
  else if(drawing && getCtrl()->GetMode() && cur == ECGV_EV_MOUSE_BUTTON_UP){
    getImp()->GetCursorPosition(x2, y2);
    if(ctemp){
      getCtrl()->RemoveTemp();
      ctemp = false;
    }
    getCtrl()->CreateShape(x1, y1, x2, y2);
    drawing = false;
  }
}

void ECTimerObserver::Update(){ //timer, handles redraw
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(cur == ECGV_EV_TIMER)
    getCtrl()->RedrawShapes();
}


void ECSelectObserver::Update(){ //handles selection, should work with all shapes //bug when moving, deleteing, and undoing
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(!getCtrl()->IsSelected() && !getCtrl()->GetMode() && getCtrl()->NumSelected() < 1 && cur == ECGV_EV_MOUSE_BUTTON_DOWN){
    int x1, y1;
    getImp()->GetCursorPosition(x1, y1);
    getCtrl()->IsCurSelected(x1, y1);
  }
  else if(!getCtrl()->GetMode() && getCtrl()->IsSelected() && !IsOnShape() && getCtrl()->NumSelected() >= 1 && cur == ECGV_EV_MOUSE_BUTTON_DOWN){
    getCtrl()->DeSelect(); 
  }
}

void ECMultiSelectObserver::Update(){
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(!selecting && !getCtrl()->IsSelected() && !getCtrl()->GetMode() && cur == ECGV_EV_KEY_DOWN_CTRL)
    selecting = true;
  else if(!getCtrl()->GetMode() && cur == ECGV_EV_KEY_UP_CTRL)
    selecting = false;
  else if(selecting && !getCtrl()->GetMode() && cur == ECGV_EV_MOUSE_BUTTON_DOWN){
    int x, y;
    getImp()->GetCursorPosition(x, y);
    getCtrl()->IsCurSelected(x, y);
  }
}

void ECMoveObserver::Update(){ //handles move
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(cur == ECGV_EV_KEY_DOWN_CTRL)
    multisel = true;
  else if(cur == ECGV_EV_KEY_UP_CTRL)
    multisel = false;

  if (!multisel && getCtrl()->IsSelected() && !moving && !getCtrl()->GetMode() && getCtrl()->NumSelected() == 1 && cur == ECGV_EV_MOUSE_BUTTON_DOWN){   
    getImp()->GetCursorPosition(orig_x1, orig_y1);
    moving = IsOnShape();
    getCtrl()->preMove();
    getCtrl()->MoveTemp(0, 0);
  }
  else if (moving && !getCtrl()->GetMode() && getCtrl()->IsSelected() && cur == ECGV_EV_MOUSE_MOVING){
    int x1, y1;
    getImp()->GetCursorPosition(x1, y1);
    x_offset = x1 - orig_x1;
    y_offset = y1 - orig_y1;
    getCtrl()->MoveTemp(x_offset, y_offset);
  }
  else if (moving && !getCtrl()->GetMode() && getCtrl()->IsSelected() && cur == ECGV_EV_MOUSE_BUTTON_UP){
    int x1, y1;
    moving = false;
    getImp()->GetCursorPosition(x1, y1);
    x_offset = x1 - orig_x1;
    y_offset = y1 - orig_y1;
    if(x_offset != 0 && y_offset != 0)
      getCtrl()->Move(x_offset, y_offset);
    else  
      getCtrl()->NoMove();
    getCtrl()->RemoveTemp();  
  }
}

void ECDeleteObserver::Update(){
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(getCtrl()->IsSelected() && !getCtrl()->GetMode() && cur == ECGV_EV_KEY_DOWN_D){
    getCtrl()->DeleteShape();
  }
}

void ECCmdHistoryObserver::Update(){
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(cur == ECGV_EV_KEY_DOWN_Z){      //Undo
    if(getCtrl()->IsSelected())
      getCtrl()->DeSelect();
    getCtrl()->Undo();
  }
  else if(cur == ECGV_EV_KEY_DOWN_Y){ //Redo
    if(getCtrl()->IsSelected())
      getCtrl()->DeSelect();
    getCtrl()->Redo();
  }
}

void ECShapeToDrawObserver::Update(){
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(getCtrl()->GetMode() && cur == ECGV_EV_KEY_DOWN_G)
    getCtrl()->ChangeShapeDraw();
}

void ECFilledModeObserver::Update(){
  ECGVEventType cur = getImp()->GetCurrEvent();
  if (getCtrl()->GetMode() && cur == ECGV_EV_KEY_DOWN_F)
    getCtrl()->ChangeFilled();
}

void ECButtonMoveObserver::Update(){
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(!getCtrl()->GetMode() && getCtrl()->IsSelected()){
    switch(cur){
      case(ECGV_EV_KEY_DOWN_UP): getCtrl()->Move(0, -50); break;
      case(ECGV_EV_KEY_DOWN_DOWN): getCtrl()->Move(0, 50); break;
      case(ECGV_EV_KEY_DOWN_LEFT): getCtrl()->Move(-50, 0); break;
      case(ECGV_EV_KEY_DOWN_RIGHT): getCtrl()->Move(50, 0); break;
      default: break;
    }
  }
}

void ECGroupingObserver::Update(){
  ECGVEventType cur = getImp()->GetCurrEvent();
  if(!getCtrl()->GetMode() && getCtrl()->NumSelected() > 1 && cur == ECGV_EV_KEY_UP_G){
    getCtrl()->GroupSelectedShapes();
    getCtrl()->DeSelect();
  }
  else if(!getCtrl()->GetMode() && getCtrl()->NumSelected() == 1 && cur == ECGV_EV_KEY_UP_G){
    getCtrl()->UnGroupSelectedShapes();
    getCtrl()->DeSelect();
  }
}




