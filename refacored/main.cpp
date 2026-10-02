#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

static const float FRICTION = 0.9f;
static const float GRAVITY = 0.2f;
static const float JUMP_POWER = -2.2f;
static const int MAX_LEVEL = 3;

static const int MAP_WIDTH = 80;
static const int MAP_HEIGHT = 25;

static const char COLOR_WIN[] = "COLOR 2F";
static const char COLOR_LOSE[] = "COLOR 4F";
static const char COLOR_DEFAULT[] = "COLOR 9F";

static const char TYPE_BRICK = '#';
static const char TYPE_COIN = '$';
static const char TYPE_ENEMY = '^';
static const char TYPE_EXIT = '+';
static const char TYPE_MARIO = '@';
static const char TYPE_BOX = '?';
static const char TYPE_LINES = '-';


typedef struct SObject{
    float x, y;
    float width, height;
    float vertical_speed;
    float horizontal_speed;
    bool is_flying;
    char object_type;
} Object;

char map[MAP_HEIGHT][MAP_WIDTH+1];
float camera_x = 0.0f;

Object mario;
Object *brick = NULL;
Object *moving_objects = NULL;

int brick_counts = 0;
int moving_objects_count;


int current_level = 1;
int player_score = 0;

void ClearMap(){
    for (int i = 0; i < MAP_WIDTH; i++)
        map[0][i] = ' ';
    map[0][MAP_WIDTH] = '\0';
    for (int j = 1; j < MAP_HEIGHT; j++)
        sprintf(map[j], map[0]);
}

void ShowMap(){
    map[MAP_HEIGHT - 1][MAP_WIDTH - 1]= '\0';
    for (int j = 0; j < MAP_HEIGHT; j++)
        printf("%s\n", map[j]);
}


void SeObjectPos(Object *obj, float xPos, float yPos){
    (*obj).x = xPos;
    (*obj).y = yPos;
}

void IniObject(Object *obj, float xPos, float yPos, float oWidth, float oHeight, char inType){
    SeObjectPos(obj, xPos, yPos);
    (*obj).width = oWidth;
    (*obj).height = oHeight;
    (*obj).vertical_speed = 0;
    (*obj).object_type = inType;
    (*obj).horizontal_speed = FRICTION;
}
;
void CreateLevel(int lvl);

void PlayerDead()
{
    system(COLOR_LOSE);
    Sleep(500);
    CreateLevel(current_level);
}

bool IsCollision(Object o1, Object o2);
Object *GetNewmoving_objects();

void VertMoveObject(Object *obj)
{
    (*obj).is_flying = true;
    (*obj).vertical_speed += GRAVITY;
    SeObjectPos(obj, (*obj).x, (*obj).y + (*obj).vertical_speed);

    for (int i = 0; i < brick_counts; i++)
        if (IsCollision(*obj, brick[i]))
        {
            if (obj[0].vertical_speed > 0)
                obj[0].is_flying = false;

            if ( (brick[i].object_type == TYPE_BOX) && (obj[0].vertical_speed < 0) && (obj == &mario) )
            {
                brick[i].object_type = '-';
                IniObject(GetNewmoving_objects(), brick[i].x, brick[i].y-3, 3, 2, TYPE_COIN);
                moving_objects[moving_objects_count - 1].vertical_speed = -0.7;
            }
            (*obj).y -= (*obj).vertical_speed;
            (*obj).vertical_speed = 0;

            if(brick[i].object_type == TYPE_EXIT)
            {
                current_level++;
                if (current_level > MAX_LEVEL) current_level = 1;

                system(COLOR_WIN);
                Sleep(500);
                CreateLevel(current_level);
            }

            break;
        }
}

void Deletemoving_objects(int i)
{
    moving_objects_count--;
    moving_objects[i] = moving_objects[moving_objects_count];

    if(moving_objects_count == 0)
    {
        free(moving_objects);
        moving_objects = NULL;
    }
    else
    {
        moving_objects = (Object*)realloc(moving_objects, sizeof(*moving_objects) * moving_objects_count);
    }
}

void MarioCollision()
{
    for(int i = 0; i < moving_objects_count; i++)
        if (IsCollision(mario, moving_objects[i]))
        {
            if(moving_objects[i].object_type == TYPE_ENEMY)
            {
                if ( (mario.is_flying == TRUE)
                    && (mario.vertical_speed > 0)
                    && (mario.y + mario.height < moving_objects[i].y + moving_objects[i].height * 0.5)
                )
                {
                    player_score += 50;
                    Deletemoving_objects(i);
                    i--;
                    continue;
                }
                else
                    PlayerDead();
            }

            if(moving_objects[i].object_type == TYPE_COIN)
            {
                player_score += 100;
                Deletemoving_objects(i);
                i--;
                continue;
            }
        }
}

void HorizonMoveObj(Object *obj)
{
    (*obj).x += (*obj).horizontal_speed;

    for (int i = 0; i < brick_counts; i++)
        if (IsCollision(obj[0], brick[i]))
        {
            obj[0].x -= obj[0].horizontal_speed;
            obj[0].horizontal_speed = -obj[0].horizontal_speed;
            return;
        }

        if (obj[0].object_type == TYPE_ENEMY)
        {
            Object tmp = *obj;
            VertMoveObject(&tmp);
            if(tmp.is_flying == TRUE)
            {
                obj[0].x -= obj[0].horizontal_speed;
                obj[0].horizontal_speed = -obj[0].horizontal_speed;
            }
        }
}

bool IsPositionMap(int x, int y)
{
    return ( (x >= 0) && (x < MAP_WIDTH) && (y >= 0) && (y < MAP_HEIGHT));
}

void PuObjectOnMap(Object obj){

    int ix = (int)round(obj.x - camera_x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);

    for(int i = ix; i < (ix + iWidth); i++)
        for (int j = iy; j < (iy + iHeight); j++)
            if (IsPositionMap(i, j))
                map[j][i] = obj.object_type;
}

void setCur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}



void HorizonMoveMap(float dx)
{
    float oldX = mario.x;
    mario.x -= dx;

    for (int i = 0; i < brick_counts; i++)
        if (IsCollision(mario, brick[i]))
        {
            mario.x = oldX;
            return;
        }

        camera_x = mario.x - MAP_WIDTH / 2;

    if (camera_x < 0) camera_x = 0;

    for(int i = 0; i < brick_counts; i++)
        brick[i].x += dx;
    for(int i = 0; i < moving_objects_count; i++)
        moving_objects[i].x += dx;

}

bool IsCollision(Object o1, Object o2)
{
    return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) && ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height));
}

Object *GetNewBrick()
{
    brick_counts++;
    brick = (Object*)realloc(brick, sizeof(*brick) * brick_counts);
    return brick + brick_counts - 1;
}

Object *GetNewmoving_objects()
{
    moving_objects_count++;
    moving_objects = (Object*)realloc(moving_objects, sizeof(*brick) * moving_objects_count);
    return moving_objects + moving_objects_count - 1;
}

void PutScoreOnMap()
{
    char c[30];
    sprintf(c, "player_score: %d", player_score);
    int len = strlen(c);
    for(int i = 0; i < len; i++)
    {
        map[1][i+5] = c[i];
    }
}

void CreateLevel(int lvl)
{
    system(COLOR_DEFAULT);
    if (brick != NULL)
    {
        free(brick);
        brick = NULL;
    }
    brick_counts = 0;

    if (moving_objects != NULL)
    {
        free(moving_objects);
        moving_objects = NULL;
    }
    moving_objects_count = 0;

    IniObject(&mario, 39, 10, 3, 3, TYPE_MARIO);
    player_score = 0;

    camera_x = 0;

    if(lvl == 1)
    {
        IniObject(GetNewBrick(), 20, 20, 40, 5, TYPE_BRICK);
        IniObject(GetNewBrick(), 30, 10, 5, 3, TYPE_BOX);
        IniObject(GetNewBrick(), 50, 10, 5, 3, TYPE_BOX);
        IniObject(GetNewBrick(), 60, 15, 40, 10, TYPE_BRICK);
        IniObject(GetNewBrick(), 60, 5, 10, 3, TYPE_LINES);
        IniObject(GetNewBrick(), 70, 5, 5, 3, TYPE_BOX);
        IniObject(GetNewBrick(), 75, 5, 5, 3, TYPE_LINES);
        IniObject(GetNewBrick(), 80, 5, 5, 3, TYPE_BOX);
        IniObject(GetNewBrick(), 80, 5, 10, 3, TYPE_LINES);
        IniObject(GetNewBrick(), 100, 20, 20, 5, TYPE_BRICK);
        IniObject(GetNewBrick(), 120, 15, 10, 10, TYPE_BRICK);
        IniObject(GetNewBrick(), 150, 20, 40, 5, TYPE_BRICK);
        IniObject(GetNewBrick(), 210, 15, 10, 10, TYPE_EXIT);

        IniObject(GetNewmoving_objects(), 25, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 80, 10, 3, 2, TYPE_ENEMY);


    }

    if(lvl == 2)
    {
        IniObject(GetNewBrick(), 20, 20, 40, 5, TYPE_BRICK);
        IniObject(GetNewBrick(), 30, 10, 5, 3, TYPE_BOX);
        IniObject(GetNewBrick(), 50, 10, 5, 3, TYPE_BOX);
        IniObject(GetNewBrick(), 60, 15, 10, 10, TYPE_BRICK);
        IniObject(GetNewBrick(), 80, 20, 20, 5, TYPE_BRICK);
        IniObject(GetNewBrick(), 120, 15, 10, 10, TYPE_BRICK);
        IniObject(GetNewBrick(), 122, 5, 5, 3, TYPE_BOX);
        IniObject(GetNewBrick(), 150, 20, 40, 5, TYPE_BRICK);
        IniObject(GetNewBrick(), 210, 15, 10, 10, TYPE_EXIT);

        IniObject(GetNewmoving_objects(), 25, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 80, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 65, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 120, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 160, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 175, 10, 3, 2, TYPE_ENEMY);

        IniObject(GetNewmoving_objects(), 25, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 80, 10, 3, 2, TYPE_ENEMY);
    }

    if(lvl == 3)
    {
        IniObject(GetNewBrick(), 20, 20, 40, 5, TYPE_BRICK);
        IniObject(GetNewBrick(), 80, 20, 15, 5, TYPE_BRICK);
        IniObject(GetNewBrick(), 120, 15, 15, 10, TYPE_BRICK);
        IniObject(GetNewBrick(), 160, 10, 15, 15, TYPE_EXIT);

        IniObject(GetNewmoving_objects(), 25, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 50, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 80, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 90, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 120, 10, 3, 2, TYPE_ENEMY);
        IniObject(GetNewmoving_objects(), 130, 10, 3, 2, TYPE_ENEMY);
    }

    MAX_LEVEL;
}

int main()
{
    CreateLevel(current_level);

    do{
        ClearMap();

        if ( (mario.is_flying == false) && (GetKeyState(VK_SPACE) < 0) ) mario.vertical_speed = JUMP_POWER;
        if (GetKeyState('A') < 0) HorizonMoveMap(1);
        if (GetKeyState('D') < 0) HorizonMoveMap(-1);

        if (MAP_HEIGHT < mario.y || mario.y < 0) PlayerDead();

        VertMoveObject(&mario);
        MarioCollision();

        for(int i = 0; i < brick_counts; i++)
            PuObjectOnMap(brick[i]);
        for(int i = 0; i < moving_objects_count; i++)
        {
            VertMoveObject(moving_objects + i);
            HorizonMoveObj(moving_objects + i);
            PuObjectOnMap(moving_objects[i]);
        }

        PuObjectOnMap(mario);
        PutScoreOnMap();

        setCur(0, 0);
        ShowMap();

        Sleep(10);
    }
    while(GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}
