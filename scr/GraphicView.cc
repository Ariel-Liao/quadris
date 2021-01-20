#include "GraphicView.h"
#include <iostream>
#include <sstream>
#include <valarray>

GraphicView::GraphicView(GameModel* game, Controller* controller, bool disableBonus):
  frame_scoreboard("Scored Board"),
  frame_nextBlock("Nect Block"),
  left_button("Left"),
  right_button("Right"),
  down_button("Down"),
  drop_button("Drop"),
  cw_buttion("Rotate Clockwise"),
  ccw_button("Rotate CounterClockwise"),
  lu_button("Level up"),
  ld_button("Level down"),
  re_button("Restart"),
  hint_button("Hint")
{
  set_title("Quadris");
  _game = game;
  _controller = controller;
  _boardArea = new GraphicView::BoardArea(_game);
  _nextBlockArea = new GraphicView::NextBlockArea(_game);

  m_Container.pack_start(frame_scoreboard, Gtk::PACK_SHRINK);
  m_Container.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_Container.add(*_boardArea);
  m_Container.pack_start(frame_nextBlock, Gtk::PACK_SHRINK);

  //set the default score board;
  label_scoreboard.set_justify(Gtk::JUSTIFY_LEFT);
  frame_scoreboard.add(label_scoreboard);

  std::stringstream fmt;
  fmt << "Level: " << _game->getLevel() << std::endl << "Score: " << _game->getScore() << std::endl << "Hi Score: " << _game->getHiScore() << std::endl;
  label_scoreboard.set_text(fmt.str());
  

  //set the next block frame
  frame_nextBlock.add(*_nextBlockArea);

  if (!disableBonus){
    //add button onclcked funtions
    left_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "left") );
    right_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "right") );
    down_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "down") );
    drop_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "drop") );
    cw_buttion.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "clockwise") );
    ccw_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "counterclockwise") );
    lu_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "levelup") );
    ld_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "leveldown") );
    re_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "restart") );
    hint_button.signal_clicked().connect( sigc::bind<Glib::ustring>( sigc::mem_fun(*this, &GraphicView::on_button_clicked), "hint") );

    //add buttons to the screen
    H_BOX_1.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    H_BOX_1.add(left_button);
    H_BOX_1.add(right_button);
    H_BOX_1.add(down_button);
    H_BOX_1.add(drop_button);
    H_BOX_2.add(cw_buttion);
    H_BOX_3.add(ccw_button);
    H_BOX_4.add(lu_button);
    H_BOX_4.add(ld_button);
    

    m_Container.add(H_BOX_1);
    m_Container.add(H_BOX_2);
    m_Container.add(H_BOX_3);
    m_Container.add(H_BOX_4);
    m_Container.add(re_button);
    m_Container.add(hint_button);

    //add keyboard events
    add_events(Gdk::KEY_PRESS_MASK);
  } 
  add(m_Container);
  

  show_all_children();

}

void GraphicView::update(){
  _boardArea->update();

  //update scoreboard
  if(_game->isGameOver()){
    label_scoreboard.set_text("GAME OVER!\n Please Restart.");
  }else{ 
    std::stringstream fmt;
    fmt << "Level: " << _game->getLevel() << std::endl << "Score: " << _game->getScore() << std::endl << "Hi Score: " << _game->getHiScore() << std::endl;
  label_scoreboard.set_text(fmt.str());
  }
  //update Next Block
  _nextBlockArea->update();


}


//Button events

void GraphicView::on_button_clicked(Glib::ustring command){
      _controller->runCommand(command);
}

//Key Events:
bool GraphicView::on_key_press_event(GdkEventKey* key_event){
  //Not sure the type of keyval, thus hard coded
  //
  if (key_event->keyval == GDK_KEY_d){
    _controller->runCommand("right");
    return true;
  }
  if (key_event->keyval == GDK_KEY_a){
    _controller->runCommand("left");
  }
  if (key_event->keyval == GDK_KEY_s){
    _controller->runCommand("down");
  }
  if (key_event->keyval == GDK_KEY_k){
    _controller->runCommand("drop");
  }
  if (key_event->keyval == GDK_KEY_q){
    _controller->runCommand("counterclockwise");
  }
  if (key_event->keyval == GDK_KEY_e){
    _controller->runCommand("clockwise");
  }

  if (key_event->keyval == GDK_KEY_h){
    _controller->runCommand("leveldown");
  }

  if (key_event->keyval == GDK_KEY_y){
    _controller->runCommand("levelup");
  }
  //if (key_event->keyval == GDK
  return Gtk::Window::on_key_press_event(key_event);
}


GraphicView::BoardArea::BoardArea(GameModel* game)
{
  //get the required size for our board
  set_size_request (240,380);
  _game = game;
  _board = _game->getBoard();
}

bool GraphicView::BoardArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
  //std::cout << "DRAWING" << std::endl;

  int x = 10;
  int y = 10;
  int width = 20;
  int height = 20;

  //static const double dashParam[] = {4.0, 1.0};
  
  std::valarray<double> dashParam{4.0, 1.0};

  //Draing the backgroud grid
  cr->save();
  cr->set_source_rgb(0.7, 0.7, 0.7);
  cr->set_line_width(0.5);
  cr->set_dash(dashParam, 0);
  for(int i = 0; i <= NUM_ROWS; i++){
    cr->move_to(x, y + i * height);
    cr->line_to(x + NUM_COLS * width, y + i * height);
  }
  for(int i = 0; i <= NUM_COLS; i++){
    cr->move_to(x + i * width, y);
    cr->line_to(x + i * width, y + NUM_ROWS* height);
  }

  cr->stroke();
  cr->restore();

  //cr->set_source_rgb(0.3, 0.8, 1);
  for(std::vector<Pixel> row: _board){
    for(Pixel p: row){
      if(p.isOccupied()){
        setColor(cr, p.getVal());
        cr->rectangle(x,y,width,height);
        cr->fill_preserve();
        
        //Let cr actually draw it on canvas.
        cr->stroke();
       }
      x += width;
      //std::cout << p.getVal() ;
    }
    x = 10;
    y += height;
    //std::cout << std::endl;
  }
  return true;
}

void GraphicView::BoardArea::update(){
  _board = _game->getBoard();
  queue_draw();
}


//NextBlockArea
//

GraphicView::NextBlockArea::NextBlockArea(GameModel* game){
  set_size_request(80, 80);
  _game = game;
  _block = _game->getNextBlockLetter();
  _points = _game->getNextBlockPoints();
}

bool GraphicView::NextBlockArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
  int x = 10;
  int y = 30;
  int width = 20;
  int height = 20;
  

  for(Coordinate cord: _points){
    setColor(cr, _block);
    //cord x and y are acc row and col
    cr->rectangle(x + cord.y * width, y + cord.x * height, width, height);
    cr->fill_preserve();
    cr->stroke();
  }
  return true;
}

void GraphicView::NextBlockArea::update(){
  _block = _game->getNextBlockLetter();
  _points = _game->getNextBlockPoints();
  queue_draw();
}






void GraphicView::MyArea::setColor(const Cairo::RefPtr<Cairo::Context> & cr, char block){
  switch(block){
    case 'I':
      {
        // IIII
        cr->set_source_rgb(0.3, 0.8, 1);
        break;
      }
    case 'J':
      {
        // J
        // JJJ
        cr->set_source_rgb(0,0,0.7);
        break;
      }
    case 'L':
      {
        //   L
        // LLL
        cr->set_source_rgb(1, 0.45, 0);
        break;
      }
    case 'O':
      {
        // OO
        // OO
        cr->set_source_rgb(1, 0.45, 0.7);
        break;
      }
    case 'S':
      {
        //  SS
        // SS
        cr->set_source_rgb(0, 0.7, 0.2);
        break;
      }
    case 'T':
      {
        //  T
        // TTT
        cr->set_source_rgb(0.66, 0.35, 0.86);
        break;
      }
    case 'Z':
      {
        // ZZ
        //  ZZ
        cr->set_source_rgb(1, 0.2, 0.2);
        break;
      }
    case '*':
      {
        //level4 block
        cr->set_source_rgb(0.67, 0.67, 0.67);
      }
    case '?':
      {
        //hint block
        cr->set_source_rgb(0,0,0);
      }
    default:
      cr->set_source_rgb(0, 0, 0);

  }
}



