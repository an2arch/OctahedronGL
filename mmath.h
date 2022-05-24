#ifndef MMATH_H_
#define MMATH_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159265358979323846
#define PI_DIV_180 0.017453292519943296
#define INV_PI_DIV_180 57.2957795130823229

#define degToRad(x) ((x) * (PI_DIV_180))
#define radToDeg(x) ((x) * (INV_PI_DIV_180))

#define VECTOR3_COMPS 3
typedef GLfloat Vector3[VECTOR3_COMPS];

#define VECTOR4_COMPS 4
typedef GLfloat Vector4[VECTOR4_COMPS];

#define MATRIX4_SIZE 4
typedef GLfloat Matrix4[MATRIX4_SIZE * MATRIX4_SIZE]; // A column major 4x4 matrix

void loadIdentity(Matrix4 result);
void multMatrixByVector4(Matrix4 first, Vector4 second, Vector4 result);
void multMatrixByVector4_mut(Matrix4 first, Vector4 second);
void multMatrixByMatrix(Matrix4 first, Matrix4 second, Matrix4 result);
void multMatrixByMatrix_mut(Matrix4 first, Matrix4 second);
void loadScale(float x, float y, float z, Matrix4 result);
void loadTranslate(float x, float y, float z, Matrix4 result);
void loadRotationX(float angle, Matrix4 result);
void loadRotationY(float angle, Matrix4 result);
void loadRotationZ(float angle, Matrix4 result);
void loadPerspective(float fovy, float aspect, float zNear, float zFar, Matrix4 result);

#endif // MMATH_H_
