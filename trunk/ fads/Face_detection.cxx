////////////////////////////////////////////////////////////////////////////////
///////////////  FACE and EYE DETECTION FUCNTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "Face_detection.h"
#include "Face_authentication.h"

CvHaarClassifierCascade* cascade_face=0;//Stores the cascade of face
CvHaarClassifierCascade* cascade_eye=0;//Stores the cascade of eye
CvHaarClassifierCascade* cascade_nose=0;//Stores the cascade of nose

CvMemStorage* storage_face=0;// Storage to store faces detected
CvMemStorage* storage_eye=0;// Storage to store eyes detected
CvMemStorage* storage_nose=0;// Storage to store nose detected

////////////////////////////////////////////////////////////////////////////////
// Description :- Function to detect face and draw rectangle 
//                around the face (min 100 X 100)
// Arugment :- IplImage (the image in which face is to be detected)
// Returns :- IplImage (Image with rectangle drawn around the face)
// Called by :- void Playing_CB(void*)
// Requires opencv.h
////////////////////////////////////////////////////////////////////////////////

IplImage* detect_and_draw( IplImage* img )
{

    if( img->origin != IPL_ORIGIN_TL )
       cvFlip( img, img, 0 );
    
    // This is used to scale down the image
	double scale = 2;
    

	// create a gray image same sized to that of image
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    
	// Create scaled down image of the frame by a scale of "scale" value
	IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),cvRound (img->height/scale)),8, 1 );
    
	// convert the image to gray scale
    cvCvtColor( img, gray, CV_BGR2GRAY );
    
	//copy the resize gray image to small_img variable
	cvResize( gray, small_img, CV_INTER_LINEAR );
    
	// Increase contrast by equlising histogram of image
	cvEqualizeHist( small_img, small_img );
    
	// Clear the storage memory area used to store detected faces
	cvClearMemStorage( storage_face);

    if( cascade_face )
    {
   
        /////////////////////////////////// Function discrption //////////////////////////////
		

		CvSeq* faces = cvHaarDetectObjects( small_img, cascade_face, storage_face,
                                            1.1, 2, 0,
                                            cvSize(100,100) );
 
		
		for( int i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect r = *(CvRect*)cvGetSeqElem( faces, i );
    		
			// Draw rectangle around face
			cvRectangle( img, cvPoint((int)(r.x*scale),(int)(r.y*scale) ),cvPoint( (int)((r.x+r.width)*scale),(int)((r.y+r.height)*scale) ),CV_RGB(0,255,0), 3 );
			
		}
    }

        
	cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
    return img;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Description:- Function to Extract face and detect eyes snd nose in it ///////
//                image size to be saved  120 X 140  ///////////////////////////
// Input :- IplImage (The image in which face is to extracted
// Returns :- int (according to face is detected equals to one
// Called by :- void Capture_CB(Fl_Button*, void*)
// Misc :- Sets the golbal IplIamges IplImage *img,*face_result_color=0,
//         *face_result_gray=0,*face_result_rotated=0; 
////////////////////////////////////////////////////////////////////////////////
int cutface_and_save( IplImage* img, IplImage** face_result_color, IplImage** face_result_gray, IplImage** face_result_rotated  )
{

    double scale = 2;
	IplImage* face_gray=0;
	IplImage* face_color=0;
	
	IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    IplImage* small_img_gray = cvCreateImage( cvSize( cvRound (img->width/scale),cvRound (img->height/scale)),8, 1 );
    IplImage* small_img_color=cvCreateImage( cvSize( cvRound (img->width/scale),cvRound (img->height/scale)),8, 3 );

    cvCvtColor( img, gray, CV_BGR2GRAY );
   	
	cvResize( gray, small_img_gray, CV_INTER_LINEAR );
	cvResize( img, small_img_color, CV_INTER_LINEAR );

	//cvEqualizeHist( small_img_gray, small_img_gray );
   	cvClearMemStorage( storage_face);

    
    if( cascade_face )
    {
   
        // detect face
		CvSeq* faces = cvHaarDetectObjects( small_img_gray, cascade_face, storage_face,
                                            1.1, 2, 0,
                                            cvSize(100, 100) );
        
		// IF no face is detected or more than one face detected return 
		if(faces->total > 1 )
		{
			fl_alert("More than one face detected .. try again!");
			return 0;
		}

		else if (faces->total == 0)
		{
			fl_alert("No face detected .. try again!");
			return 0;
		}

		else
		{

			CvRect r = *(CvRect*)cvGetSeqElem( faces, 0 );
			r.x+=5;
            r.width-=5;
            r.y+=5;
            r.height-=5;			
		    
/////////////////////// Cut face form small_img ///////////////////////////////////

			face_gray=cvCreateImage(cvSize(r.width,r.height), 8, 1);
			face_color=cvCreateImage(cvSize(r.width,r.height), 8, 3);
			IplImage *left_face,*right_face,*nose_part;
            left_face=cvCreateImage(cvSize(r.width/2,r.height), 8, 1);
            right_face=cvCreateImage(cvSize(r.width/2,r.height), 8, 1);
                       
            (*face_result_color)=cvCreateImage(cvSize(120,140),8,3);
            (*face_result_gray)=cvCreateImage(cvSize(120,140),8,1);	
            (*face_result_rotated)=cvCreateImage(cvSize(120,140),8,1);		 
             
             // copy
			 cvSetImageROI(small_img_gray,r);
			 cvSetImageROI(small_img_color,r);
			 
			 cvCopy( small_img_gray, face_gray );
			 cvCopy( small_img_color , face_color );
			
             //copy left part
             CvRect r1;
             r1.x=0;
             r1.width=r.width/2;
             r1.y=0;
             r1.height=r.height;
             cvSetImageROI(face_gray,r1);
             cvCopy( face_gray, left_face );
             // copy right part
             cvResetImageROI(face_gray);
             r1.x=r.width/2;
             r1.width=r.width/2;
             r1.y=0;
             r1.height=r.height;
             cvSetImageROI(face_gray,r1);
             cvCopy( face_gray, right_face);
             cvResetImageROI(face_gray);
             
             CvPoint left_eye_pt,right_eye_pt,nose_pt,center;
             left_eye_pt.x=0;
             left_eye_pt.y=0;
             right_eye_pt.x=0;
             right_eye_pt.y=0;
             
 /////////////////////// detect eye and mark it in face_color /////////////////////
    	     cvClearMemStorage( storage_eye);
            {CvSeq* eyes = cvHaarDetectObjects( left_face, cascade_eye, storage_eye,
                                            1.1, 2, 0,
                                          cvSize(30,30) );
	                                             
    		    if(eyes->total)
                {	
                    CvRect r1 = *(CvRect*)cvGetSeqElem( eyes, 0 );
    				center.x = cvRound (r1.x + r1.width*0.5);
    				center.y = cvRound (r1.y + r1.height*0.5);
                	//radius = cvRound((r1.width + r1.height)*0.05);
                	left_eye_pt=center;				
                 }
             }
            cvClearMemStorage( storage_eye);
            
			{CvSeq*eyes = cvHaarDetectObjects( right_face, cascade_eye, storage_eye,
                                            1.1, 2, 0,
                                          cvSize(30,30) );
                if(eyes->total)
                {
                    CvRect r1 = *(CvRect*)cvGetSeqElem( eyes, 0 );
    				center.x = cvRound (r1.x + r1.width*0.5);
    				center.y = cvRound (r1.y + r1.height*0.5);
                	//radius = cvRound((r1.width + r1.height)*0.05);
                	right_eye_pt=center;
                	right_eye_pt.x+=r.width/2;
                 }				
             }
      
        rotate_face( face_color,left_eye_pt,right_eye_pt,(*face_result_rotated) );
   
   ///////////////// detect nose and mark it in face_color /////////////////////////////////
		  	//copy nose part
             //CvRect r1;
             r1.x=r.width/4;
             r1.width=r.width/2;
             r1.y=r.height/3;
             r1.height=r.height/3;
             cvSetImageROI(face_gray,r1);
             nose_part=cvCreateImage(cvSize(r1.width,r1.height),8,1);
             cvCopy( face_gray, nose_part );
               
          cvClearMemStorage( storage_nose);
          CvSeq* nose_seq = cvHaarDetectObjects( nose_part, cascade_nose, storage_nose,
                                            1.1, 2, 0,
                                          cvSize(30, 30) );
	 
			for(int i = 0; i < (nose_seq ? nose_seq->total : 0); i++ )
			 {
				CvRect r2 = *(CvRect*)cvGetSeqElem( nose_seq, i );

				center.x = cvRound (r2.x + r2.width*0.5);
				center.y = cvRound (r2.y + r2.height*0.5);
				int radius = cvRound((r2.width + r2.height)*0.05);
            	
				cvCircle( face_color, center, radius, CV_RGB(255,255,0), 1, 8, 0 );
				nose_pt=center;
			 }
		
            // draw lines between eyes and nose
			cvLine(face_color,nose_pt,left_eye_pt,CV_RGB(0,255,0),1,8,0);
			cvLine(face_color,nose_pt,right_eye_pt,CV_RGB(0,255,0),1,8,0);
			cvLine(face_color,left_eye_pt,right_eye_pt,CV_RGB(0,255,0),1,8,0);

            // resize image			
            cvResize( face_color,(*face_result_color), CV_INTER_LINEAR );
			cvResize( face_gray,(*face_result_gray), CV_INTER_LINEAR );
		    		
            cvReleaseImage( &left_face );	
            cvReleaseImage( &right_face);
            cvReleaseImage( &nose_part);
         }
	}
    
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img_color );
    cvReleaseImage( &small_img_gray );
	cvReleaseImage( &face_gray );
	cvReleaseImage( &face_color );
    	
    
    
    return 1;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Description:- Function to load HAAR CASCADES 
// Input :- void
// Returns :- int (according to error in loading cascade
// Called by :- main()
///////////////////////////////////////////////////////////////////////////////

int load_casades()
{
 
    const char* cascade_name_face ="haar_cascades\\haarcascade_face.xml";//Path of cascade of face
    const char* cascade_name_eye ="haar_cascades\\haarcascade_eye1.xml";//Path of cascade of eye
    const char* cascade_name_nose ="haar_cascades\\haarcascade_nose.xml";//Path of cascade of nose
    
    cascade_face = (CvHaarClassifierCascade*)cvLoad( cascade_name_face, 0, 0, 0 );
	cascade_eye = (CvHaarClassifierCascade*)cvLoad( cascade_name_eye, 0, 0, 0 );
	cascade_nose = (CvHaarClassifierCascade*)cvLoad( cascade_name_nose, 0, 0, 0 );
    	
	// If any cascade cannot be loaded the print error and exit
	if( !cascade_face )
    {
        fl_alert("ERROR: Could not load classifier of face  cascade form \"haar_cascades\" folder" );
        return -1;
    }

	if( !cascade_eye )
    {
        fl_alert("ERROR: Could not load classifier of eye cascade form \"haar_cascades\" folder" );
        return -1;
    }

	if( !cascade_nose )
    {
        fl_alert("ERROR: Could not load classifier of nose cascade form \"haar_cascades\" folder" );
        return -1;
    }
    
    // Create memory storage for storing detected objects
	storage_face = cvCreateMemStorage(0);
    storage_eye = cvCreateMemStorage(0);
	storage_nose = cvCreateMemStorage(0);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void rotate_point(CvPoint* srcP,CvPoint* dstP,float angle)
{
    CvPoint2D32f p1;
    CvPoint2D32f p2;
    p1.x=(*srcP).x;
    p1.y=(*srcP).y;
    CvMat src = cvMat( 1, 1, CV_32FC2, &p1 );
    CvMat dst = cvMat( 1, 1, CV_32FC2, &p2 );
    CvMat *translate = cvCreateMat(2, 3, CV_32FC1);
    CvPoint2D32f centre;
    centre.x = 0;
    centre.y = 0;
    cv2DRotationMatrix(centre, angle, 1.0, translate);
    cvTransform( &src, &dst, translate, NULL );
    (*dstP).x=(int)(p2.x);
    (*dstP).y=(int)(p2.y);
    cvReleaseMat(&translate);

}

////////////////////////////////////////////////////////////////////////////////
// Description:- Function to align face according to eyes 
//   
// Input :- eye cordinate
// Returns :- int (according to error in loading cascade
// Called by :- cut_and_save()
///////////////////////////////////////////////////////////////////////////////
int rotate_face(IplImage * img,CvPoint plefteye,CvPoint prighteye,IplImage * face)
{

    cvZero(face);
    
	double h=(prighteye.y-(prighteye.y+plefteye.y)/2);
    double w=prighteye.x - (prighteye.x+plefteye.x)/2;
    double an= atan(h/w);
    an=an*180;
    an=an/CV_PI;
    
	double negFlag=0;
    if (an<0)
    {
        negFlag=1;
        an=-1*an;
        cvFlip( img, img, 0 );
        prighteye.y=img->height-prighteye.y;
        plefteye.y=img->height-plefteye.y;

    }
    
	IplImage* rotatedImg = cvCreateImage( cvSize(img->width*3,img->height*3),8,3);
    
	if (check_image_ROI(rotatedImg,img->width,img->height,img->width,img->height,1)==-1) 
		return 0;
    
	cvSetImageROI(rotatedImg,cvRect(img->width,img->height,img->width,img->height));
    cvResize(img,rotatedImg,CV_INTER_LINEAR);
    
	cvResetImageROI(rotatedImg);
    
	CvPoint pLeftCordWithOut={(plefteye.x+img->width),(plefteye.y+img->height)};
    CvPoint pLeftCordTrans;
    rotate_point(&pLeftCordWithOut,&plefteye ,an);
    
	CvPoint pRightCordWithOut={(prighteye.x+img->width),(prighteye.y+img->height)};
    CvPoint pRightCordTrans;
    rotate_point(&pRightCordWithOut,&prighteye ,an);
    
	rotate_image(an,0 ,0,rotatedImg,rotatedImg);
    
	if (negFlag==1)
    {
        cvFlip( rotatedImg, rotatedImg, 0 );
        cvFlip( img, img, 0 );
        prighteye.y=rotatedImg->height-prighteye.y;
        plefteye.y=rotatedImg->height-plefteye.y;

    }
    double width=(prighteye.x-plefteye.x);
    double ratio= 80/width;
    double reWidth=img->width*3*ratio;
    double reHeight=img->height*3*ratio;
    
	if (ratio>.5  && ratio<1.75 && reWidth>0 && reHeight>0)
    {

        IplImage* resizedImg = cvCreateImage( cvSize(reWidth,reHeight),8,1);
        IplImage* gray = cvCreateImage( cvSize(img->width*3,img->height*3), 8, 1 );
        
		cvCvtColor( rotatedImg, gray, CV_BGR2GRAY );
        cvResize( gray, resizedImg, CV_INTER_LINEAR );
        
		prighteye.x*=ratio;
        prighteye.y*=ratio;
        plefteye.x*=ratio;
        plefteye.y*=ratio;
        
		CvPoint p1LeftTop,p1RightTop;
        
		p1LeftTop.x=plefteye.x-40;
        p1LeftTop.y=((plefteye.y+prighteye.y)/2)-55;
        p1RightTop.x=prighteye.x+45;
        p1RightTop.y=((plefteye.y+prighteye.y)/2)  +110;
        
		if (p1LeftTop.x>0 && p1LeftTop.y>0&& p1LeftTop.x<resizedImg->width && p1LeftTop.y<resizedImg->height && p1RightTop.x>0 && p1RightTop.y>0&&p1RightTop.x<resizedImg->width && p1RightTop.y<resizedImg->height && p1RightTop.y-p1LeftTop.y<resizedImg->height && p1RightTop.x-p1LeftTop.x <resizedImg->width)
        {
            if (check_image_ROI(resizedImg,p1LeftTop.x,p1LeftTop.y,p1RightTop.x-p1LeftTop.x,p1RightTop.y-p1LeftTop.y,2)!=-1)
            {
                cvSetImageROI(resizedImg,cvRect(p1LeftTop.x,p1LeftTop.y,p1RightTop.x-p1LeftTop.x,p1RightTop.y-p1LeftTop.y));
                
				//cvResetImageROI(resizedImg);
				cvResize(resizedImg,face,CV_INTER_LINEAR);
                cvResetImageROI(resizedImg);

                cvReleaseImage( &resizedImg );
                cvReleaseImage( &gray );
                cvReleaseImage( &rotatedImg );

                return 1;
            }

            // cvReleaseImage( &face );
        }
        cvReleaseImage( &resizedImg );
        cvReleaseImage( &gray );
    }
    cvReleaseImage( &rotatedImg );
	return 0;

}
////////////////////////////////////////////////////////////////////////////////
// Description:- Function to check Image ROI 
//   
// Input :- 
// Returns :- int (according to error in loading cascade
// Called by :- rotate_face
////////////////////////////////////////////////////////////////////////////////
int check_image_ROI(IplImage* img,double x, double y,double width,double height,double fun)
{
    if (img==NULL)
        return -1;
    if (((img->width)<=0) || ((img->height)<=0))
    {
        return -1;
    }
    if ((x)<=0 || (y)<=0)
    {
        return -1;
    }
    if (((width)<=0) || ((height)<=0))
    {
        return -1;
    }
    if ((width)>=(img->width) ||(height)>=(img->height))
    {
        return -1;
    }
    if ((((x)+(width))>=(img->width)) || (((y)+(height))>=(img->height)))
    {
        return -1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Description:- Function to rotate image 
//   
// Input :- angle of rotation, src image, dest image
// Returns :- void
// Called by :- rotate_face
////////////////////////////////////////////////////////////////////////////////
void rotate_image(double angle, float centreX, float centreY,IplImage * img,IplImage * dstimg)
{
    CvPoint2D32f centre;
    CvMat *translate = cvCreateMat(2, 3, CV_32FC1);

    centre.x = centreX;
    centre.y = centreY;

    cv2DRotationMatrix(centre, angle, 1.0, translate);
    cvWarpAffine(img,dstimg,translate,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0));
    cvReleaseMat(&translate);
}
////////////////////////////////////////////////////////////////////////////////
// Description:- Function to release cascades //////////////////////////////////
//   
// Input :- void
// Returns :- void
// Called by :- preprocess
////////////////////////////////////////////////////////////////////////////////
void release_cascades()
{
    cvReleaseHaarClassifierCascade(&cascade_face );
    cvReleaseHaarClassifierCascade(&cascade_eye );
    cvReleaseHaarClassifierCascade(&cascade_nose );
    
    // Release tha storage area 
    cvReleaseMemStorage( &storage_eye );
    cvReleaseMemStorage( &storage_face);
    cvReleaseMemStorage( &storage_nose);
    
}
