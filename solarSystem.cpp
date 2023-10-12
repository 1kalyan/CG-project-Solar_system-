#include <GLUT/GLUT.h>
#include <cmath>

// Variables for rotation
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

// Function to handle keyboard input
void handleKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            xRot -= 5.0f;
            break;
        case 's':
            xRot += 5.0f;
            break;
        case 'a':
            yRot -= 5.0f;
            break;
        case 'd':
            yRot += 5.0f;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void update(int value) {
    xRot += 0.5f;
    yRot += 0.5f;

    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the projection matrix
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    // Set the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Apply rotation based on user input
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    // Enable lighting and set up light parameters
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set light properties
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Set material properties for the sun
    GLfloat sunAmbient[] = { 1.0f, 0.5f, 0.0f, 1.0f };
    GLfloat sunDiffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    GLfloat sunSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat sunShininess[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, sunAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sunDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sunSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, sunShininess);

    // Draw the sun as a sphere
    glColor3f(1.0, 1.0f, 0.0f);
    glutSolidSphere(1.0f, 50, 50);
    
    // Enable specular reflection for the sun
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glColor3f(1.0, 0.5f, 0.0f);
        glMaterialfv(GL_FRONT, GL_SPECULAR, sunSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, sunShininess);

        // Rotate the sun for shining effect
        glPushMatrix();
        glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.1f, 0.0f, 1.0f, 0.0f);
        glutSolidSphere(1.0f, 50, 50);
        glPopMatrix();

    // Render the planets
    GLfloat planetColors[][3] = {
        {0.7f, 0.7f, 0.7f},  // Mercury (Gray)
        {0.8f, 0.6f, 0.4f},  // Venus (Sandy Brown)
        {0.2f, 0.4f, 0.8f},  // Earth (Deep Sky Blue)
        {0.7f, 0.2f, 0.2f},  // Mars (Indian Red)
        {0.9f, 0.6f, 0.4f},  // Jupiter (Navajo White)
        {0.8f, 0.6f, 0.8f},  // Saturn (Thistle)
        {0.4f, 0.8f, 0.8f},  // Uranus (Turquoise)
        {0.4f, 0.4f, 0.9f}   // Neptune (Royal Blue)
    };

    for (int i = 0; i < 8; i++) {
        GLfloat distanceFromSun = 1.5f + i * 0.6f;      // Increase the distance for each planet
        GLfloat orbitSpeed = 0.02f + i * 0.01f;          // Increase the orbit speed for each planet
        GLfloat rotationSpeed = 1.0f + i * 0.5f;         // Increase the rotation speed for each planet

        // Calculate the current position of the planet in its circular orbit
        GLfloat angle = (glutGet(GLUT_ELAPSED_TIME) * 0.001f * orbitSpeed) * (360.0f / 360.0f);
        GLfloat x = distanceFromSun * cos(angle);
        GLfloat z = distanceFromSun * sin(angle);

        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.01f * rotationSpeed, 0.0f, 1.0f, 0.0f);  // Rotate the planet around its own axis
        glColor3fv(planetColors[i]);

        // Set material properties for the planet
        glMaterialfv(GL_FRONT, GL_AMBIENT, planetColors[i]);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, planetColors[i]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, sunSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, sunShininess);

        glutSolidSphere(0.2f, 30, 30);
        glPopMatrix();
    }

    // Disable lighting for drawing stars
    glDisable(GL_LIGHTING);

    // Draw small white dots as stars in the background
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; i++) {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f - 10.0f;
        float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f - 10.0f;
        float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f - 10.0f;
        glVertex3f(x, y, -z);
    }
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solar System");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glutDisplayFunc(renderScene);
    glutKeyboardFunc(handleKeyboard);
    glutTimerFunc(10, update, 0);

    glutMainLoop();
    return 0;
}

