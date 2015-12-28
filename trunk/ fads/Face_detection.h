////////////////////////////////////////////////////////////////////////////////
//                 HEADER FILE FOR Face_detection.cxx 
////////////////////////////////////////////////////////////////////////////////

#ifndef Face_detection_h
#define Face_detection_h

#include "main.h"


IplImage* detect_and_draw( IplImage*);
int cutface_and_save( IplImage*,IplImage**,IplImage**,IplImage** );
int load_casades(void);
void release_cascades();

void rotate_point(CvPoint* ,CvPoint* ,float );
int rotate_face(IplImage *,CvPoint ,CvPoint ,IplImage * );
int check_image_ROI(IplImage* ,double, double,double ,double ,double);
void rotate_image(double , float ,float ,IplImage *,IplImage *);

extern CvHaarClassifierCascade* cascade_face;//Stores the cascade of face
extern CvHaarClassifierCascade* cascade_eye;//Stores the cascade of eye
extern CvHaarClassifierCascade* cascade_nose;//Stores the cascade of nose

extern CvMemStorage* storage_face;// Storage to store faces detected
extern CvMemStorage* storage_eye;// Storage to store eyes detected
extern CvMemStorage* storage_nose;// Storage to store nose detected


#endif  

 
