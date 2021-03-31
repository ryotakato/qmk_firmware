#include <stdbool.h>

#include "xorshift.h"
#include "screen.h"


#define LifegameHeight 40
#define LifegameWidth 10

struct Lifegame {
  uint8_t blocks[LifegameHeight][LifegameWidth];
  uint8_t backup[LifegameHeight][LifegameWidth];
  uint8_t gamestate;
  uint16_t lasttime;
  uint16_t speed;
  int debugval;
  Xorshift xorshift;
  bool initialized;
};
typedef struct Lifegame Lifegame;

extern void lifegame_init(Lifegame* lifegame);
extern void lifegame_update(Lifegame* lifegame);
extern void lifegame_render(Lifegame* lifegame, ScreenMatrix* matrix);

extern void lifegame_reset(Lifegame* lifegame);
