#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <gtkmm.h>
#include <iostream>
#include <thread>
#include <functional>
#include "GameModel.h"
#include "TextView.h"
#include "GraphicView.h"
#include "Controller.h"
extern int optind, opterr, optopt;
extern char *optarg;

static struct option long_options[]=
{
  {"text", no_argument,NULL, 't'},
  {"seed", required_argument, NULL, 'd'},
  {"scriptfile", required_argument, NULL, 'e'},
  {"startlevel", required_argument, NULL, 'l'},
  {"disableBonus", no_argument, NULL, 'b'},
  {NULL, 0,0,0} //DON'T know why but prevent segmentation fault
};

void graphicIni(Glib::RefPtr<Gtk::Application> app, GraphicView &window){
  app->run(window);
}

//void graphicIni(GraphicView &window){
//  return;
//}
int main(int argc, char *argv[]){

    int index = 0;
    int command = 0; //receipt options
    
    int seed = 0;
    int startlevel = 0;
    bool textOnly = false;
    bool disableBonus = false;
    std::string file = "sequence.txt";

    while(-1 != (command = getopt_long_only(argc, argv, "", long_options, &index))){
      switch(command){
        case 't':
          //text only view
          textOnly = true;
          break;
        case 'd':
          //set seed
          {
            seed = atoi(optarg);
            break;
          }
        case 'e':
          //set scriptfile
          {
            file = optarg;
            break;
          }
        case 'l':
          //set startlevel
          {
            startlevel = atoi(optarg);
            break;
          }
        case 'b':
          {
            disableBonus = true;
            break;
          }
        case '?':
          //unsupported command
          std::cout<< "Unknown option" << std::endl;
          break;
        default:
          break;
      }
    }

    GameModel* g = new GameModel(seed, startlevel, file);
    TextView* t = new TextView(g);
    g->subscribe(t);
    //g->initializeCurNextBlock();
    g->generateNextBlock();
    Controller c = Controller(g, disableBonus);

    if(!textOnly){
      auto app = Gtk::Application::create("org.gtkmm.examples.base");
      GraphicView* window = new GraphicView(g, &c, disableBonus);
      window->set_default_size(240, 500);

      g->subscribe(window);

      //another thread for graphics view
      //std::thread graphic{&Controller::runWhileLoop, &c};
      std::thread graphic{graphicIni, std::ref(app), std::ref(*window)};
      //app->run(window);
      c.runWhileLoop();
      graphic.join();
      delete window;
    }else{
      c.runWhileLoop();
    }

    delete g;
    delete t;

    return 0;
}
