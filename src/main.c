#include <stdio.h>
#include "pd_api.h"
#include "game.h"

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI *pd, PDSystemEvent event, uint32_t arg) {
    (void) arg; // arg is currently only used for event = kEventKeyPressed

    if (event == kEventInit) {
        pd->system->setUpdateCallback(game_update, pd);
        game_init(pd);
    }

    return 0;
}


