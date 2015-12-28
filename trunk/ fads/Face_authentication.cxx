////////////////////////////////////////////////////////////////////////////////
///////////////  FACE AUTHENTICATION FUCTIONS /// //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "Face_authentication.h"

////////////////////////////////////////////////////////////////////////////////
// Description:- Function to do Linear Binary Pattern operation on /////////////
//               gray scaled images.
// Input :- IplImage* (gray scaled image on which lpb is to be opearated
// Returns :- IplImage* (lbp operated image )
// Called by :- 
////////////////////////////////////////////////////////////////////////////////

int lbp_operator(IplImage* img_src,IplImage** img_dest)
{
    int i=0;
    int j=0;
	(*img_dest)=cvCreateImage( cvSize(img_src->width,img_src->height), 8, 1 );
	
	if(img_src->nChannels!=1)
	{
		fl_alert("Error! Images are not gray scale\n");
		return 0;
	}
    
	cvZero((*img_dest));
  	
	for (i=0;i<img_src->height;i++)
    {

        for (j=0;j<img_src->width;j++)
        {
            int p1x,p2x,p3x,p4x,p5x,p6x,p7x,p8x;
            int p1y,p2y,p3y,p4y,p5y,p6y,p7y,p8y;

            p1x=j-1;
            p1y=i-1;

            p2x=j;
            p2y=i-1;

            p3x=j+1;
            p3y=i-1;

            p4x=j+1;
            p4y=i;

            p5x=j+1;
            p5y=i+1;

            p6x=j;
            p6y=i+1;

            p7x=j-1;
            p7y=i+1;


            p8x=j-1;
            p8y=i;

            CvScalar s;
            s=cvGet2D( img_src,i,j);
            double bit1=128*get_bitval(img_src,p1x,p1y,s.val[0]);
            double bit2=64*get_bitval(img_src,p2x,p2y,s.val[0]);
            double bit3=32*get_bitval(img_src,p3x,p3y,s.val[0]);
            double bit4=16*get_bitval(img_src,p4x,p4y,s.val[0]);
            double bit5=8*get_bitval(img_src,p5x,p5y,s.val[0]);
            double bit6=4*get_bitval(img_src,p6x,p6y,s.val[0]);
            double bit7=2*get_bitval(img_src,p7x,p7y,s.val[0]);
            double bit8=1*get_bitval(img_src,p8x,p8y,s.val[0]);
            CvScalar s1;
            s1.val[0]=bit1+bit2+bit3+bit4+bit5+bit6+bit7+bit8;
            s1.val[1]=0;
            s1.val[2]=0;

            cvSet2D((*img_dest),i,j,s1);
        }
    }
    return 1;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Despcription :- Return the value of 8 pixel surrounded by x y pixel
// Called by :- IplImage* lbp_operator(IplImage* img_src)
// Return value :- double
// Input value :- image, point, and threshold value
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
double get_bitval(IplImage* img,double px,double py,double threshold)
{
    if (px<0 || py<0 || px>=img->width || py>=img->height)
        return 0;
    else
    {
        CvScalar s;
        s=cvGet2D(img,(int)py,(int)px);
        if (s.val[0]>=threshold)
            return 1;
        else
            return 0;
    }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Description:- Function to find histogram difference on two 
//               gray scaled images.
// Input :- 2 IplImage* (gray scaled image )
// Returns :- double (histrogram difference value)
// Called by :- double lbp_diff(IplImage* image1,IplImage* image2)
///////////////////////////////////////////////////////////////////////////////

double hist_difference(IplImage* img1,IplImage* img2)
{

    double hist1[256];
	double hist2[256];
	
	int i,j;
    int m;
    
	// initialise the histigram bins to zero
    for (i=0;i<256;i++)
    {
        hist1[i]=0;
        hist2[i]=0;
    }

    // get image histogram for 1st image
	for (i=0;i<20;i++)
    {
       for (j=0;j<20;j++)
        {
            CvScalar s;
            s=cvGet2D(img1,i,j);
            m=s.val[0];
            hist1[m]++;
        }
    }
    // get image histogram for 2nd image
	for (i=0;i<20;i++)
    {

        for (j=0;j<20;j++)
        {
            CvScalar s;
            s=cvGet2D(img2,i,j);
            m=s.val[0];
            hist2[m]++;
        }
    }

    double chiSquare1=0;
    double chiSquare=0;
  	double lastBin1=0;
    double lastBin2=0;
    
	// find chisqaure distance
	for (i=0;i<256;i++)
    {
        if (check_bit(i)!=-1)
        {
            chiSquare1=0;
            if ((hist1[i]+hist2[i])!=0)
                chiSquare1=((pow(hist1[i]-hist2[i],2)/(hist1[i]+hist2[i])));
            
            chiSquare=chiSquare1+chiSquare;
        }
        {
            lastBin1+=hist1[i];
            lastBin2+=hist2[i];

        }

    }

	if ((lastBin1+lastBin2)!=0)
        chiSquare1=((pow(lastBin1-lastBin2,2)/(lastBin1+lastBin2)));
    
    chiSquare=chiSquare1+chiSquare;
    
	return chiSquare;

}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Description:- Function check bit of no
// Input :- int
// Returns :- int
// Called by :- double hist_difference(IplImage* img1,IplImage* img2)
////////////////////////////////////////////////////////////////////////////////
int check_bit(int i)
{
    int j=i;
    int bit8=(i%2);
    int bit7=((i/2)%2);
    int bit6=((i/4)%2);
    int bit5=((i/8)%2);
    int bit4=((i/16)%2);
    int bit3=((i/32)%2);
    int bit2=((i/64)%2);
    int bit1=((i/128)%2);
    int bitVector[9]  = {bit1,bit8,bit7, bit6, bit5,bit4, bit3,bit2,bit1};

    int current=bitVector[0];
    int count=0;
    for (i=0;i<9;i++)
    {
        if (current!=bitVector[i])
            count++;
        current=bitVector[i];
    }
    if (count>2)
        return -1;
    else
        return 0;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Description:- Finds histogram differece between two lbp operated image of
//               size 120x140 of 4x7 pieces
// Input :- 2 IplImage*
// Returns :- double
// Called by :- 
////////////////////////////////////////////////////////////////////////////////
double lbp_diff(IplImage* image1,IplImage* image2)
{
 	double weights[7][4]  =
    {
        { 1, 1, 1, 1},
        { 1,  2, 2, 1},
        { 1,   2,  2,  1},
        { 2,  4,  4, 2},
        { 2,  4,  4, 2},
        { 1,  2,  2, 1},
        { 1,  1,    1,   1 }
    };

    int m,n;
    double differenceValue=0;
    for (m=0;m<4;m++)
    {
        for (n=0;n<7;n++)
        {

            cvSetImageROI(image1,cvRect((30*m),(n*20),30,20));
            cvSetImageROI(image2,cvRect((30*m),(n*20),30,20));
            differenceValue+=weights[n][m]*hist_difference(image1,image2);
			cvResetImageROI(image1);
            cvResetImageROI(image2);
        }

    }
    return differenceValue;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Description:- does template matching b/w two images and return result
//                 by using CV_TM_SQDIFF method 
// Input :- 2 IplImage*
// Returns :- double (min value of difference)
// Called by :- double face_template_match( IplImage*first, IplImage*second)
///////////////////////////////////////////////////////////////////////////////
double template_match( IplImage*first_image, IplImage*second_image)
{
    IplImage* result = cvCreateImage( cvSize(1,1), IPL_DEPTH_32F, 1 );
    CvPoint    min_loc, max_loc;
    min_loc=cvPoint(0,0);
    max_loc=cvPoint(0,0);
    double		min, max;
    cvMatchTemplate( first_image, second_image, result, CV_TM_SQDIFF );
    cvMinMaxLoc( result, &min, &max, &min_loc, &max_loc, 0 );
    cvReleaseImage( &result);
    return min;

}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Description:- does template matching between two faces in 4 diffrent 
//                regions
// Input :- 2 IplImage*
// Returns :- double
// Called by :- 
///////////////////////////////////////////////////////////////////////////////
double face_template_match( IplImage*first, IplImage*second)
{

    IplImage* firstHalf1=cvCreateImage( cvSize(first->width/2,first->height), 8, first->nChannels );
    cvSetImageROI(first,cvRect(0,0,first->width/2,first->height));
    cvResize( first,firstHalf1, CV_INTER_LINEAR ) ;
    cvResetImageROI(first);

    IplImage* secondHalf1=cvCreateImage( cvSize(second->width/2,second->height), 8, first->nChannels );
    cvSetImageROI(second,cvRect(0,0,second->width/2,second->height));
    cvResize( second,secondHalf1, CV_INTER_LINEAR ) ;
    cvResetImageROI(second);

    IplImage* firstHalf2=cvCreateImage( cvSize(first->width/2,first->height), 8, first->nChannels );
    cvSetImageROI(first,cvRect(first->width/2,0,first->width/2,first->height));
    cvResize( first,firstHalf2, CV_INTER_LINEAR ) ;
    cvResetImageROI(first);

    IplImage* secondHalf2=cvCreateImage( cvSize(second->width/2,second->height), 8, first->nChannels );
    cvSetImageROI(second,cvRect(second->width/2,0,second->width/2,second->height));
    cvResize( second,secondHalf2, CV_INTER_LINEAR ) ;
    cvResetImageROI(second);

    double d1 =template_match(firstHalf1,secondHalf1);
    double d2 =template_match(firstHalf2,secondHalf2);
    double d=0;
    if (d1<d2)
        d=d1*2;
    else
        d=d2*2;

    cvReleaseImage( &firstHalf1);
    cvReleaseImage( &firstHalf2);
    cvReleaseImage( &secondHalf1);
    cvReleaseImage( &secondHalf2);

    IplImage* firstEye=cvCreateImage( cvSize(first->width,40), 8, first->nChannels );
    cvSetImageROI(first,cvRect(0,25,first->width,65));
    cvResize( first,firstEye, CV_INTER_LINEAR ) ;
    cvResetImageROI(first);

    IplImage* secondEye=cvCreateImage( cvSize(second->width,40), 8, first->nChannels );
    cvSetImageROI(second,cvRect(0,25,second->width,65));
    cvResize( second,secondEye, CV_INTER_LINEAR ) ;
    cvResetImageROI(second);

    d=d+(template_match(firstEye,secondEye)*2); // Eye area
    cvReleaseImage( &firstEye);
    cvReleaseImage( &secondEye);
    return d;

}
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////-----END---///////////////////////////////////////

