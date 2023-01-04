#include "mmath.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>

#define FPS 60.0f

#define GL_PI 3.141592653589

#define ESC_KEY 0x1B

static float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;
static float dxRot = 0.0f, dyRot = 0.0f, dzRot = 0.0f;

static Matrix4 camera_matrix;
static Matrix4 inv_camera;
static Matrix4 projection_matrix;

#define VERTS_PER_PLANE 3
#define OCT_PLANES 8

static const float r = 1.0f;
Vector3 octahedron_mesh[OCT_PLANES][VERTS_PER_PLANE] = {
    {{0.0f, r, 0.0f}, {r, 0.0f, 0.0f}, {0.0f, 0.0f, -r}},
    {{0.0f, r, 0.0f}, {0.0f, 0.0f, -r}, {-r, 0.0f, 0.0f}},
    {{0.0f, r, 0.0f}, {-r, 0.0f, 0.0f}, {0.0f, 0.0f, r}},
    {{0.0f, r, 0.0f}, {0.0f, 0.0f, r}, {r, 0.0f, 0.0f}},

    {{0.0f, -r, 0.0f}, {r, 0.0f, 0.0f}, {0.0f, 0.0f, r}},
    {{0.0f, -r, 0.0f}, {0.0f, 0.0f, -r}, {r, 0.0f, 0.0f}},
    {{0.0f, -r, 0.0f}, {-r, 0.0f, 0.0f}, {0.0f, 0.0f, -r}},
    {{0.0f, -r, 0.0f}, {0.0f, 0.0f, r}, {-r, 0.0f, 0.0f}},
};

static float bodyMatrix[VECTOR4_COMPS][OCT_PLANES];
static float transformBody[VECTOR4_COMPS][OCT_PLANES];

void normalKeysDown(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    switch (key) {
    case ESC_KEY:
        glutDestroyWindow(glutGetWindow());
        break;
    case 'w':
        dxRot = -1.0f;
        break;
    case 's':
        dxRot = 1.0f;
        break;
    case 'q':
        dyRot = -1.0f;
        break;
    case 'e':
        dyRot = 1.0f;
        break;
    case 'a':
        dzRot = 1.0f;
        break;
    case 'd':
        dzRot = -1.0f;
        break;
    case 'b':
        printf("Body matrix:\n");
        for (size_t i = 0; i < VECTOR4_COMPS; ++i) {
            for (size_t j = 0; j < OCT_PLANES; ++j) {
                printf("%f ", bodyMatrix[j][i]);
            }
            printf("\n");
        }
        printf("\n");
        break;
    case 't':
        printf("Transformed body matrix:\n");
        for (size_t i = 0; i < VECTOR4_COMPS; ++i) {
            for (size_t j = 0; j < OCT_PLANES; ++j) {
                printf("%f ", transformBody[j][i]);
            }
            printf("\n");
        }
        printf("\n");
        break;
    case 'c':
        printf("Camera matrix:\n");
        print_matrix4(camera_matrix);
        printf("\n");
        break;
    case 'i':
        printf("Inversion of camera matrix:\n");
        print_matrix4(inv_camera);
        printf("\n");
        break;

    default:
        break;
    }
}

void normalKeysUp(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    switch (key) {
    case 'w':
    case 's':
        dxRot = 0.0f;
        break;
    case 'q':
    case 'e':
        dyRot = 0.0f;
        break;
    case 'a':
    case 'd':
        dzRot = 0.0f;
        break;
    default:
        break;
    }
}

void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
        float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;
}

//matrix will receive the calculated perspective matrix.
//You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
        float znear, float zfar)
{
    float ymax, xmax;
    ymax = znear * tanf(fovyInDegrees * GL_PI / 360.0);
    //ymin = -ymax;
    //xmin = -ymax * aspectRatio;
    xmax = ymax * aspectRatio;
    glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void reshape(int w, int h) {
    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float fAspect = (float) w / (float) h;

    // gluPerspective(60.0f, fAspect, 0.01f, 100.0f);

    float matrix[16] = {0};
    glGetFloatv(GL_PROJECTION_MATRIX, matrix);
    glhPerspectivef2(matrix, 60.0f, fAspect, 0.01f, 100.0f);
    glLoadMatrixf(matrix);

    /*
    float range = 5.0f;
    if (w <= h) {
        glOrtho(-range * fAspect, range * fAspect, -range, range, range, -range);
    } else {
        glOrtho(-range, range, -range / fAspect, range / fAspect, range, -range);
    }
    */

    glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update() {
    if (dxRot || dyRot || dzRot) {
        xRot += dxRot;
        yRot += dyRot;
        zRot += dzRot;
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(0.0f, -0.05f, -2.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, camera_matrix);

    multMatrixByMatrix_mut(projection_matrix, camera_matrix);

    inversion4(camera_matrix, inv_camera);

    for (size_t i = 0; i < OCT_PLANES; ++i) {
        Vector4 tmp;
        for (size_t j = 0; j < VECTOR4_COMPS; ++j) {
            tmp[j] = bodyMatrix[j][i];
        }
        multMatrixByVector4_mut(inv_camera, tmp);
        for (size_t j = 0; j < VECTOR4_COMPS; ++j) {
            transformBody[j][i] = tmp[j];
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2);
    glLineStipple(5, 1);

    for (size_t plane = 0; plane < OCT_PLANES; ++plane) {
        if (plane < 4) {
            glColor3f(0.0f, 1.0f, 0.0f);
        } else {
            glColor3f(1.0f, 0.0f, 0.0f);
        }

        if (transformBody[2][plane] < 0.0f) {
            glDisable(GL_LINE_STIPPLE);
        } else {
            glEnable(GL_LINE_STIPPLE);
        }

        glBegin(GL_LINE_LOOP);
        for (size_t i = 0; i < VERTS_PER_PLANE; ++i) {
            glVertex3fv(octahedron_mesh[plane][i]);
        }
        glEnd();
    }
    glPopMatrix();

    glutSwapBuffers();
}

void timer(int value) {
    (void) value;
    update();
    glutTimerFunc(1000.0f / FPS, timer, 1);
    if (glutGetWindow()) {
        glutPostRedisplay();
    }
}

void setup() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Vector3 f, s, n;
    Vector3 p1, p2, p3;
    for (size_t plane = 0; plane < OCT_PLANES; ++plane) {
        vector3_copy(p1, octahedron_mesh[plane][0]);
        vector3_copy(p2, octahedron_mesh[plane][1]);
        vector3_copy(p3, octahedron_mesh[plane][2]);

        vector3_sub(p2, p1, f);
        vector3_sub(p3, p1, s);
        vector3_cross(f, s, n);

        float d = -p1[0] * n[0] - p1[1] * n[1] - p1[2] * n[2];

        if (d > 0) {
            vector3_scale(n, -1);
            d *= -1;
        }

        bodyMatrix[0][plane] = n[0];
        bodyMatrix[1][plane] = n[1];
        bodyMatrix[2][plane] = n[2];
        bodyMatrix[3][plane] = d;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Culling");

    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(normalKeysDown);
    glutKeyboardUpFunc(normalKeysUp);

    glutReshapeFunc(reshape);
    glutDisplayFunc(render);

    glutTimerFunc(1000.0f / FPS, timer, 1);
    setup();
    glutMainLoop();
    return 0;
}
