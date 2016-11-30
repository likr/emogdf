#include <emscripten/bind.h>

void defineBasic();
void defineEnergybased();
void defineFileformats();

EMSCRIPTEN_BINDINGS(OGDF) {
  defineBasic();
  defineEnergybased();
  defineFileformats();
}
