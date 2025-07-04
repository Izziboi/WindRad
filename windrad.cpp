// Dieser Code steuert die Geschwindigkeit eines Windrads

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <direct.h>

using namespace std;

double angle = 0.0;           // Drehungswinkel
int speed = 0;                // Geschwindigkeitseinstellung
string settingsFile = "settings.txt";

// Diese Funktion liest die Geschwindigkeitseinstellung aus der Datei settings.txt
void readSettings() {
    ifstream file(settingsFile);
    if (file.is_open()) {
        int value;
        file >> value;
        if (value >= 1 && value <= 10)
            speed = value;
        else
            cerr << "Ungueltige Geschwindigkeit in Datei. Muss 1 bis 10 sein." << endl;
        file.close();
    }
    else {
        cerr << "Koennte settings.txt nicht oeffnen" << endl;
    }
}

// timer-Funktion zur Aktualisierung der Geschwindigkeit alle 5 Sekunden
void timer(int value) {
    readSettings();                      
    glutTimerFunc(5000, timer, 0);       // In 5 Sekunden erneut anrufen
}

// Zeichnen Sie eine einzelne Klinge
void drawBlade() {
    glBegin(GL_TRIANGLES);
    glColor3d(0.7, 0.7, 0.7);         // Graue Farbe
    glVertex2d(0.0, 0.0);
    glVertex2d(-0.1, 0.6);
    glVertex2d(0.1, 0.6);
    glEnd();
}

// Von GLUT aufgerufene Anzeigefunktion
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Turm zeichnen
    glColor3d(0.3, 0.3, 0.3);         // Dunkelgraue Farbe
    glBegin(GL_QUADS);
    glVertex2d(-0.05, -1.0);
    glVertex2d(0.05, -1.0);
    glVertex2d(0.05, 0.3);
    glVertex2d(-0.05, 0.3);
    glEnd();

    // Nabe zeichnen
    glColor3d(0.8, 0.0, 0.0);         // Rote mitte
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159 / 180.0;
        glVertex2d(0.05 * cos(theta), 0.05 * sin(theta) + 0.3);
    }
    glEnd();

    // Klingen zeichnen
    glPushMatrix();
    glTranslated(0.0, 0.3, 0.0);      // Zum Hub bewegen
    glRotated(angle, 0.0, 0.0, 1.0);  // Drehen um den Mittelpunkt
    for (int i = 0; i < 3; ++i) {     // 3 hier bedeutet die 3 Klingen
        drawBlade();
        glRotated(120.0, 0.0, 0.0, 1.0);  // Nächste Klinge
    }
    glPopMatrix();

    glutSwapBuffers();
}

// idle-funktion, die wiederholt aufgerufen wird, um zu animieren
void idle() {
    angle += 0.1 * speed;     // Drehgeschwindigkeit anpassen
    if (angle >= 360.0)
        angle -= 360.0;
    glutPostRedisplay();
}

// Initialisierung der Fenster- und OpenGL-Einstellungen
void init() {
    glClearColor(0.5, 0.8, 1.0, 1.0); // Leichtblaue Farbe
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // 2D-Projektion
}

int main(int argc, char** argv)
{
    readSettings();  // Erstes Lesen

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Windrad");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutTimerFunc(5000, timer, 0); // Timer für das Neuladen der Einstellungen starten

    glutMainLoop();


    return 0;
}

