package com.anrox.helloworld;

import java.io.IOException;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.os.Handler;
import android.os.Looper;
import android.os.Vibrator;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.ImageView;
import android.content.Context;
import android.os.Build;

public class CameraPreview extends MainActivity implements Camera.PreviewCallback, SurfaceHolder.Callback
{
	private Camera mCamera = null;
	private ImageView MyCameraPreview = null;
	private Bitmap bitmap = null;
	private int[] pixels = null;
	private byte[] FrameData = null;
	private int imageFormat;
	private int PreviewSizeWidth;
 	private int PreviewSizeHeight;
 	private boolean bProcessing = false;
 	private int[] a = null;
	
	
 	Handler mHandler = new Handler(Looper.getMainLooper());
 	
	public CameraPreview(int PreviewlayoutWidth, int PreviewlayoutHeight,
    		ImageView CameraPreview)
    {	 
		PreviewSizeWidth = PreviewlayoutWidth;
    	PreviewSizeHeight = PreviewlayoutHeight;
    	MyCameraPreview = CameraPreview;
    	bitmap = Bitmap.createBitmap(PreviewSizeWidth, PreviewSizeHeight, Bitmap.Config.ARGB_8888);
    	pixels = new int[PreviewSizeWidth * PreviewSizeHeight];
    	a = new int[2];
    }

	@Override
	public void onPreviewFrame(byte[] arg0, Camera arg1) 
	{	
		// At preview mode, the frame data will push to here.
		if (imageFormat == ImageFormat.NV21)
        {
			//We only accept the NV21(YUV420) format.
			if ( !bProcessing )
			{
				FrameData = arg0;
				mHandler.post(DoImageProcessing);
			}
        }
	}

	public void onPause()
    {
    	mCamera.stopPreview();
    }

	@Override
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) 
	{
	    Parameters parameters;
		
	    parameters = mCamera.getParameters();
		// Set the camera preview size
		parameters.setPreviewSize(PreviewSizeWidth, PreviewSizeHeight);
		
		imageFormat = parameters.getPreviewFormat();
		
		mCamera.setParameters(parameters);
		
		mCamera.startPreview();
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) 
	{
		mCamera = Camera.open();
		try
		{
			// If did not set the SurfaceHolder, the preview area will be black.
			mCamera.setPreviewDisplay(arg0);
			mCamera.setPreviewCallback(this);
		} 
		catch (IOException e)
		{
			mCamera.release();
			mCamera = null;
		}
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) 
	{
    	mCamera.setPreviewCallback(null);
		mCamera.stopPreview();
		mCamera.release();
		mCamera = null;
	}

	//
	// Native JNI 
	//
	public native boolean ImageProcessing(int width, int height, 
    		byte[] NV21FrameData, int [] pixels, int [] p);
    static 
    {
    	System.loadLibrary("ImageProcessing");
    }
    
    private Runnable DoImageProcessing = new Runnable() 
    {
        public void run() 
        {	a[0]=0;
        	a[1]=0;
        	Log.i("MyRealTimeImageProcessing", "DoImageProcessing():");
        	bProcessing = true;
			ImageProcessing(PreviewSizeWidth, PreviewSizeHeight, FrameData, pixels,a);
			if(a[1]==1)
			{
				Vib v1=new Vib();
				v1.vibl();
			}
			else if(a[0]==1)
			{
				Vib v=new Vib();
		        v.vibr();
			}
			else 
				;
			bitmap.setPixels(pixels, 0, PreviewSizeWidth, 0, 0, PreviewSizeWidth, PreviewSizeHeight);
			MyCameraPreview.setImageBitmap(bitmap);
			bProcessing = false;
        }
    };
   }
