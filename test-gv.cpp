//
//  Created by Yufeng Wu on 3/2/22.
//
#include "ECGraphicViewImp.h"
#include "ECGraphicDoc.h"
#include "ECGraphicCtrl.h"
#include <string>
#include <iostream>
#include <fstream>

// Test graphical view code
int real_main(int argc, char **argv)
{
  bool saving = false, loading = false;
  std::string filename = "";
  const int widthWin = 1000, heightWin = 1000;
  std::fstream file; //change this to an ifstream

  if(argc > 1){ //checks if we are saving to a file or not, file closed inside function
    saving = true; loading = true;
    filename = argv[1];
    file.open(filename);
    if(!file.is_open()){
      loading = false;
      file.open(filename, std::ios::out);
    }
  }

  ECGraphicViewImp view(widthWin, heightWin);
  ECGraphicDoc model(view, file, filename, saving, loading);
  ECGraphicCtrl ctrl(view, model);
  
  view.Show();
  return 0;
}

int main(int argc, char **argv)
{
    return real_main(argc, argv);
    //return al_run_main(argc, argv, real_main);
}

