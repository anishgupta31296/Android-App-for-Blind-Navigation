package com.anrox.helloworld;

import android.content.Context;
import android.os.Vibrator;

public class Vib {
	public void vibrate(Context c, int i)
	{
		 Vibrator mVibrator = (Vibrator) c.getSystemService(Context.VIBRATOR_SERVICE);
	        mVibrator.vibrate(i);
	}
	void vibr()
	{
		Context c;
		c=MyApplication.getContext();
		vibrate(c,100);
	}
	void vibl()
	{
		Context c;
		c=MyApplication.getContext();
		vibrate(c,500);
	}
	
	

}
