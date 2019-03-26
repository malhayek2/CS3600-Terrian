//
//  Rat.h
//  Maze
//
//  Created by Nate Armstrong on 2/6/14.
//  Copyright (c) 2014 Nate Armstrong. All rights reserved.
//

#ifndef __Maze__Rat__
#define __Maze__Rat__

#include "maze.h"

const double MOVE_SPEED = 25;
const double SPIN_SPEED = .5;

class Rat
{
public:
    Rat();
    Maze * getMaze();
    double getX();
    double getY();
    double getDegrees();
    void setMaze(Maze *maze);
    void setX(double x);
    void setY(double y);
    void setDegrees(double degrees);
    void draw(double invisible);
    void move(double dt);
    void spinLeft(double dt);
    void spinRight(double dt);

private:
    double x, y, degrees;
    Maze *maze;
};

#endif /* defined(__Maze__Rat__) */
