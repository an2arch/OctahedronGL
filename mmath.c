#include "mmath.h"
#include <assert.h>
#include <math.h>

void loadIdentity(Matrix4 result) {
    assert(result);
    memset(result, 0, MATRIX4_SIZE * MATRIX4_SIZE * sizeof(result[0]));
    result[0 + 0 * MATRIX4_SIZE] = 1.0f;
    result[1 + 1 * MATRIX4_SIZE] = 1.0f;
    result[2 + 2 * MATRIX4_SIZE] = 1.0f;
    result[3 + 3 * MATRIX4_SIZE] = 1.0f;
}

void multMatrixByVector4(Matrix4 first, Vector4 second, Vector4 result) {
    assert(first && second && result && MATRIX4_SIZE == VECTOR4_COMPS);
    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        result[i] = 0.0f;
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            result[i] += first[i + j * MATRIX4_SIZE] * second[j];
        }
    }
}

void multMatrixByVector4_mut(Matrix4 first, Vector4 second) {
    assert(first && second && MATRIX4_SIZE == VECTOR4_COMPS);

    Vector4 tmp = {};
    memcpy(tmp, second, VECTOR4_COMPS * sizeof(second[0]));

    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        second[i] = 0.0f;
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            second[i] += first[i + j * MATRIX4_SIZE] * tmp[j];
        }
    }
}

void multMatrixByMatrix(Matrix4 first, Matrix4 second, Matrix4 result) {
    assert(first && second && result);
    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            result[i + j * MATRIX4_SIZE] = 0;
            for (size_t k = 0; k < MATRIX4_SIZE; ++k) {
                result[i + j * MATRIX4_SIZE] +=
                    first[i + k * MATRIX4_SIZE] * second[k + j * MATRIX4_SIZE];
            }
        }
    }
}

void multMatrixByMatrix_mut(Matrix4 first, Matrix4 second) {
    assert(first && second);

    Matrix4 tmp = {};
    memcpy(tmp, second, MATRIX4_SIZE * MATRIX4_SIZE * sizeof(second[0]));

    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            second[i + j * MATRIX4_SIZE] = 0;
            for (size_t k = 0; k < MATRIX4_SIZE; ++k) {
                second[i + j * MATRIX4_SIZE] +=
                    first[i + k * MATRIX4_SIZE] * tmp[k + j * MATRIX4_SIZE];
            }
        }
    }
}

void loadScale(float x, float y, float z, Matrix4 result) {
    assert(result);
    loadIdentity(result);
    result[0 + 0 * MATRIX4_SIZE] = x;
    result[1 + 1 * MATRIX4_SIZE] = y;
    result[2 + 2 * MATRIX4_SIZE] = z;
}

void loadTranslate(float x, float y, float z, Matrix4 result) {
    assert(result);
    loadIdentity(result);
    result[1 + 3 * MATRIX4_SIZE] = x;
    result[1 + 3 * MATRIX4_SIZE] = y;
    result[2 + 3 * MATRIX4_SIZE] = z;
}

// https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
void loadRotationX(float angle, Matrix4 result) {
    assert(result);
    loadIdentity(result);
    result[1 + 1 * MATRIX4_SIZE] = cosf(angle);
    result[1 + 2 * MATRIX4_SIZE] = -sinf(angle);
    result[2 + 1 * MATRIX4_SIZE] = sinf(angle);
    result[2 + 2 * MATRIX4_SIZE] = cosf(angle);
}

void loadRotationY(float angle, Matrix4 result) {
    assert(result);
    loadIdentity(result);
    result[0 + 0 * MATRIX4_SIZE] = cosf(angle);
    result[0 + 2 * MATRIX4_SIZE] = sinf(angle);
    result[2 + 0 * MATRIX4_SIZE] = -sinf(angle);
    result[2 + 2 * MATRIX4_SIZE] = cosf(angle);
}

void loadRotationZ(float angle, Matrix4 result) {
    assert(result);
    loadIdentity(result);
    result[0 + 0 * MATRIX4_SIZE] = cosf(angle);
    result[0 + 1 * MATRIX4_SIZE] = -sinf(angle);
    result[1 + 0 * MATRIX4_SIZE] = sinf(angle);
    result[1 + 1 * MATRIX4_SIZE] = cosf(angle);
}
