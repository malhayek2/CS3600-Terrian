// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <cmath>
#include <cstring>
#include <iostream>
#include <time.h>
#include "Rat.h"
#include "glut.h"
// #include <GLUT/GLUT.h>


// Global Variables (Only what you need!)
double screen_x = 1000;
double screen_y = 800;
bool gLeft, gMiddle, gRight, gFirstPerson;
double gWaterHeight = -1.9;

Maze gMaze;
Rat gRat;

double GetDeltaTime()
{
	static clock_t start_time = clock();
	static int current_frame = 0;
	clock_t current_time = clock();
	current_frame += 1;
	double total_time = double(current_time - start_time)/CLOCKS_PER_SEC;
	if (total_time == 0)
		total_time = .00001;
	double frames_per_second = (double)current_frame / total_time;
	double DT = 1.0 / frames_per_second;
	return DT;
}

// 
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

//void DrawRectangle(double x1, double y1, double x2, double y2)
//{
//	glBegin(GL_QUADS);
//	glVertex2d(x1,y1);
//	glVertex2d(x2,y1);
//	glVertex2d(x2,y2);
//	glVertex2d(x1,y2);
//	glEnd();
//}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}


//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();
    double dt = GetDeltaTime();
    if (gFirstPerson)
    {
        double rad = gRat.getDegrees()/180.0 * 3.12;
        double dx = cos(rad) * MOVE_SPEED * dt;
        double dy = sin(rad) * MOVE_SPEED * dt;

        double hover = 2.0;
        double terrainHeight = gMaze.getZ(gRat.getX() + dx, gRat.getY() + dy);
        double waterHeight = gWaterHeight + .4;
        double H = fmax(terrainHeight, waterHeight) + hover;
        double currentTerrainHeight = fmax(waterHeight, gMaze.getZ(gRat.getX(), gRat.getY()));
        double tilt = (fmax(terrainHeight, waterHeight) - currentTerrainHeight);
        double lookZ = H + tilt;
        gluLookAt(gRat.getX(), gRat.getY(), H, gRat.getX() + dx, gRat.getY() + dy, lookZ, 0, 0, 1);
        // when doing rat, calculate at point but z will stay.
    }
    else
    {
        gluLookAt(M*.5, -N*.5, 15, M*.5, N*.5, 0, 0, 0, 1); // 3 eye, 3 at point, 3 z-axis up
    }

	gMaze.draw(gWaterHeight);
    gRat.draw(gFirstPerson);
    if (gLeft) gRat.spinLeft(dt);
    if (gRight) gRat.spinRight(dt);
    if (gMiddle) gRat.move(dt);

    if (gFirstPerson)
    {
        gluPerspective(.02, (double)screen_x/screen_y, .0001, .0001);
    }
    else
    {
        gluPerspective(40, (double)screen_x/screen_y, RES*.5, 3*(RES+RES));
    }

	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'b':
			// do something when 'b' character is hit.
			break;
    case '1':
        gFirstPerson = !gFirstPerson;
        break;
    case 'd':
      gMiddle = !gMiddle;
      break;
    case 'i':
      gWaterHeight += 0.2;
      break;
    case 'k':
      gWaterHeight -= 0.2;
      break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	gluOrtho2D(-.5, M+.5, -.5, N+.5);
    if (gFirstPerson)
    {
        gluPerspective(60, (double)w/h, .0001, M*N);
    }
    else
    {
        gluPerspective(40, (double)w/h, N*.5, 3*(M+N));
    }
	glMatrixMode(GL_MODELVIEW);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
        gLeft = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
        gLeft = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
        gMiddle = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
        gMiddle = false;
	}
    if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
        gRight = true;
	}
    if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
        gRight = false;
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
    gRat.setMaze(&gMaze);
    gFirstPerson = true;
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("This appears in the title bar");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
