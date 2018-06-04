LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
override OPENCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=off
OPENCV_LIB_TYPE:=STATIC
include F:\OpenCV-android-sdk\sdk\native\jni\OpenCV.mk
LOCAL_MODULE    := ImageProcessing
LOCAL_SRC_FILES := ImageProcessing.cpp
LOCAL_LDLIBS +=  -llog -ldl
include $(BUILD_SHARED_LIBRARY)
