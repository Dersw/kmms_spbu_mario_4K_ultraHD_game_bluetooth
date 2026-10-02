#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace Config {
    inline const int MAP_WIDTH = 150;
    inline const int MAP_HEIGHT = 30;
    inline const int SCORE_X_OFFSET = 4;
    inline const int SCORE_AREA_WIDTH = 50;

    inline const int FRAME_DELAY_MS = 5;
    inline const int RESTART_DELAY_MS = 2000;
    inline const char COLOR_DEFAULT[] = "color 1F";
    inline const char COLOR_LOSE[] = "color 4F";
    inline const char COLOR_WIN[] = "color 2F";

    inline const float GRAVITY = 0.4f;
    inline const float JUMP_FORCE = -3.1f;
    inline const float ENEMY_SPEED = 0.5f;
	inline const float COIN_SPEED = 0.3f;
    inline const float MARIO_START_X = 15.0f;
    inline const float MARIO_START_Y = 10.0f;

    inline const int SCORE_FOR_KILL = 200;
    inline const int SCORE_FOR_COIN = 100;

    inline const char TYPE_MARIO = 'M';
    inline const char TYPE_BRICK = '#';
    inline const char TYPE_BOX   = '?';
    inline const char TYPE_LINES = '=';
    inline const char TYPE_ENEMY = 'E';
    inline const char TYPE_COIN  = 'o';
    inline const char TYPE_EXIT  = 'X';
}

#endif