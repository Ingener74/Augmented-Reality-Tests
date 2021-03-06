cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME 05_ocv_gles20_app)
project(${PROJECT_NAME})

include_directories(
    ../../modules/libtools/include
    ../../modules/libglestool/include
    ../../modules/libaruco/include
    )
include_directories(
	${INSTALL_3RDPARTY}/sdk/native/jni/include
        ${INSTALL_3RDPARTY}/include
	${ANDROID_NDK}/sources/android/native_app_glue
       	${ANDROID_NDK}/platforms/android-14/arch-arm/usr/include
	)
link_directories(
       	${INSTALL_3RDPARTY}/sdk/native/libs/armeabi-v7a
       	${INSTALL_3RDPARTY}/sdk/native/3rdparty/libs/armeabi-v7a
	${ANDROID_NDK}/platforms/android-14/arch-arm/usr/lib
       	)

set(ANDROID_LIBS 
    log 
    android 
    EGL 
    GLESv1_CM
    GLESv2
    OpenSLES
    OpenMAXAL
    z        
    )
set(OPENCV_LIBS 
    opencv_highgui
    opencv_androidcamera
    opencv_imgproc
    opencv_core
    
    opencv_calib3d
    opencv_objdetect
    opencv_features2d
    opencv_flann
    
    opencv_java

    #tbb
    libpng
    libjpeg
    libtiff
    libjasper
    IlmImf
    )
    
file(GLOB SRC src/*.cpp)
set(SOURCES ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c  ${SRC})

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ANDROID_LIBS} ${OPENCV_LIBS} tools glestools aruco)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ANAT_INSTALL_APPS}/${PROJECT_NAME}/libs/armeabi-v7a)
