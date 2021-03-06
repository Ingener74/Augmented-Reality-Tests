cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME glestools)
project(${PROJECT_NAME})

include_directories(
	include
	../libtools/include
	${INSTALL_3RDPARTY}/sdk/native/jni/include
	${INSTALL_3RDPARTY}/include
	${ANDROID_NDK}/sources/android/native_app_glue
	${ANDROID_NDK}/platforms/android-14/arch-arm/usr/include
	)
	
link_directories(
	${INSTALL_3RDPARTY}/sdk/native/libs/armeabi-v7a
	${INSTALL_3RDPARTY}/sdk/native/3rdparty/libs/armeabi-v7a
	)

file(GLOB SOURCES src/*.cpp)
	
set(OPENCV_LIBS
    opencv_highgui
    opencv_androidcamera
    opencv_imgproc
    opencv_core
    #tbb
    libpng
    libjpeg
    libtiff
    libjasper
    IlmImf
    )

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OPENCV_LIBS})
