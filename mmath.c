#include "mmath.h"
#include <assert.h>
#include <math.h>

void vector3_copy(Vector3 dst, Vector3 src) {
    memcpy(dst, src, sizeof(src[0]) * VECTOR3_COMPS);
}

void vector3_add(Vector3 first, Vector3 second, Vector3 result) {
    result[0] = first[0] + second[0];
    result[1] = first[1] + second[1];
    result[2] = first[2] + second[2];
}

void vector3_sub(Vector3 first, Vector3 second, Vector3 result) {
    result[0] = first[0] - second[0];
    result[1] = first[1] - second[1];
    result[2] = first[2] - second[2];
}

void vector3_scale(Vector3 v, float s) {
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
}

float vector3_len_sqr(Vector3 v) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

float vector3_len(Vector3 v) {
    return sqrtf(vector3_len_sqr(v));
}

void vector3_norm(Vector3 v) {
    float len = vector3_len(v);
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

float vector3_dot(Vector3 first, Vector3 second) {
    return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
}

float vector4_dot(Vector3 first, Vector3 second) {
    return first[0] * second[0] + first[1] * second[1] + first[2] * second[2] +
           first[3] * second[3];
}

void vector3_cross(Vector3 first, Vector3 second, Vector3 result) {
    result[0] = first[1] * second[2] - first[2] * second[1];
    result[1] = first[2] * second[0] - first[0] * second[2];
    result[2] = first[0] * second[1] - first[1] * second[0];
}

void print_matrix3(Matrix3 matrix) {
    for (size_t i = 0; i < MATRIX3_SIZE; ++i) {
        for (size_t j = 0; j < MATRIX3_SIZE; ++j) {
            printf("%f ", matrix[i + j * MATRIX3_SIZE]);
        }
        printf("\n");
    }
}

void print_matrix4(Matrix4 matrix) {
    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            printf("%f ", matrix[i + j * MATRIX4_SIZE]);
        }
        printf("\n");
    }
}

void matrix4_copy(Matrix4 dst, Matrix4 src) {
    assert(dst && src);
    memcpy(dst, src, sizeof(src[0]) * MATRIX4_SIZE * MATRIX4_SIZE);
}

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

    Vector4 tmp;
    memcpy(tmp, second, VECTOR4_COMPS * sizeof(second[0]));

    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        second[i] = 0.0f;
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            second[i] += first[j + i * MATRIX4_SIZE] * tmp[j];
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

    Matrix4 tmp;
    memcpy(tmp, second, MATRIX4_SIZE * MATRIX4_SIZE * sizeof(second[0]));

    multMatrixByMatrix(first, tmp, second);
}

void transpose4(Matrix4 matrix, Matrix4 res) {
    assert(matrix && res);
    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            res[j + i * MATRIX4_SIZE] = matrix[i + j * MATRIX4_SIZE];
        }
    }
}

float det3(Matrix3 m) {
    size_t tl = 0 + 0 * MATRIX3_SIZE;
    size_t t = 0 + 1 * MATRIX3_SIZE;
    size_t tr = 0 + 2 * MATRIX3_SIZE;
    size_t l = 1 + 0 * MATRIX3_SIZE;
    size_t c = 1 + 1 * MATRIX3_SIZE;
    size_t r = 1 + 2 * MATRIX3_SIZE;
    size_t bl = 2 + 0 * MATRIX3_SIZE;
    size_t b = 2 + 1 * MATRIX3_SIZE;
    size_t br = 2 + 2 * MATRIX3_SIZE;
    return m[tl] * m[c] * m[br] - m[tl] * m[b] * m[r] + m[t] * m[r] * m[bl] - m[t] * m[br] * m[l] +
           m[tr] * m[l] * m[b] - m[tr] * m[bl] * m[c];
}

void alliance4(Matrix4 matrix, int row, int col, Matrix3 res) {
    assert(row < MATRIX4_SIZE && col < MATRIX4_SIZE);

    int di = 0, dj = 0;
    for (int i = 0; i < MATRIX4_SIZE; ++i) {
        if (i == row) {
            di += 1;
            continue;
        }
        dj = 0;
        for (int j = 0; j < MATRIX4_SIZE; ++j) {
            if (j == col) {
                dj += 1;
                continue;
            }
            assert((i - di) < MATRIX3_SIZE && (j - dj) < MATRIX3_SIZE);
            res[(i - di) + (j - dj) * MATRIX3_SIZE] = matrix[i + j * MATRIX4_SIZE];
        }
    }
}

float det4(Matrix4 matrix) {
    assert(matrix);
    Matrix3 first, second, third, fourth;

    alliance4(matrix, 0, 0, first);
    alliance4(matrix, 0, 1, second);
    alliance4(matrix, 0, 2, third);
    alliance4(matrix, 0, 3, fourth);

    return matrix[0] * det3(first) - matrix[MATRIX4_SIZE] * det3(second) +
           matrix[MATRIX4_SIZE * 2] * det3(third) - matrix[MATRIX4_SIZE * 3] * det3(fourth);
}

void find_alliance_matrix4(Matrix4 matrix, Matrix4 res) {
    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            Matrix3 alliance;
            alliance4(matrix, i, j, alliance);
            res[i + j * MATRIX4_SIZE] = ((i + j) % 2 == 0 ? 1 : -1) * det3(alliance);
        }
    }
}

void inversion4(Matrix4 orig_matrix, Matrix4 result) {
    float det = det4(orig_matrix);
    assert(det);

    Matrix4 transposed, alliance;
    transpose4(orig_matrix, transposed);
    find_alliance_matrix4(transposed, alliance);
    for (size_t i = 0; i < MATRIX4_SIZE; ++i) {
        for (size_t j = 0; j < MATRIX4_SIZE; ++j) {
            result[i + j * MATRIX4_SIZE] = alliance[i + j * MATRIX4_SIZE] / det;
        }
    }
}
