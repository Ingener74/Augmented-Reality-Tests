/*
 * GraphicsService.h
 *
 *  Created on: 27.01.2013
 *      Author: pavel
 */

#ifndef GRAPHICSSERVICE_H_
#define GRAPHICSSERVICE_H_

#include <EGL/egl.h>
#include <android_native_app_glue.h>

#include "TimeService.h"
#include "Types.hpp"

namespace smallarsdk {

class GraphicsService {
public:
	GraphicsService( android_app* application, TimeService* timeService );

	const char* getPath();
	const int32_t& getWidth() const ;
	const int32_t& getHeight() const ;

	status start();
	void   stop();
	status update();

private:
	android_app* _application;
	TimeService* _timeService;

	int32_t _width, _height;
	EGLDisplay _display;
	EGLSurface _surface;
	EGLContext _context;

};

} /* namespace smallarsdk */
#endif /* GRAPHICSSERVICE_H_ */