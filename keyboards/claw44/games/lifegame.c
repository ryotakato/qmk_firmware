#include "lifegame.h"

#include <string.h>

#include "timer.h"


void lifegame_init(Lifegame* lifegame) {
  uint16_t t = timer_read();
  xorshift_init(&lifegame->xorshift, t, (t >> 8), 3, 4);

  lifegame->gamestate = 0;
  lifegame->lasttime = 0;

  lifegame->debugval = sizeof(int);
  lifegame->speed = 1000;
  lifegame->initialized = true;

  // 初期化
  memset(lifegame->blocks, 0, sizeof(lifegame->blocks));
  memset(lifegame->backup, 0, sizeof(lifegame->backup));

  // 初期はランダム配置
  for (int y = 0; y < LifegameHeight; y++) {
    for (int x = 0; x < LifegameWidth; x++) {
      lifegame->blocks[y][x] = xorshift_next(&lifegame->xorshift) % 2;
    }
  }
  // 終わったらbackupにコピーしておく
  memcpy(lifegame->backup, lifegame->blocks, sizeof(lifegame->backup));


}

void lifegame_reset(Lifegame* lifegame) {
  lifegame->gamestate = 1;
}

void lifegame_update(Lifegame* lifegame) {
  if (!lifegame->initialized) {
    lifegame_init(lifegame);
  }

  if (lifegame->gamestate == 0) {
    uint16_t time = timer_read();
    if (time - lifegame->lasttime < lifegame->speed) {
      return;
    }
    lifegame->lasttime = time;



    uint8_t tmp;

    // 初期化
    memset(lifegame->blocks, 0, sizeof(lifegame->blocks));


    int maxH = LifegameHeight - 1;
    int maxW = LifegameWidth - 1;

    // 次の世代を算出
    for (int y = 0; y < LifegameHeight; y++) {
      for (int x = 0; x < LifegameWidth; x++) {
        lifegame->blocks[y][x] = xorshift_next(&lifegame->xorshift) % 2;

        tmp = lifegame->backup[y-1<0?maxH:y-1][x             ] +
              lifegame->backup[y+1>maxH?0:y+1][x             ] +
              lifegame->backup[y             ][x-1<0?maxW:x-1] +
              lifegame->backup[y             ][x+1>maxW?0:x+1] +
              lifegame->backup[y-1<0?maxH:y-1][x-1<0?maxW:x-1] +
              lifegame->backup[y-1<0?maxH:y-1][x+1>maxW?0:x+1] +
              lifegame->backup[y+1>maxH?0:y+1][x-1<0?maxW:x-1] +
              lifegame->backup[y+1>maxH?0:y+1][x+1>maxW?0:x+1];

        if (lifegame->backup[y][x] == 1) {
          lifegame->blocks[y][x] = (tmp==2 || tmp==3);
        } else {
          lifegame->blocks[y][x] = (tmp==3);
        }
      }
    }

    // 終わったらbackupにコピーしておく
    memcpy(lifegame->backup, lifegame->blocks, sizeof(lifegame->backup));

  } else {
    uint16_t time = timer_read();
    if (time - lifegame->lasttime < 2000) {
      return;
    }
    lifegame->lasttime = time;

    // init
    lifegame_init(lifegame);
  }
}

void lifegame_render(Lifegame* lifegame, ScreenMatrix* matrix) {
  for (int y = 0; y < LifegameHeight; y++) {
    for (int x = 0; x < LifegameWidth; x++) {
      if (lifegame->blocks[y][x] != 0) {
        screen_draw_rect(matrix, x * 3, y * 3, 2, 2, 0);
      }
    }
  }

}
