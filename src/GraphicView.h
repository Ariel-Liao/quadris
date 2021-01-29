#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include "Observer.h"
#include "GameModel.h"
#include "Pixel.h"
#include "Block.h"
#include "Controller.h"
#include <iostream>
#include <memory>
#include <map>

struct Coordinate;

class GraphicView : public Gtk::Window, public Observer {

  public:
    GraphicView(GameModel* game, Controller* controller, bool disableBonus);
    ~GraphicView(){
      delete _boardArea;
      delete _nextBlockArea;
    };
    void update() override;
  private:

    class MyArea : public Gtk::DrawingArea{
      public:
        MyArea(){};
        virtual ~MyArea(){};
        virtual void update() = 0;
    
      protected:
        virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) = 0;
        void setColor(const Cairo::RefPtr<Cairo::Context>& cr, char block);
    };

    class BoardArea : public MyArea{
      public:
        BoardArea(GameModel* game);
        virtual ~BoardArea(){};
        void update() override;
    
      protected:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
      
      private:
        GameModel* _game;
        std::vector<std::vector <Pixel> > _board;

    };

    class NextBlockArea : public MyArea{
      public:
        NextBlockArea(GameModel*);
        virtual ~NextBlockArea(){};
        void update() override;

      protected:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

      private:
        GameModel* _game;

        std::vector<Coordinate> _points;
        char _block;
    };

    GameModel* _game;
    Controller* _controller;
    GraphicView::BoardArea* _boardArea;
    GraphicView::NextBlockArea* _nextBlockArea;
    Gtk::Box m_Container;
    Gtk::Frame frame_scoreboard, frame_nextBlock;
    Gtk::Label label_scoreboard;
    
    Gtk::Button left_button, right_button, down_button, drop_button, cw_buttion, ccw_button, lu_button, ld_button, re_button, hint_button;

    Gtk::Box H_BOX_1, H_BOX_2, H_BOX_3, H_BOX_4;

    void on_button_clicked(Glib::ustring command);

    bool on_key_press_event(GdkEventKey* key_event);
    //void on_left_button_clicked();
    //void on_left_button_clicked();
    //void on_left_button_clicked();
    //void on_left_button_clicked();
    //void on_left_button_clicked();
    //void on_left_button_clicked();
    //void on_left_button_clicked();
    //void on_left_button_clicked();
};

#endif
