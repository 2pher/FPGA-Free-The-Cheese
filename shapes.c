#include "globals.h"
#include "utilities.h"
#include "shapes.h"

//square 'constructor'
Square* squareStruct(point* position, int sideLength){

    //create points
    Square* newSquare;
    point* pos = position;
    point* vel = pointStruct(0, 0);
    point* acc = pointStruct(0, 0);
    int hitboxSideLength = sideLength - 2;
    squareHitbox* newHitbox = centreSquareHitboxStruct(position, hitboxSideLength);

    //assign values to square
    newSquare -> position = pos;
    newSquare -> velocity = vel;
    newSquare -> acceleration = acc;
    newSquare -> sideLength = sideLength;
    newSquare -> hitbox = newHitbox;
    newSquare -> hitboxSideLength = hitboxSideLength;
    newSquare -> isAlive = true;

    return newSquare;

}
//uodate acceleration based on key press
void moveSquare(Square* square, bool upKey, bool downKey, bool rightKey, bool leftKey){
    int acceleration = 1; //to be changed in testing

    //if pushing up and down, and either left or right
    if((upKey && downKey) && !(leftKey && rightKey)){
        if(rightKey && !leftKey){
            square -> acceleration = pointStruct(acceleration, 0);
        }else if(leftKey && !rightKey){
            square -> acceleration = pointStruct(-acceleration, 0);
        }else{
            square -> acceleration = pointStruct(0, 0);
        }
    //if pushing left and right, and either up or down
    }else if((leftKey && rightKey) && !(upKey && downKey)){
        if(upKey && !downKey){
            square -> acceleration = pointStruct(0, -acceleration);
        }else if(downKey && !upKey){
            square -> acceleration = pointStruct(0, acceleration);
        }else{
            square -> acceleration = pointStruct(0, 0);
        }
    //if pushing all
    }else if(upKey && downKey && rightKey && leftKey){
        square -> acceleration = pointStruct(0, 0);
    }else{
        //if pushing any 2 non-opposite directions
        if(upKey && rightKey){
            square -> acceleration = pointStruct(acceleration, -acceleration);
        }else if(rightKey && downKey){
            square -> acceleration = pointStruct(acceleration, acceleration);
        }else if(downKey && leftKey){
            square -> acceleration = pointStruct(-acceleration, acceleration);
        }else if(leftKey && upKey){
            square -> acceleration = pointStruct(-acceleration, -acceleration);
        //if pushing any 1 direction
        }else if(upKey){
            square -> acceleration = pointStruct(0, -acceleration);
        }else if(rightKey){
            square -> acceleration = pointStruct(acceleration, 0);
        }else if(downKey){
            square -> acceleration = pointStruct(0, acceleration);
        }else if(leftKey){
            square -> acceleration = pointStruct(-acceleration, 0);
        }
    }
}

//update square properties on gameloop
void updateSquare(Square* square){
    square -> position = addPoints(square -> position, square -> velocity);
    square -> velocity = addPoints(square -> velocity, square -> acceleration);

    //check for collisions, manually change velocity to 0
    //also need to set a max velocity probably

}

//circle 'constructor'
Circle* circleStruct(point* position, int radius, point** path){

    //create points
    Circle* newCircle;
    point* pos = position;
    point* vel = pointStruct(0, 0);
    point* acc = pointStruct(0, 0);
    int hitboxRadius = radius - 2;
    circleHitbox* newHitbox = circleHitboxStruct(position, hitboxRadius);

    //assign values to circle
    newCircle -> position = pos;
    newCircle -> velocity = vel;
    newCircle -> acceleration = acc;
    newCircle -> radius = radius;
    newCircle -> hitbox = newHitbox;
    newCircle -> hitboxRadius = hitboxRadius;
    newCircle -> path = path;
    newCircle -> isVisible = true;

    return newCircle;

}

