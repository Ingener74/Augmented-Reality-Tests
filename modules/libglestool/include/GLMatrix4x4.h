/*
 * GLMatrix4x4.h
 *
 *  Created on: 15.03.2013
 *      Author: pavel
 */

#ifndef GLMATRIX4X4_H_
#define GLMATRIX4X4_H_

#include <GLES2/gl2.h>
#include <math.h>

class GLMatrix4x4 {
public:
	GLMatrix4x4();
	virtual ~GLMatrix4x4();

	static void matrixIdentity( GLfloat* matrix );

	static void matrixPosition( GLfloat* matrix, GLfloat x, GLfloat y, GLfloat z );
	static void matrixMove( GLfloat* matrix, GLfloat dx, GLfloat dy, GLfloat dz );

	static void matrixScale( GLfloat* matrix, GLfloat x, GLfloat y, GLfloat z);
	static void matrixResize( GLfloat* matrix, GLfloat x, GLfloat y, GLfloat z);

	static void matrixRotationX( GLfloat* matrix, GLfloat angleRad );
	static void matrixRotationY( GLfloat* matrix, GLfloat angleRad );
	static void matrixRotationZ( GLfloat* matrix, GLfloat angleRad );

	static void matrixMultyply( GLfloat* matrix1, GLfloat* matrix2, GLfloat* result );

	static void matrixOrtho( GLfloat* matrix, GLfloat left, GLfloat right,
			GLfloat top, GLfloat bottom, GLfloat near, GLfloat far );

	static void matrixProjection( GLfloat* matrix, GLfloat near, GLfloat far,
			GLfloat angleOfView, GLfloat aspectRatio );
};

class ModelViewMatrix{
public:
	ModelViewMatrix();
	virtual ~ModelViewMatrix(){}

	void position(GLfloat x, GLfloat y, GLfloat z);
	void move(GLfloat dx, GLfloat dy, GLfloat dz);

	void scale(GLfloat x, GLfloat y, GLfloat z);
	void resize(GLfloat dx, GLfloat dy, GLfloat dz);

	void rotateX(GLfloat rad);
	void rotateY(GLfloat rad);
	void rotateZ(GLfloat rad);

	void operator*( const GLfloat* m1 );

	GLfloat* getMatrix(){ return _m; }

private:
	GLfloat _m[16];
};

#endif /* GLMATRIX4X4_H_ */
