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
#define MATRIX3_SIZE 3
typedef GLfloat Matrix3[MATRIX3_SIZE * MATRIX3_SIZE]; // A column major 3x3 matrix

float det3(Matrix3 matrix);
float det4(Matrix4 matrix);
void alliance4(Matrix4 matrix, int row, int col, Matrix3 res);
void find_alliance_matrix4(Matrix4 matrix, Matrix4 res);
void transpose4(Matrix4 matrix, Matrix4 res);
void inversion4(Matrix4 orig_matrix, Matrix4 result);
void print_matrix3(Matrix3 matrix);
void print_matrix4(Matrix4 matrix);

void vector3_copy(Vector3 dst, Vector3 src);
void vector3_add(Vector3 first, Vector3 second, Vector3 result);
void vector3_sub(Vector3 first, Vector3 second, Vector3 result);
void vector3_scale(Vector3 v, float s);
float vector3_dot(Vector3 first, Vector3 second);
void vector3_cross(Vector3 first, Vector3 second, Vector3 result);

void matrix4_copy(Matrix4 dst, Matrix4 src);
void loadIdentity(Matrix4 result);
void multMatrixByVector4(Matrix4 first, Vector4 second, Vector4 result);
void multMatrixByVector4_mut(Matrix4 first, Vector4 second);
void multMatrixByMatrix(Matrix4 first, Matrix4 second, Matrix4 result);
void multMatrixByMatrix_mut(Matrix4 first, Matrix4 second);

#endif // MMATH_H_
