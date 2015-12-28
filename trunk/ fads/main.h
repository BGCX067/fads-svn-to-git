// generated by Fast Light User Interface Designer (fluid) version 1.0109

#ifndef main_h
#define main_h

#include <FL/Fl_File_Chooser.H>
#include "Fl_OpenCV.h"
#include "cvaux.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern Fl_OpenCV *Pict; 
extern Fl_Capture Cap;
extern bool playing,extracting; 

#include <FL/Fl.H>
void Exit_CB(Fl_Widget*,void*);
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Display.H>
extern Fl_Text_Display *text1;
extern Fl_Text_Display *heading;
extern Fl_Text_Buffer *heading_buff;

extern Fl_Button *train_button;
extern Fl_Button *authenticate_button;
extern Fl_Button *accept_button;
extern Fl_Button *reject_button;
extern Fl_Button *capture_button;
extern Fl_Input *name_train;
extern Fl_Input *name_authenticate;

void About_CB(Fl_Widget*,void*);
#include <FL/Fl_Button.H>
extern void Capture_CB(Fl_Button*, void*);
#include <FL/Fl_Scroll.H>
extern Fl_Scroll *Board;
void Train_CB(Fl_Widget*,void*);

#include <FL/Fl_Browser.H>
#include <FL/filename.H>
#include <FL/Fl_Progress.H>


extern void main_window_exit_CB(Fl_Double_Window*, void*);
extern void train_window_exit_CB(Fl_Double_Window*, void*);
extern void authenticate_window_exit_CB(Fl_Double_Window*, void*);
extern void about_window_exit_CB(Fl_Double_Window*, void*);
extern void help_window_exit_CB(Fl_Double_Window*, void*);
void recognition_win_exit_CB(Fl_Double_Window*, void*);
void Reject_CB(Fl_Button*, void*);
void Recognition_CB(Fl_Button*, void*);

void submit_train_CB(Fl_Widget*,void*); 
void submit_authenticate_CB(Fl_Widget*,void*);

bool OpenCAM_CB();
void Playing_CB(void*);

// global variables
extern IplImage *face_result_rotated,*face_result_color,*face_result_gray,*img;


int find_user(char *);
int add_user(char *);
int next_user(char *,int );
int find_user_no(void);
void delete_user(char *);
#endif