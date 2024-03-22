#include <shapes.h>

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
//update acceleration based on key press
void moveSquare(Square* square, bool upKey, bool downKey, bool rightKey, bool leftKey){
    int maxVelocity = 10; //to be changed in testing
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
    square -> position = addPoints(square -> position, square -> velocity);
    square -> velocity = addPoints(square -> velocity, square -> acceleration);
}

//update velocity with keypress 
void moveSquareNoAcc(Square* square, bool upKey, bool downKey, bool rightKey, bool leftKey){
    int unit_velocity = 1;
    //if pushing up and down, and either left or right
    if((upKey && downKey) && !(leftKey && rightKey)){
        if(rightKey && !leftKey){
            square -> velocity = pointStruct(unit_velocity, 0);
        }else if(leftKey && !rightKey){
            square -> velocity = pointStruct(-unit_velocity, 0);
        }else{
            square -> velocity = pointStruct(0, 0);
        }
    //if pushing left and right, and either up or down
    }else if((leftKey && rightKey) && !(upKey && downKey)){
        if(upKey && !downKey){
            square -> velocity = pointStruct(0, -unit_velocity);
        }else if(downKey && !upKey){
            square -> velocity = pointStruct(0, unit_velocity);
        }else{
            square -> velocity = pointStruct(0, 0);
        }
    //if pushing all
    }else if(upKey && downKey && rightKey && leftKey){
        square -> velocity = pointStruct(0, 0);
    }else{
        //if pushing any 2 non-opposite directions
        if(upKey && rightKey){
            square -> velocity = pointStruct(unit_velocity, -unit_velocity);
        }else if(rightKey && downKey){
            square -> velocity = pointStruct(unit_velocity, unit_velocity);
        }else if(downKey && leftKey){
            square -> velocity = pointStruct(-unit_velocity, unit_velocity);
        }else if(leftKey && upKey){
            square -> velocity = pointStruct(-unit_velocity, -unit_velocity);
        //if pushing any 1 direction
        }else if(upKey){
            square -> velocity = pointStruct(0, -unit_velocity);
        }else if(rightKey){
            square -> velocity = pointStruct(unit_velocity, 0);
        }else if(downKey){
            square -> velocity = pointStruct(0, unit_velocity);
        }else if(leftKey){
            square -> velocity = pointStruct(-unit_velocity, 0);
        }
    }
    square -> position = addPoints(square -> position, square -> velocity);
}

//update square properties on gameloop
void updateSquare(Square* square, bool upKey, bool downKey, bool rightKey, bool leftKey){

    //move square, check for collisions, 
    moveSquareNoAcc(square, upKey, downKey, rightKey, leftKey);

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

