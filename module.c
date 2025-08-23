#include "ui_test.c"

// TODO make the cradle layer for entry points and stuff

void *module_init(void*);
void *module_close(void*);
void *module_main(void*);
void *module_load(void*);
void *module_unload(void*);

void *module_init(void* _) {

  return (void*)game_init();

}

void *module_close(void* gp) {

  game_close(gp);
  return 0;

}

void *module_main(void* _gp) {

  Game *gp = _gp;
  game_update_and_draw(gp);

  if(gp->quit) {
    return (void*)0;
  } else {
    return (void*)1;
  }

}

void *module_load(void* _gp) {

  Game *gp = _gp;

  //Clay_SetCurrentContext(gp->clay_context);

  os_state_select(gp->os_state);
  ui_state_select(gp->ui_state);

  game_load_assets(gp);

  return 0;

}

void *module_unload(void* gp) {

  game_unload_assets((Game*)gp);
  return 0;

}
