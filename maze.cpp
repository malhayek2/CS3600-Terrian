#include "maze.h"
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <time.h>
#include <cmath>

int randomNumberInRange(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

void Maze::Cell::setVisited(bool visited)
{
    this->visited = visited;
}

void Maze::Cell::setLeft(bool left)
{
    this->left = left;
}

void Maze::Cell::setTop(bool top)
{
    this->top = top;
}

void Maze::Cell::setRight(bool right)
{
    this->right = right;
}

void Maze::Cell::setBottom(bool bottom)
{
    this->bottom = bottom;
}

bool Maze::Cell::getVisited()
{
    return visited;
}

bool Maze::Cell::getLeft()
{
    return left;
}

bool Maze::Cell::getTop()
{
    return top;
}

bool Maze::Cell::getRight()
{
    return right;
}

bool Maze::Cell::getBottom()
{
    return bottom;
}

// END GETTERS AND SETTERS //

void Maze::Cell::draw(int i, int j)
{
    glColor3ub(i*2342423%255, j*2234232342%255, j*i*128%255);
    if (bottom)
    {
        
        glBegin(GL_QUADS);
        glVertex3d(i, j, 0);
        glVertex3d(i+1, j, 0);
        glVertex3d(i+1, j, 1);
        glVertex3d(i, j, 1);
        glEnd();
    }
    if (right)
    {
        glBegin(GL_QUADS);
        glVertex3d(i+1, j, 0);
        glVertex3d(i+1, j+1, 0);
        glVertex3d(i+1, j+1, 1);
        glVertex3d(i+1, j, 1);
        glEnd();
    }
    if (top)
    {
        glBegin(GL_QUADS);
        glVertex3d(i+1, j+1, 0);
        glVertex3d(i, j+1, 0);
        glVertex3d(i, j+1, 1);
        glVertex3d(i+1, j+1, 1);
        glEnd();
    }
    if (left)
    {
        glBegin(GL_QUADS);
        glVertex3d(i, j+1, 0);
        glVertex3d(i, j, 0);
        glVertex3d(i, j, 1);
        glVertex3d(i, j+1, 1);
        glEnd();
    }
}

double Maze::getZ(double x, double y)
{
    // return (y * .03 * sin(y * .09)) + (.221 * y * cos(x*.09)) + ( .9 * sin(x*.432)) * (x * .03 + cos(y*.32324));
    double z = 0;
    double zscale = 0.75;
    z += 2*sin(0.4*y);
    z += 1.5*cos(0.3*x);
    z += 4*sin(0.2*x)*cos(0.3*y);
    z += 6*sin(0.11*x)*cos(0.03*y);
    return z*zscale; 
}

void Maze::draw(double waterHeight)
{
    double zValues[RES+1][RES+1];
    for (int i=0; i<RES+1; i++)
    {
        for (int j=0; j<RES+1; j++)
        {
            zValues[i][j] = getZ(i, j);
        }
    }


    for (int i=0; i<RES-1; i++)
    {
        for (int j=0; j<RES-1; j++)
        {
            int r = i*300 + j *350 + 75;
            int g = i*200 + j*450 + 25;
            int b = i*1200 + j*550 + 175;
            glColor3ub((unsigned char) (r%256), (unsigned char) (g%256), (unsigned char) (b%256) );
            // glColor3ub(i*2342423%255, j*2862257%255, j*i*128%255);
            glBegin(GL_QUADS);
            glVertex3d(i, j, zValues[i][j]);
            glVertex3d(i+1, j, zValues[i+1][j]);
            glVertex3d(i+1, j+1, zValues[i+1][j+1]);
            glVertex3d(i, j+1, zValues[i][j+1]);
            glEnd();

            glColor4ub(0, 0, 255, 1.0);
            double waterZ = waterHeight;
            glBegin(GL_QUADS);
            glVertex3d(i, j, waterZ);
            glVertex3d(i+1, j, waterZ);
            glVertex3d(i+1, j+1, waterZ);
            glVertex3d(i, j+1, waterZ);
            glEnd();
        }
    }
}

Maze::Maze()
{
    srand(time(NULL));
}

bool Maze::isLegal(double x, double y, double r)
{
    int cellx = (int)x;
    int celly = (int)y;
    double offsetx = x - cellx;
    double offsety = y - celly;
    Cell cell = cells[cellx][celly];

    // wall tests
    if ((cell.getLeft() && offsetx - r < 0) ||
        (cell.getRight() && offsetx + r > 1) ||
        (cell.getBottom() && offsety - r < 0) ||
        (cell.getTop() && offsety + r > 1))
        {
            return false;
        }


    // corner tests
    if ((offsetx + r > 1 && offsety - r < 0) || // bottom right
        (offsetx - r < 0 && offsety - r < 0) || // bottom left
        (offsetx - r < 0 && offsety + r > 1) || // top left
        (offsetx +r > 1 && offsety +r > 1)) // top right
        {
            return false;
        }

    return true;
}
