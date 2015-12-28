////////////////////////////////////////////////////////////////////////////////
//                 HEADER FILE FOR Face_authentication.cxx 
////////////////////////////////////////////////////////////////////////////////

#ifndef Face_authentication_h
#define Face_authentication_h

#include "main.h"
int lbp_operator(IplImage*,IplImage**);
double get_bitval(IplImage*,double,double,double);
double hist_difference(IplImage* img1,IplImage* img2);
int check_bit(int i);
double lbp_diff(IplImage* ,IplImage* );
double template_match( IplImage*, IplImage*);
double face_template_match( IplImage*, IplImage*);


#endif  

 
