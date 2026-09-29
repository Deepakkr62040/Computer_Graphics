#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define PI 3.14159265f
#define W 1000
#define H 600
#define NUM_STARS 60

/* ---------- state ---------- */
static int   night = 0;
static int   paused = 0;
static float carX = -200.0f, carSpeed = 3.0f;
static float wheelAngle = 0.0f;
static float cloudX[3] = { 100.0f, 450.0f, 800.0f };
static float sunAngle = 0.0f;
static float starX[NUM_STARS], starY[NUM_STARS];
static int   tick = 0;

/* ---------- helpers ---------- */
static void color(float r, float g, float b) { glColor3f(r, g, b); }

static void rect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1); glVertex2f(x2, y1);
    glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
}

static void tri(float x1, float y1, float x2, float y2, float x3, float y3)
{
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1); glVertex2f(x2, y2); glVertex2f(x3, y3);
    glEnd();
}

static void circle(float cx, float cy, float r)
{
    int i;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (i = 0; i <= 40; i++) {
        float a = 2.0f * PI * i / 40.0f;
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
    }
    glEnd();
}

static void text(float x, float y, const char *s)
{
    glRasterPos2f(x, y);
    while (*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *s++);
}

/* ---------- scene objects ---------- */
static void drawSky(void)
{
    glBegin(GL_QUADS);
    if (night) {
        color(0.02f, 0.02f, 0.15f); glVertex2f(0, H); glVertex2f(W, H);
        color(0.10f, 0.10f, 0.35f); glVertex2f(W, 150); glVertex2f(0, 150);
    } else {
        color(0.25f, 0.55f, 0.95f); glVertex2f(0, H); glVertex2f(W, H);
        color(0.75f, 0.90f, 1.00f); glVertex2f(W, 150); glVertex2f(0, 150);
    }
    glEnd();
}

static void drawSunMoon(void)
{
    int i;
    if (night) {
        color(0.95f, 0.95f, 0.85f);
        circle(850, 500, 40);
        color(0.10f, 0.10f, 0.30f);          /* crescent cut-out */
        circle(868, 508, 34);
    } else {
        glPushMatrix();
        glTranslatef(850, 500, 0);
        glRotatef(sunAngle, 0, 0, 1);
        color(1.0f, 0.85f, 0.1f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        for (i = 0; i < 12; i++) {
            float a = 2.0f * PI * i / 12.0f;
            glVertex2f(55 * cosf(a), 55 * sinf(a));
            glVertex2f(80 * cosf(a), 80 * sinf(a));
        }
        glEnd();
        glPopMatrix();
        color(1.0f, 0.9f, 0.2f);
        circle(850, 500, 45);
    }
}

static void drawStars(void)
{
    int i;
    if (!night) return;
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (i = 0; i < NUM_STARS; i++) {
        float b = ((tick / 10 + i) % 5 == 0) ? 0.4f : 1.0f;   /* twinkle */
        color(b, b, b);
        glVertex2f(starX[i], starY[i]);
    }
    glEnd();
}

static void cloud(float x, float y)
{
    if (night) color(0.35f, 0.35f, 0.5f); else color(1, 1, 1);
    circle(x, y, 25);
    circle(x + 30, y + 10, 30);
    circle(x + 62, y, 25);
    circle(x + 30, y - 8, 25);
}

static void drawHills(void)
{
    if (night) color(0.05f, 0.20f, 0.12f); else color(0.25f, 0.60f, 0.30f);
    tri(-100, 150, 250, 330, 600, 150);
    if (night) color(0.04f, 0.16f, 0.10f); else color(0.20f, 0.50f, 0.25f);
    tri(350, 150, 700, 290, 1100, 150);
}

static void drawGround(void)
{
    float x;
    if (night) color(0.06f, 0.30f, 0.10f); else color(0.35f, 0.75f, 0.25f);
    rect(0, 0, W, 150);
    /* road */
    color(0.25f, 0.25f, 0.27f);
    rect(0, 20, W, 100);
    /* road dashes */
    color(1, 1, 1);
    for (x = 0; x < W; x += 100) rect(x + 10, 57, x + 60, 63);
}

static void drawHouse(float x, float y)
{
    /* walls */
    if (night) color(0.55f, 0.40f, 0.30f); else color(0.95f, 0.80f, 0.55f);
    rect(x, y, x + 180, y + 110);
    /* roof */
    color(0.70f, 0.15f, 0.12f);
    tri(x - 20, y + 110, x + 90, y + 190, x + 200, y + 110);
    /* door */
    color(0.40f, 0.22f, 0.10f);
    rect(x + 75, y, x + 110, y + 65);
    color(1.0f, 0.85f, 0.2f);
    circle(x + 103, y + 32, 3);
    /* windows */
    if (night) color(1.0f, 0.90f, 0.40f); else color(0.6f, 0.85f, 1.0f);
    rect(x + 20, y + 55, x + 60, y + 95);
    rect(x + 125, y + 55, x + 165, y + 95);
    color(0.3f, 0.2f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x + 40, y + 55);  glVertex2f(x + 40, y + 95);
    glVertex2f(x + 20, y + 75);  glVertex2f(x + 60, y + 75);
    glVertex2f(x + 145, y + 55); glVertex2f(x + 145, y + 95);
    glVertex2f(x + 125, y + 75); glVertex2f(x + 165, y + 75);
    glEnd();
    /* chimney */
    color(0.45f, 0.20f, 0.15f);
    rect(x + 140, y + 140, x + 165, y + 185);
}

static void drawTree(float x, float y)
{
    color(0.40f, 0.25f, 0.10f);
    rect(x - 8, y, x + 8, y + 60);
    if (night) color(0.05f, 0.30f, 0.10f); else color(0.10f, 0.55f, 0.15f);
    circle(x, y + 85, 35);
    circle(x - 22, y + 65, 28);
    circle(x + 22, y + 65, 28);
}

static void drawWheel(float cx, float cy)
{
    int i;
    color(0.05f, 0.05f, 0.05f);
    circle(cx, cy, 16);
    color(0.75f, 0.75f, 0.75f);
    circle(cx, cy, 8);
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(-wheelAngle, 0, 0, 1);
    color(0.2f, 0.2f, 0.2f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (i = 0; i < 4; i++) {
        float a = PI * i / 4.0f;
        glVertex2f(-8 * cosf(a), -8 * sinf(a));
        glVertex2f( 8 * cosf(a),  8 * sinf(a));
    }
    glEnd();
    glPopMatrix();
}

static void drawCar(float x, float y)
{
    /* body */
    color(0.85f, 0.10f, 0.10f);
    rect(x, y + 12, x + 140, y + 42);
    /* cabin */
    tri(x + 30, y + 42, x + 50, y + 75, x + 50, y + 42);
    rect(x + 50, y + 42, x + 100, y + 75);
    tri(x + 100, y + 42, x + 100, y + 75, x + 120, y + 42);
    /* windows */
    color(0.7f, 0.9f, 1.0f);
    rect(x + 55, y + 47, x + 75, y + 70);
    rect(x + 80, y + 47, x + 98, y + 70);
    /* headlight */
    if (night) {
        color(1.0f, 1.0f, 0.6f);
        circle(x + 138, y + 30, 6);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.5f, 0.25f);
        tri(x + 140, y + 30, x + 260, y + 5, x + 260, y + 55);
        glDisable(GL_BLEND);
    } else {
        color(1.0f, 0.95f, 0.5f);
        circle(x + 138, y + 30, 5);
    }
    /* wheels */
    drawWheel(x + 30, y + 12);
    drawWheel(x + 110, y + 12);
}

/* ---------- callbacks ---------- */
static void display(void)
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawStars();
    drawSunMoon();
    for (i = 0; i < 3; i++) cloud(cloudX[i], 480 - i * 35);
    drawHills();
    drawGround();
    drawTree(80, 110);
    drawTree(930, 110);
    drawHouse(300, 110);
    drawTree(600, 110);
    drawTree(700, 110);
    drawCar(carX, 40);

    color(night ? 1.0f : 0.0f, night ? 1.0f : 0.0f, night ? 1.0f : 0.0f);
    text(10, H - 20, "N: day/night   +/-: speed   SPACE: pause   ESC: quit");

    glutSwapBuffers();
}

static void update(int v)
{
    (void)v;
    if (!paused) {
        int i;
        tick++;
        carX += carSpeed;
        if (carX > W + 50) carX = -200;
        wheelAngle += carSpeed * 4.0f;
        sunAngle += 0.6f;
        for (i = 0; i < 3; i++) {
            cloudX[i] += 0.4f + 0.2f * i;
            if (cloudX[i] > W + 80) cloudX[i] = -120;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

static void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    switch (key) {
    case 'n': case 'N': night = !night; break;
    case '+': case '=': carSpeed += 0.5f; break;
    case '-': case '_': if (carSpeed > 0.5f) carSpeed -= 0.5f; break;
    case ' ': paused = !paused; break;
    case 27:  exit(0);
    }
}

static void init(void)
{
    int i;
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W, 0, H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_LINE_SMOOTH);
    for (i = 0; i < NUM_STARS; i++) {
        starX[i] = (float)(rand() % W);
        starY[i] = 200.0f + (float)(rand() % (H - 200));
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutInitWindowPosition(100, 60);
    glutCreateWindow("Computer Graphics Project - Animated Village Scene");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}