#include "shapes.h"

extern bool KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT;

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
      if (checkBoundaryRight(square)) square->velocity = pointStruct(unit_velocity, 0);
      else square->velocity = pointStruct(0, 0);
    } else if (KEY_LEFT && !KEY_RIGHT) {
      if (checkBoundaryLeft(square)) square->velocity = pointStruct(-unit_velocity, 0);
      else square->velocity = pointStruct(0, 0);
    } else {
      square->velocity = pointStruct(0, 0);
    }
    // if pushing left and right, and either up or down
  } else if ((KEY_LEFT && KEY_RIGHT) && !(KEY_UP && KEY_DOWN)) {
    if (KEY_UP && !KEY_DOWN) {
      if (checkBoundaryUp(square)) square->velocity = pointStruct(0, -unit_velocity);
      else square->velocity = pointStruct(0, 0);
    } else if (KEY_DOWN && !KEY_UP) {
      if (checkBoundaryDown(square)) square->velocity = pointStruct(0, unit_velocity);
      else square->velocity = pointStruct(0, 0);
    } else {
      square->velocity = pointStruct(0, 0);
    }
    // if pushing all
  } else if (KEY_UP && KEY_DOWN && KEY_RIGHT && KEY_LEFT) {
    square->velocity = pointStruct(0, 0);
  } else {
    // if pushing any 2 non-opposite directions
    if (KEY_UP && KEY_RIGHT) {
      if (!checkBoundaryUp(square) && !checkBoundaryRight(square)) { square->velocity = pointStruct(0, 0);
      } else if (!checkBoundaryUp(square)) { square->velocity = pointStruct(unit_velocity, 0);
      } else if (!checkBoundaryRight(square)) { square->velocity = pointStruct(0, -unit_velocity);
      } else { square->velocity = pointStruct(unit_velocity, -unit_velocity);
      }
    } else if (KEY_RIGHT && KEY_DOWN) {
      if (!checkBoundaryDown(square) && !checkBoundaryRight(square)) { square->velocity = pointStruct(0, 0);
      } else if (!checkBoundaryDown(square)) { square->velocity = pointStruct(unit_velocity, 0);
      } else if (!checkBoundaryRight(square)) { square->velocity = pointStruct(0, unit_velocity);
      } else { square->velocity = pointStruct(unit_velocity, unit_velocity);
      } 
    } else if (KEY_DOWN && KEY_LEFT) {
      if (!checkBoundaryDown(square) && !checkBoundaryLeft(square)) { square->velocity = pointStruct(0, 0);
      } else if (!checkBoundaryDown(square)) { square->velocity = pointStruct(-unit_velocity, 0);
      } else if (!checkBoundaryLeft(square)) { square->velocity = pointStruct(0, unit_velocity);
      } else { square->velocity = pointStruct(-unit_velocity, unit_velocity);
      }
    } else if (KEY_LEFT && KEY_UP) {
      if (!checkBoundaryUp(square) && !checkBoundaryLeft(square)) { square->velocity = pointStruct(0, 0);
      } else if (!checkBoundaryUp(square)) { square->velocity = pointStruct(-unit_velocity, 0);
      } else if (!checkBoundaryLeft(square)) { square->velocity = pointStruct(0, -unit_velocity);
      } else { square->velocity = pointStruct(-unit_velocity, -unit_velocity);
      }
      // if pushing any 1 direction
    } else if (KEY_UP) {
      if (checkBoundaryUp(square)) square->velocity = pointStruct(0, -unit_velocity);
      else square->velocity = pointStruct(0, 0);
    } else if (KEY_RIGHT) {
      if (checkBoundaryRight(square)) square->velocity = pointStruct(unit_velocity, 0);
      else square->velocity = pointStruct(0, 0);
    } else if (KEY_DOWN) {
      if (checkBoundaryDown(square)) square->velocity = pointStruct(0, unit_velocity);
      else square->velocity = pointStruct(0, 0);
    } else if (KEY_LEFT) {
      if (checkBoundaryLeft(square)) square->velocity = pointStruct(-unit_velocity, 0);
      else square->velocity = pointStruct(0, 0);
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
Circle* circleStruct(point* position, int radius, point** path) {
  // create points
  Circle* newCircle;
  point* pos = position;
  point* vel = pointStruct(0, 0);
  point* acc = pointStruct(0, 0);
  int hitboxRadius = radius - 2;
  circleHitbox* newHitbox = circleHitboxStruct(position, hitboxRadius);

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

bool checkBoundaryLeft(Square* square) {
  int half_side_length = ((square->sideLength - 1) / 2) + 1;
  for (int y = square->position->y - half_side_length; y < square->position->y + half_side_length; y++) {
    if (LEVEL1[y][square->position->x - half_side_length] ==
        0x0000) {
      return false;
    }
  }
  return true;
}

bool checkBoundaryRight(Square* square) {
  int half_side_length = ((square->sideLength - 1) / 2) + 1;
  for (int y = square->position->y - half_side_length; y < square->position->y + half_side_length; y++) {
    if (LEVEL1[y][square->position->x + half_side_length] ==
        0x0000) {
      return false;
    }
  }
  return true;
}

bool checkBoundaryUp(Square* square) {
  int half_side_length = ((square->sideLength - 1) / 2) + 1;
  for (int x = square->position->x - half_side_length; x < square->position->x + half_side_length; x++) {
    if (LEVEL1[square->position->y - half_side_length][x] ==
        0x0000) {
      return false;
    }
  }
  return true;
}

bool checkBoundaryDown(Square* square) {
  int half_side_length = ((square->sideLength - 1) / 2) + 1;
  for (int x = square->position->x - half_side_length; x < square->position->x + half_side_length; x++) {
    if (LEVEL1[square->position->y + half_side_length][x] ==
        0x0000) {
      return false;
    }
  }
  return true;
}

/* bool checkBoundaryDiagonal(Square* square, int dx, int dy) {
  int half_side_length = ((square->sideLength - 1) / 2) + 1;
  if (LEVEL1[square->position->y + (dy * half_side_length)][square->position->x + (dx * half_side_length)] ==
      0x0000) {
    return false;
  }
  return true;
} */