#include "shapes.h"

extern bool KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT;
extern int DEATH_COUNT;

// square 'constructor'
Square* squareStruct(point* pos, int sideLength) {
  Square* newSquare = malloc(sizeof(Square));  // Allocate memory
  if (newSquare != NULL) {  // Check if memory allocation was successful
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
    newSquare->respawn = pos;
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
  free(s->respawn);
  free(s);
}

// update acceleration based on key press
void moveSquare(Square* square) {
  int maxVelocity = 10;  // to be changed in testing
  int acceleration = 1;  // to be changed in testing

  // if pushing up and down, and either left or right
  if ((KEY_UP && KEY_DOWN) && !(KEY_LEFT && KEY_RIGHT)) {
    if (KEY_RIGHT && !KEY_LEFT) {
      square->acceleration = pointStruct(acceleration, 0);
    } else if (KEY_LEFT && !KEY_RIGHT) {
      square->acceleration = pointStruct(-acceleration, 0);
    } else {
      square->acceleration = pointStruct(0, 0);
    }
    // if pushing left and right, and either up or down
  } else if ((KEY_LEFT && KEY_RIGHT) && !(KEY_UP && KEY_DOWN)) {
    if (KEY_UP && !KEY_DOWN) {
      square->acceleration = pointStruct(0, -acceleration);
    } else if (KEY_DOWN && !KEY_UP) {
      square->acceleration = pointStruct(0, acceleration);
    } else {
      square->acceleration = pointStruct(0, 0);
    }
    // if pushing all
  } else if (KEY_UP && KEY_DOWN && KEY_RIGHT && KEY_LEFT) {
    square->acceleration = pointStruct(0, 0);
  } else {
    // if pushing any 2 non-opposite directions
    if (KEY_UP && KEY_RIGHT) {
      square->acceleration = pointStruct(acceleration, -acceleration);
    } else if (KEY_RIGHT && KEY_DOWN) {
      square->acceleration = pointStruct(acceleration, acceleration);
    } else if (KEY_DOWN && KEY_LEFT) {
      square->acceleration = pointStruct(-acceleration, acceleration);
    } else if (KEY_LEFT && KEY_UP) {
      square->acceleration = pointStruct(-acceleration, -acceleration);
      // if pushing any 1 direction
    } else if (KEY_UP) {
      square->acceleration = pointStruct(0, -acceleration);
    } else if (KEY_RIGHT) {
      square->acceleration = pointStruct(acceleration, 0);
    } else if (KEY_DOWN) {
      square->acceleration = pointStruct(0, acceleration);
    } else if (KEY_LEFT) {
      square->acceleration = pointStruct(-acceleration, 0);
    }
  }
  square->position = addPoints(square->position, square->velocity);
  square->velocity = addPoints(square->velocity, square->acceleration);
}

// update velocity with keypress
void moveSquareNoAcc(Square* square) {
  int unit_velocity = 1;
  // if pushing up and down, and either left or right
  if ((KEY_UP && KEY_DOWN) && !(KEY_LEFT && KEY_RIGHT)) {
    if (KEY_RIGHT && !KEY_LEFT) {
      if (checkBoundaryRight(square))
        square->velocity = pointStruct(unit_velocity, 0);
      else
        square->velocity = pointStruct(0, 0);
    } else if (KEY_LEFT && !KEY_RIGHT) {
      if (checkBoundaryLeft(square))
        square->velocity = pointStruct(-unit_velocity, 0);
      else
        square->velocity = pointStruct(0, 0);
    } else {
      square->velocity = pointStruct(0, 0);
    }
    // if pushing left and right, and either up or down
  } else if ((KEY_LEFT && KEY_RIGHT) && !(KEY_UP && KEY_DOWN)) {
    if (KEY_UP && !KEY_DOWN) {
      if (checkBoundaryUp(square))
        square->velocity = pointStruct(0, -unit_velocity);
      else
        square->velocity = pointStruct(0, 0);
    } else if (KEY_DOWN && !KEY_UP) {
      if (checkBoundaryDown(square))
        square->velocity = pointStruct(0, unit_velocity);
      else
        square->velocity = pointStruct(0, 0);
    } else {
      square->velocity = pointStruct(0, 0);
    }
    // if pushing all
  } else if (KEY_UP && KEY_DOWN && KEY_RIGHT && KEY_LEFT) {
    square->velocity = pointStruct(0, 0);
  } else {
    // if pushing any 2 non-opposite directions
    if (KEY_UP && KEY_RIGHT) {
      if (!checkBoundaryDiagonal(square, 1, -1)) {
        square->velocity = pointStruct(0, 0);
      } else if (!checkBoundaryUp(square)) {
        square->velocity = pointStruct(unit_velocity, 0);
      } else if (!checkBoundaryRight(square)) {
        square->velocity = pointStruct(0, -unit_velocity);
      } else {
        square->velocity = pointStruct(unit_velocity, -unit_velocity);
      }
    } else if (KEY_RIGHT && KEY_DOWN) {
      if (!checkBoundaryDiagonal(square, 1, 1)) {
        square->velocity = pointStruct(0, 0);
      } else if (!checkBoundaryDown(square)) {
        square->velocity = pointStruct(unit_velocity, 0);
      } else if (!checkBoundaryRight(square)) {
        square->velocity = pointStruct(0, unit_velocity);
      } else {
        square->velocity = pointStruct(unit_velocity, unit_velocity);
      }
    } else if (KEY_DOWN && KEY_LEFT) {
      if (!checkBoundaryDiagonal(square, -1, 1)) {
        square->velocity = pointStruct(0, 0);
      } else if (!checkBoundaryDown(square)) {
        square->velocity = pointStruct(-unit_velocity, 0);
      } else if (!checkBoundaryLeft(square)) {
        square->velocity = pointStruct(0, unit_velocity);
      } else {
        square->velocity = pointStruct(-unit_velocity, unit_velocity);
      }
    } else if (KEY_LEFT && KEY_UP) {
      if (!checkBoundaryDiagonal(square, -1, -1)) {
        square->velocity = pointStruct(0, 0);
      } else if (!checkBoundaryUp(square)) {
        square->velocity = pointStruct(-unit_velocity, 0);
      } else if (!checkBoundaryLeft(square)) {
        square->velocity = pointStruct(0, -unit_velocity);
      } else {
        square->velocity = pointStruct(-unit_velocity, -unit_velocity);
      }
      // if pushing any 1 direction
    } else if (KEY_UP) {
      if (checkBoundaryUp(square))
        square->velocity = pointStruct(0, -unit_velocity);
      else
        square->velocity = pointStruct(0, 0);
    } else if (KEY_RIGHT) {
      if (checkBoundaryRight(square))
        square->velocity = pointStruct(unit_velocity, 0);
      else
        square->velocity = pointStruct(0, 0);
    } else if (KEY_DOWN) {
      if (checkBoundaryDown(square))
        square->velocity = pointStruct(0, unit_velocity);
      else
        square->velocity = pointStruct(0, 0);
    } else if (KEY_LEFT) {
      if (checkBoundaryLeft(square))
        square->velocity = pointStruct(-unit_velocity, 0);
      else
        square->velocity = pointStruct(0, 0);
    } else {
      square->velocity = pointStruct(0, 0);
    }
  }
  square->position = addPoints(square->position, square->velocity);
}

// update square properties on gameloop
void updateSquare(Square* square) {
  // move square, check for collisions,
  moveSquareNoAcc(square);
}

// circle 'constructor'
Circle* circleStruct(point* position, int radius, point* velocity,
                     point** path) {
  // create points
  Circle* newCircle;
  point* pos = position;
  point* vel = velocity;
  point* acc = pointStruct(0, 0);
  int hitboxRadius = radius - 2;
  squareHitbox* newHitbox = centreSquareHitboxStruct(position, 2*hitboxRadius);

  // assign values to circle
  newCircle->position = pos;
  newCircle->velocity = vel;
  newCircle->acceleration = acc;
  newCircle->radius = radius;
  newCircle->hitbox = newHitbox;
  newCircle->hitboxRadius = hitboxRadius;
  newCircle->path = path;
  newCircle->isVisible = true;

  return newCircle;
}

void freeCircle(Circle* circle){
  free(circle->position);
  free(circle->velocity);
  free(circle->acceleration);
  free(circle->hitbox->centre);
  free(circle->hitbox->bottomRightPoint);
  free(circle->hitbox->topLeftPoint);
  free(circle->hitbox);
  //loop to free path points
  int i = 0;
  while(circle->path != NULL) {
    free(&circle->path[i]);
    i++;
  }  
  free(circle->path);
  free(circle);
}

void moveCircle(Circle* circle[]) {
  int unit_velocity = 3;
  int half_side_length = (circle[0]->radius + 1) / 2;
  int size = sizeof(circle) / sizeof(circle[0]);

  for (int i = 0; i < size; i++) {
    // We need to update the circle's velocity based on boundaries

    if (circle[i]->velocity->x == 0 &&
        circle[i]->velocity->y == unit_velocity) {
      // Circle is moving down
      if (LEVEL1[circle[i]->position->y + half_side_length]
                [circle[i]->position->x] == 0x0000) {
        // Circle hitting bottom boundary; bounce up
        circle[i]->velocity = pointStruct(0, -unit_velocity);
      }

    } else if (circle[i]->velocity->x == 0 &&
               circle[i]->velocity->y == -unit_velocity) {
      // Circle is moving up
      if (LEVEL1[circle[i]->position->y - half_side_length]
                [circle[i]->position->x] == 0x0000) {
        // Circle hitting bottom boundary; bounce down
        circle[i]->velocity = pointStruct(0, unit_velocity);
      }
    }

    // Update the circle's current position
    circle[i]->position = addPoints(circle[i]->position, circle[i]->velocity);
  }
}

void checkForCollisions(Square* square, Circle* circle[]) {
  int size = sizeof(circle) / sizeof(circle[0]);
  for (int i = 0; i < size; i++) {
    if (collided(square, circle[i])) {
      square->position = square->respawn;
      DEATH_COUNT++;
    }
  }
}

bool collided(Square* square, Circle* circle) {
  // Calculate the coordinates of opposite corners of each square
  int square_x1 = square->position->x - ((square->sideLength - 1) / 2);
  int square_x2 = square->position->x + ((square->sideLength - 1) / 2);
  int square_y1 = square->position->y - ((square->sideLength - 1) / 2);
  int square_y2 = square->position->y + ((square->sideLength - 1) / 2);

  int circle_x1 = circle->position->x - circle->radius - 1;  // Left
  int circle_x2 = circle->position->x + circle->radius - 1;  // Right
  int circle_y1 = circle->position->y - circle->radius - 1;  // Top
  int circle_y2 = circle->position->y + circle->radius - 1;  // Bottom

  // Check if squares have collided from any direction
  if (square_x1 <= circle_x2 && square_x2 >= circle_x1 &&
      square_y1 <= circle_y2 && square_y2 >= circle_y1) {
    return true;  // Collision detected
  }

  return false;  // No collision
}

bool checkBoundaryLeft(Square* square) {
  int half_side_length = (square->sideLength - 1) / 2;
  for (int y = square->position->y - half_side_length;
       y <= square->position->y + half_side_length; y++) {
    if (LEVEL1[y][square->position->x - half_side_length - 1] == 0x0000) {
      return false;
    }
  }
  return true;
}

bool checkBoundaryRight(Square* square) {
  int half_side_length = (square->sideLength - 1) / 2;
  for (int y = square->position->y - half_side_length;
       y <= square->position->y + half_side_length; y++) {
    if (LEVEL1[y][square->position->x + half_side_length + 1] == 0x0000) {
      return false;
    }
  }
  return true;
}

bool checkBoundaryUp(Square* square) {
  int half_side_length = (square->sideLength - 1) / 2;
  for (int x = square->position->x - half_side_length;
       x <= square->position->x + half_side_length; x++) {
    if (LEVEL1[square->position->y - half_side_length - 1][x] == 0x0000) {
      return false;
    }
  }
  return true;
}

bool checkBoundaryDown(Square* square) {
  int half_side_length = (square->sideLength - 1) / 2;
  for (int x = square->position->x - half_side_length;
       x <= square->position->x + half_side_length; x++) {
    if (LEVEL1[square->position->y + half_side_length + 1][x] == 0x0000) {
      return false;
    }
  }
  return true;
}

bool checkBoundaryDiagonal(Square* square, int dx, int dy) {
  int half_side_length = ((square->sideLength - 1) / 2) + 1;
  if (LEVEL1[square->position->y + (dy * half_side_length)]
            [square->position->x + (dx * half_side_length)] == 0x0000) {
    return false;
  }
  return true;
}