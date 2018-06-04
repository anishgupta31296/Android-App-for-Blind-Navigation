/*
*  ImageProcessing.cpp
*/
#include <jni.h>
#include <stddef.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;

Mat * mCanny = NULL;

extern "C"
jboolean
Java_com_anrox_helloworld_CameraPreview_ImageProcessing(
		JNIEnv* env, jobject thiz,
		jint width, jint height,
		jbyteArray NV21FrameData, jintArray outPixels, jintArray count)
{
	jbyte * pNV21FrameData = env->GetByteArrayElements(NV21FrameData, 0);
	jint * poutPixels = env->GetIntArrayElements(outPixels, 0);
	jint * pcount = env->GetIntArrayElements(count, 0);


	if ( mCanny == NULL )
	{
		mCanny = new Mat(height, width, CV_8UC1);
	}

	Mat mGray(height, width, CV_8UC1, (unsigned char *)pNV21FrameData);
	Mat mResult(height, width, CV_8UC4, (unsigned char *)poutPixels);
	Mat Canny;
	IplImage srcImg = mGray;
	IplImage CannyImg = *mCanny;
	IplImage ResultImg = mResult;
	IplConvKernel *element1,*element2 ;
//	element1 = cvCreateStructuringElementEx (3, 3, -1, -1, CV_SHAPE_RECT, NULL);
	//element2 = cvCreateStructuringElementEx (8, 8, -1,-1, CV_SHAPE_RECT, NULL);
		/*	cvErode(&srcImg,&CannyImg,element1);
		cvErode(&srcImg,&CannyImg,element1);
		cvDilate(&CannyImg,&ResultImg,element2);
		cvDilate(&CannyImg,&ResultImg,element2);*/
	cvErode(&srcImg,&srcImg,NULL,3);
		cvDilate(&srcImg,&srcImg,NULL,7);
		CvMemStorage *storage = cvCreateMemStorage (0);
		  CvSeq *contours = 0;

	cvCanny(&srcImg, &CannyImg, 80, 100, 3);
	cvFindContours (&CannyImg, storage, &contours, sizeof (CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	cvCvtColor(&CannyImg, &ResultImg, CV_GRAY2BGRA);
	//cvDrawContours (&ResultImg, contours, CV_RGB (255, 0, 0), CV_RGB (0, 255, 0), 0, 2, CV_AA, cvPoint (0, 0));
	int a=contours->total;
	if(a>1)
	{
	char b[5];
	//itoa(a, b, 10);
	char text = 'World';

	char buffer[25];

/*	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1, 1);*/

	CvSeq* current_contour = contours;
	double longest=0;
	double largestArea = 0;
	double area1;
	CvSeq* longest_contour = NULL;
	CvSeq* largest_contour = NULL;
	while (current_contour != NULL){
		double area = fabs(cvContourArea(current_contour,CV_WHOLE_SEQ, 0));
		if(area > largestArea){
	        largestArea = area;
	        largest_contour = current_contour;
	    }
		double length = fabs(cvArcLength(current_contour,CV_WHOLE_SEQ, 0));
				if(length > longest && area<10){
			        longest = length;
			        area1=area;
			        longest_contour = current_contour;
			    }
	    current_contour = current_contour->h_next;
	}
	CvRect rect;
	rect = cvBoundingRect (longest_contour, 0);
	if((rect.width)>(7*rect.height))
	cvRectangle (&ResultImg, cvPoint (rect.x, rect.y),
	               cvPoint (rect.x + rect.width, rect.y + rect.height), CV_RGB (255, 0, 0), 2);
	sprintf(buffer, "Hello  %d", a);
	if(longest>500&&(rect.width)>(7*rect.height))
		pcount[1]=1;
	if(largestArea>300)
	pcount[0]=1;
	//if()
	//pcount[1]=1;

//	jclass cls2 = env->FindClass("com/anrox/helloworld/Vib");
/*	jmethodID show = env->GetMethodID(cls2, "vibr", "()V");
	if(show==0)
		cvPutText(&ResultImg, "NULL", cvPoint(300, 300), &font, cvScalar(255));
	else
		cvPutText(&ResultImg, "NOT NULL", cvPoint(300, 300), &font, cvScalar(255));
	env->CallVoidMethod(cls2, show);*/
	//cvPutText(&ResultImg, buffer, cvPoint(300, 300), &font, cvScalar(255));
	}

	env->ReleaseByteArrayElements(NV21FrameData, pNV21FrameData, 0);
	env->ReleaseIntArrayElements(outPixels, poutPixels, 0);
	env->ReleaseIntArrayElements(count, pcount, 0);
	return true;
}
