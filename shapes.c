#include "shapes.h"

extern bool KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT;

//square 'constructor'
Square* squareStruct(point* pos, int sideLength) {
    Square* newSquare = malloc(sizeof(Square)); // Allocate memory
    if (newSquare != NULL) { // Check if memory allocation was successful
        point* vel = pointStruct(0, 0);
        point* acc = pointStruct(0, 0);
        int hitboxSideLength = sideLength - 2;
        squareHitbox* newHitbox = centreSquareHitboxStruct(pos, hitboxSideLength);

        newSquare->position = pos;
        newSquare->velocity = vel;
        newSquare->acceleration = acc;
        newSquare->sideLength = sideLength;
        newSquare->hitbox = newHitbox;
        newSquare->hitboxSideLength = hitboxSideLength;
        newSquare->isAlive = true;
    }
    return newSquare;
}

void freeSquare(Square* s) {
    freePoint(s->position);
    freePoint(s->velocity);
    freePoint(s->acceleration);
    free(s->hitbox->topLeftPoint);
    free(s->hitbox->bottomRightPoint);
    free(s->hitbox);
    free(s);
}

//update acceleration based on key press
void moveSquare(Square* square){
    int maxVelocity = 10; //to be changed in testing
    int acceleration = 1; //to be changed in testing

    //if pushing up and down, and either left or right
    if((KEY_UP && KEY_DOWN) && !(KEY_LEFT && KEY_RIGHT)){
        if(KEY_RIGHT && !KEY_LEFT){
            square -> acceleration = pointStruct(acceleration, 0);
        }else if(KEY_LEFT && !KEY_RIGHT){
            square -> acceleration = pointStruct(-acceleration, 0);
        }else{
            square -> acceleration = pointStruct(0, 0);
        }
    //if pushing left and right, and either up or down
    }else if((KEY_LEFT && KEY_RIGHT) && !(KEY_UP && KEY_DOWN)){
        if(KEY_UP && !KEY_DOWN){
            square -> acceleration = pointStruct(0, -acceleration);
        }else if(KEY_DOWN && !KEY_UP){
            square -> acceleration = pointStruct(0, acceleration);
        }else{
            square -> acceleration = pointStruct(0, 0);
        }
    //if pushing all
    }else if(KEY_UP && KEY_DOWN && KEY_RIGHT && KEY_LEFT){
        square -> acceleration = pointStruct(0, 0);
    }else{
        //if pushing any 2 non-opposite directions
        if(KEY_UP && KEY_RIGHT){
            square -> acceleration = pointStruct(acceleration, -acceleration);
        }else if(KEY_RIGHT && KEY_DOWN){
            square -> acceleration = pointStruct(acceleration, acceleration);
        }else if(KEY_DOWN && KEY_LEFT){
            square -> acceleration = pointStruct(-acceleration, acceleration);
        }else if(KEY_LEFT && KEY_UP){
            square -> acceleration = pointStruct(-acceleration, -acceleration);
        //if pushing any 1 direction
        }else if(KEY_UP){
            square -> acceleration = pointStruct(0, -acceleration);
        }else if(KEY_RIGHT){
            square -> acceleration = pointStruct(acceleration, 0);
        }else if(KEY_DOWN){
            square -> acceleration = pointStruct(0, acceleration);
        }else if(KEY_LEFT){
            square -> acceleration = pointStruct(-acceleration, 0);
        }
    }
    square -> position = addPoints(square -> position, square -> velocity);
    square -> velocity = addPoints(square -> velocity, square -> acceleration);
}

//update velocity with keypress 
void moveSquareNoAcc(Square* square){
    int unit_velocity = 1;
    //if pushing up and down, and either left or right
    if((KEY_UP && KEY_DOWN) && !(KEY_LEFT && KEY_RIGHT)){
        if(KEY_RIGHT && !KEY_LEFT){
            square -> velocity = pointStruct(unit_velocity, 0);
        }else if(KEY_LEFT && !KEY_RIGHT){
            square -> velocity = pointStruct(-unit_velocity, 0);
        }else{
            square -> velocity = pointStruct(0, 0);
        }
    //if pushing left and right, and either up or down
    }else if((KEY_LEFT && KEY_RIGHT) && !(KEY_UP && KEY_DOWN)){
        if(KEY_UP && !KEY_DOWN){
            square -> velocity = pointStruct(0, -unit_velocity);
        }else if(KEY_DOWN && !KEY_UP){
            square -> velocity = pointStruct(0, unit_velocity);
        }else{
            square -> velocity = pointStruct(0, 0);
        }
    //if pushing all
    }else if(KEY_UP && KEY_DOWN && KEY_RIGHT && KEY_LEFT){
        square -> velocity = pointStruct(0, 0);
    }else{
        //if pushing any 2 non-opposite directions
        if(KEY_UP && KEY_RIGHT){
            square -> velocity = pointStruct(unit_velocity, -unit_velocity);
        }else if(KEY_RIGHT && KEY_DOWN){
            square -> velocity = pointStruct(unit_velocity, unit_velocity);
        }else if(KEY_DOWN && KEY_LEFT){
            square -> velocity = pointStruct(-unit_velocity, unit_velocity);
        }else if(KEY_LEFT && KEY_UP){
            square -> velocity = pointStruct(-unit_velocity, -unit_velocity);
        //if pushing any 1 direction
        }else if(KEY_UP){
            square -> velocity = pointStruct(0, -unit_velocity);
        }else if(KEY_RIGHT){
            square -> velocity = pointStruct(unit_velocity, 0);
        }else if(KEY_DOWN){
            square -> velocity = pointStruct(0, unit_velocity);
        }else if(KEY_LEFT){
            square -> velocity = pointStruct(-unit_velocity, 0);
        }
    }
    square -> position = addPoints(square -> position, square -> velocity);
}

//update square properties on gameloop
void updateSquare(Square* square){

    //move square, check for collisions, 
    moveSquareNoAcc(square);

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

