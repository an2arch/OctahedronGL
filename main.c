#include "mmath.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>

#define GL_PI 3.141592653589

static float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;
static float dxRot = 0.0f, dyRot = 0.0f, dzRot = 0.0f;

#define VERTS_PER_PLANE 3
#define OCT_PLANES 8

static const float r = 1.0f;
Vector3 octahedron_mesh[OCT_PLANES][VERTS_PER_PLANE] = {
    {{0.0f, 0.0f, r}, {r, 0.0f, 0.0f}, {0.0f, r, 0.0f}},
    {{0.0f, 0.0f, r}, {0.0f, r, 0.0f}, {-r, 0.0f, 0.0f}},
    {{0.0f, 0.0f, r}, {-r, 0.0f, 0.0f}, {0.0f, -r, 0.0f}},
    {{0.0f, 0.0f, r}, {0.0f, -r, 0.0f}, {r, 0.0f, 0.0f}},
    {{0.0f, 0.0f, -r}, {r, 0.0f, 0.0f}, {0.0f, r, 0.0f}},
    {{0.0f, 0.0f, -r}, {0.0f, r, 0.0f}, {-r, 0.0f, 0.0f}},
    {{0.0f, 0.0f, -r}, {-r, 0.0f, 0.0f}, {0.0f, -r, 0.0f}},
    {{0.0f, 0.0f, -r}, {0.0f, -r, 0.0f}, {r, 0.0f, 0.0f}}};

void normalKeysDown(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    switch (key) {
    case 0x1B: // ESC
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

void reshape(int w, int h) {
    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Produce the perspective projection
    float fAspect = (float) w / (float) h;
    gluPerspective(60.0f, fAspect, 0.0f, 100.0f);
    // glOrtho(-20.0f, 20.0f, -20.0f, 20.0f, 0.0f, -100.0f);
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

    update();
    glPushMatrix();

    Matrix4 modelMatrix;
    loadIdentity(modelMatrix);

    Matrix4 tmp;
    loadIdentity(tmp);

    loadScale(10.0f, 10.0f, 10.0f, tmp);
    multMatrixByMatrix_mut(tmp, modelMatrix);

    loadRotationZ(degToRad(zRot), tmp);
    multMatrixByMatrix_mut(tmp, modelMatrix);

    loadRotationY(degToRad(yRot), tmp);
    multMatrixByMatrix_mut(tmp, modelMatrix);

    loadRotationX(degToRad(xRot), tmp);
    multMatrixByMatrix_mut(tmp, modelMatrix);

    loadTranslate(0.0f, 0.0f, -30.0f, tmp);
    multMatrixByMatrix_mut(tmp, modelMatrix);

    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(modelMatrix);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    for (size_t plane = 0; plane < OCT_PLANES; ++plane) {
        for (size_t i = 0; i < VERTS_PER_PLANE; ++i) {
            glVertex3fv(octahedron_mesh[plane][i]);
        }
    }
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void timer(int value) {
    (void) value;
    update();
    glutTimerFunc(33, timer, 1);
    if (glutGetWindow()) {
        glutPostRedisplay();
    }
}

void setup() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Depth Test");

    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(normalKeysDown);
    glutKeyboardUpFunc(normalKeysUp);

    glutReshapeFunc(reshape);
    glutDisplayFunc(render);

    glutTimerFunc(33, timer, 1);
    setup();
    glutMainLoop();
    return 0;
}
