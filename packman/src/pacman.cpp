#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAZE_WIDTH 21
#define MAZE_HEIGHT 27
#define INVISIBLE_TIME_MAX 40

char blueprint[] = "\
#####################\
#ooooooooo#ooooooooo#\
#o###o###o#o###o###o#\
#O# #o# #o#o# #o# #O#\
#o###o###o#o###o###o#\
#ooooooooooooooooooo#\
#o###o#o#####o#o###o#\
#o###o#o#####o#o###o#\
#ooooo#ooo#ooo#ooooo#\
#####o### # ###o#####\
    #o#   r   #o#    \
    #o# ##### #o#    \
#####o# #bpy# #o#####\
     o  #   #  o     \
#####o# ##### #o#####\
#ooooooooo#ooooooooo#\
#o###o###o#o###o###o#\
#Ooo#oooooiooooo#ooO#\
###o#o#o#####o#o#o###\
###o#o#o#####o#o#o###\
#ooooo#ooo#ooo#ooooo#\
#o#######o#o#######o#\
#ooooooooooooooooooo#\
#####################";

int map[MAZE_HEIGHT][MAZE_WIDTH];
int invisibleTime = 0;

enum {
  CELL_TYPE_NONE,
  CELL_TYPE_WALL,
  CELL_TYPE_DOT,
  CELL_TYPE_POWER,
  CELL_TYPE_MAX
};

enum {
  DIRECTION_NORTH,
  DIRECTION_WEST,
  DIRECTION_SOUTH,
  DIRECTION_EAST,
  DIRECTION_MAX
};

enum {
  MONSTER_TYPE_PAC,
  MONSTER_TYPE_RED,
  MONSTER_TYPE_BLUE,
  MONSTER_TYPE_PINK,
  MONSTER_TYPE_ORANGE,
  MONSTER_TYPE_MAX
};

char cellAA[][2 + 1] = {
    "Å@", // CELL_TYPE_NONE
    "Å°",  // CELL_TYPE_WALL
    "ÅE", // CELL_TYPE_DOT
    "Åô"   // CELL_TYPE_POWER
};

char monsterAA[][2 + 1] = {
    "Åó", // MONSTER_TYPE_PAC
    "ê‘", // MONSTER_TYPE_RED
    "ê¬", // MONSTER_TYPE_BLUE
    "ìç", // MONSTER_TYPE_PINK
    "ûÚ"  // MONSTER_TYPE_ORANGE
};

int directions[][2] = {
    {0, -1}, // DIRECTION_NORTH
    {-1, 0}, //  DIRECTION_WEST
    {0, 1},  //  DIRECTION_SOUTH
    {1, 0},  //  DIRECTION_EAST
};

typedef struct {
  int x, y;
  int direction;
} MONSTER;

MONSTER monsters[MONSTER_TYPE_MAX];

void initializeMap() {

  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      char cellPattern = blueprint[MAZE_WIDTH * y + x];
      int cell = CELL_TYPE_NONE;

      switch (cellPattern) {
      case '#':
        cell = CELL_TYPE_WALL;
        break;
      case 'o':
        cell = CELL_TYPE_DOT;
        break;
      case 'O':
        cell = CELL_TYPE_POWER;
        break;

      default: {
        int monster = -1;
        switch (cellPattern) {
        case 'i':
          monster = MONSTER_TYPE_PAC;
          break;
        case 'r':
          monster = MONSTER_TYPE_RED;
          break;
        case 'b':
          monster = MONSTER_TYPE_BLUE;
          break;
        case 'p':
          monster = MONSTER_TYPE_PINK;
          break;
        case 'y':
          monster = MONSTER_TYPE_ORANGE;
          break;
        }

        if (monster >= 0) {
          monsters[monster].x = x;

          monsters[monster].y = y;
        }
      } break;
      }
      map[y][x] = cell;
    }
  }
}
int getMonster(int _x, int _y) {

  for (int i = 0; i < MONSTER_TYPE_MAX; i++) {
    if ((monsters[i].x == _x) && (monsters[i].y == _y)) {
      return i;
    }
  }
  return -1;
}

void display() {

  system("cls");
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      int monster = getMonster(x, y);
      if (monster >= 0) {
        if ((monster > MONSTER_TYPE_PAC) && (invisibleTime > 0)) {
          printf("%s", (invisibleTime < INVISIBLE_TIME_MAX / 4) ? "Å¢" : "Å¸");
        } else {
          printf(monsterAA[monster]);
        }
      } else {
        printf(cellAA[map[y][x]]);
      }
    }
    printf("\n");
  }
}

int getDotCount() {

  int dotCount = 0;
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      switch (map[y][x]) {
      case CELL_TYPE_DOT:
      case CELL_TYPE_POWER:
        dotCount++;
        break;
      }
    }
  }
  return dotCount;
}

void gameOver() {

  monsters[MONSTER_TYPE_PAC].x = -1;
  monsters[MONSTER_TYPE_PAC].y = -1;

  display();
  printf("GAME OVER\a");
  _getch();
  exit(0);
}

void gameClear() {

  monsters[MONSTER_TYPE_PAC].x = -1;
  monsters[MONSTER_TYPE_PAC].y = -1;

  printf("CLEAR!\a");
  display();
  _getch();
  exit(0);
}

int main() {

  initializeMap();

  srand((unsigned int)time(NULL));
  int dotCountDefault = getDotCount();

  while (1) {
    display();

    int x = monsters[MONSTER_TYPE_PAC].x;
    int y = monsters[MONSTER_TYPE_PAC].y;

    switch (_getch()) {
    case 'w':
      y--;
      break;
    case 's':
      y++;
      break;
    case 'a':
      x--;
      break;
    case 'd':
      x++;
      break;
    }

    x = (MAZE_WIDTH + x) % MAZE_WIDTH;

    if (getMonster(x, y) > MONSTER_TYPE_PAC) {
      if (invisibleTime > 0) {
        int monster = getMonster(x, y);
        monsters[monster].x = 10;
        monsters[monster].y = 10;
      } else {
        gameOver();
      }
    }

    if (map[y][x] == CELL_TYPE_WALL) {
      // ignore

    } else {
      if (map[y][x] == CELL_TYPE_POWER) {

        invisibleTime = INVISIBLE_TIME_MAX;
      }

      map[y][x] = CELL_TYPE_NONE;

      int dotCount = getDotCount();
      int monster = -1;

      if (dotCount <= 0) {

        gameClear();

      } else if (dotCount == dotCountDefault * 3 / 4) {

        monster = MONSTER_TYPE_BLUE;
      } else if (dotCount == dotCountDefault * 2 / 4) {

        monster = MONSTER_TYPE_PINK;
      } else if (dotCount == dotCountDefault * 1 / 4) {

        monster = MONSTER_TYPE_ORANGE;
      }

      if (monster >= 0) {

        monsters[monster].x = 10;
        monsters[monster].y = 10;
      }

      monsters[MONSTER_TYPE_PAC].x = x;
      monsters[MONSTER_TYPE_PAC].y = y;
    }

    for (int i = MONSTER_TYPE_PAC + 1; i < MONSTER_TYPE_MAX; i++) {
      int x = monsters[i].x + directions[monsters[i].direction][0];
      int y = monsters[i].y + directions[monsters[i].direction][1];

      int monster = getMonster(x, y);

      if (monster == MONSTER_TYPE_PAC) {
        if (invisibleTime > 0) {
          monsters[monster].x = 10;
          monsters[monster].y = 10;
        } else {
          monsters[i].x = x;
          monsters[i].y = y;
          gameOver();
        }
      }

      if (map[y][x] == CELL_TYPE_WALL || monster > MONSTER_TYPE_PAC) {
        monsters[i].direction = rand() % DIRECTION_MAX;
      } else {
        monsters[i].x = x;
        monsters[i].y = y;
      }
    }

    if (invisibleTime >= 1) {

      invisibleTime--;
    }
  }
  return 1;
}
