#include <emscripten/bind.h>

void defineBasic();
void defineEnergybased();
void defineFileformats();
void defineMisclayout();
void definePacking();
void definePlanarity();
void defineTree();
void defineUpward();

EMSCRIPTEN_BINDINGS(OGDF) {
  defineBasic();
  defineEnergybased();
  defineFileformats();
  defineMisclayout();
  definePacking();
  definePlanarity();
  defineTree();
  defineUpward();
}
