/*
 * OpenGLES20GraphicService.cpp
 *
 *  Created on: 13.03.2013
 *      Author: pavel
 */

#include "OpenGLES20GraphicService.h"

#include <RGBTexture.h>
#include <GLTriangle.h>

#include <GLMatrix4x4.h>

#include <Mesh.h>

#include <OBJLoader.h>

const char vertexSource[] =
		"uniform   mat4  uortho;"
		"attribute vec4  vpos;"
//		"attribute vec2  atex;"
//		"varying   vec2  vtex;"
		"varying   vec4  vcolor;"
		"void main(){"
		"    gl_Position = uortho * vpos;"
//		"    vtex = atex;"
		"    vcolor = vpos / vec4(5.0, 5.0, 5.0, 1.0);"
		"}"
		;

//const char vertexSource[] =
//		"uniform   mat4  uortho;"
//		"attribute vec4  vpos;"
//		"void main(){"
//		"    gl_Position = uortho * vpos;"
//		"}"
//		;

//const char fragmentSource[] =
//		"precision mediump float;"
//		"varying vec2      vtex;"
//		"uniform sampler2D stex;"
//		"void main(){"
//		"    gl_FragColor = texture2D(stex, vtex);"
//		"}"
//		;

const char fragmentSource[] =
		"precision mediump float;"
		"varying vec4 vcolor;"
		"void main(){"
//		"    gl_FragColor = vec4(1.0, 1.0, 1.0, 0.5);"
		"    gl_FragColor = vcolor;"
		"}"
		;

const int32_t texSize = 1024;

GLMatrix4x4 projective;

OpenGLES20GraphicService::OpenGLES20GraphicService(): _isInit(false),
		_display(EGL_NO_DISPLAY),
		_context(EGL_NO_CONTEXT),
		_surface(EGL_NO_SURFACE),

		_width(0),
		_height(0),
		_program(0),
		_vpos(0),
		_atex(0),
		_stex(0),
		_uortho(0),

		_obj001(NULL),
		_obj002(NULL),
		_mt(NULL)
{
}

OpenGLES20GraphicService::~OpenGLES20GraphicService() {
}

OpenGLES20GraphicService::STATUS OpenGLES20GraphicService::init(
		android_app* application ){

	_application = application;

	const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RED_SIZE,   8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE,  8,
			EGL_DEPTH_SIZE, 8,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE, EGL_NONE };

	EGLint format, numConf;
	EGLConfig config;

	_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(_display, 0, 0);

	eglChooseConfig(_display, attribs, &config, 1, &numConf);

	eglGetConfigAttrib(_display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(application->window, 0, 0, format);

	_surface = eglCreateWindowSurface(_display, config, application->window, NULL);

	EGLint contextAttr[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
	};
	_context = eglCreateContext(_display, config, NULL, contextAttr);

	if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE){
		LOGE_OGLES20GS("Unable to eglMakeCurrent");
		return IGraphicsService::STATUS_ERROR;
	}
	eglQuerySurface(_display, _surface, EGL_WIDTH, &_width);
	eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

	const double dim = 200.0;
	const double aspect = _height / double(_width);
	glViewport(0, 0, _width, _height);
	LOGI_OGLES20GS("h = %d, w = %d", _height, _width);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	_program = createProgram(vertexSource, fragmentSource);
	if(!_program){
		LOGE_OGLES20GS("Creating program is failed!!!");
	}else{
		LOGI_OGLES20GS("vpos handle getting");
		_vpos = glGetAttribLocation(_program, "vpos");  Tools::glCheck("glGetAttribLocation");
//		_atex = glGetAttribLocation(_program, "atex");  Tools::glCheck("glGetAttribLocation");
//		_stex = glGetUniformLocation(_program, "stex"); Tools::glCheck("glGetUniformLocation");
		_uortho = glGetUniformLocation(_program, "uortho"); Tools::glCheck("glGetUniformLocation");
	}

	Mat im(texSize, texSize, CV_8UC3, Scalar(0, 100, 70));
	circle(im, Point(100, 100), 60, Scalar(170, 0,0), 7);

	if( EGLDispatcher::init() ){
		_mt = new FastEGLTexture(im.data, im.rows, im.cols,
				FastEGLTexture::IMAGE_FORMAT_RGB888, _application, _display,
				FastEGLTexture::FEGL_TEXTURE_FORMAT_RGBA8888);
	}

	projective = GLMatrix4x4(0.1, 10000, 54 * 3.1415926 / 180, _width / GLfloat(_height) );

//	OBJLoader load_obj001("/sdcard/repo/data/my_mesh.obj");
//	MeshV mv001;
//	if(load_obj001.getNumOfLoadedObjects()){
//		mv001.createMesh(load_obj001.getObject(0));
//	}
//	_obj001 = new Mesh(mv001, _vpos);

//	"/sdcard/repo/data/my_mesh.obj"
//	"/sdcard/repo/data/mesh002_cube.obj"
	OBJLoader obj002("/sdcard/repo/data/my_mesh.obj");
	MeshV mv002;
	if(obj002.getNumOfLoadedObjects()){
		mv002.createMesh(obj002.getObject(0));
	}
	_obj002 = new Mesh(mv002, _vpos);

	_isInit = true;
	return STATUS_OK;
}
bool OpenGLES20GraphicService::isInit(){
	return _isInit;
}

void OpenGLES20GraphicService::deinit(){

	if(_display != EGL_NO_DISPLAY){
		eglMakeCurrent(_display, _surface, _surface, _context);
		if(_context != EGL_NO_CONTEXT){
			eglDestroyContext(_display, _context);
		}
		if(_surface != EGL_NO_SURFACE){
			eglDestroySurface(_display, _surface);
		}
		eglTerminate(_display);
	}
	_display = EGL_NO_DISPLAY;
	_context = EGL_NO_CONTEXT;
	_surface = EGL_NO_SURFACE;
}



void OpenGLES20GraphicService::draw(){
	glClearColor(0.5f, 0.9f, 0.5f, 1.f); Tools::glCheck("glClearColor");
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); Tools::glCheck("glClear");

	glUseProgram(_program); Tools::glCheck("glUseProgram");
//
//	glActiveTexture(GL_TEXTURE0);
//	_tex1->bind();
//	glUniform1i(_stex, 0);

//	if(_obj001)
//		_obj001->draw();

	static GLfloat ax = 0.0f, az = 0.0f;
	az += 0.05;
	ax += 0.01;

	glUniformMatrix4fv(_uortho, 1, false, (projective * GLMatrix4x4().rotateX(ax).position(0,0,-12)).getMatrix() );

	if(_obj002)
		_obj002->draw();


	eglSwapBuffers(_display, _surface);

}

void OpenGLES20GraphicService::setImage( Mat image ){
}

GLuint OpenGLES20GraphicService::loadShader( GLenum shaderType, const char* source ){
	GLuint shader = glCreateShader(shaderType);
	if(shader){
		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if(!compiled){
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if(infoLen){
				char* buf = new char[infoLen];
				if(buf){
					glGetShaderInfoLog(shader, infoLen, 0, buf);
					LOGE("loadShader", "Could not compile shader: %d,\n%s", shaderType, buf);
					delete buf;
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

GLuint OpenGLES20GraphicService::createProgram( const char* vertexShader, const char* fragmentShader ){
	GLuint vs = loadShader(GL_VERTEX_SHADER, vertexShader);
	if(!vs){
		return 0;
	}
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, fragmentShader);
	if(!fs){
		return 0;
	}

	GLuint program = glCreateProgram();
	if(program){
		glAttachShader(program, vs); GLERR;
		glAttachShader(program, fs); GLERR;

		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if(linkStatus != GL_TRUE){
			GLint bufLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLen);
			if(bufLen){
				char* buf = (char*)new char[bufLen];
				if(buf){
					glGetProgramInfoLog(program, bufLen, NULL, buf);
					LOGE_OGLES20GS("Could not link program");
					delete [] buf;
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}