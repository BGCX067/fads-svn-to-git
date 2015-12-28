// generated by Fast Light User Interface Designer (fluid) version 1.0109

#include "recognition.h"

int main(int argc, char **argv) {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = new Fl_Double_Window(460, 178, "Recognition");
    w = o;
    { Fl_Text_Display* o = new Fl_Text_Display(30, 110, 410, 50, "RECOGNISING YOUR FACE");
      o->box(FL_PLASTIC_DOWN_BOX);
      o->color((Fl_Color)247);
      o->selection_color((Fl_Color)55);
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelsize(17);
      o->textfont(1);
      o->textsize(18);
      o->textcolor(136);
    } // Fl_Text_Display* o
    { Fl_Progress* o = new Fl_Progress(30, 35, 400, 35, "processing data");
      o->box(FL_PLASTIC_DOWN_BOX);
      o->color((Fl_Color)55);
      o->selection_color((Fl_Color)176);
    } // Fl_Progress* o
    o->end();
  } // Fl_Double_Window* o
  w->show(argc, argv);
  return Fl::run();
}