#include <emscripten/bind.h>
#include <ogdf/fileformats/GraphIO.h>

using namespace emscripten;

std::string getSVG(const ogdf::GraphAttributes &A) {
  std::stringstream os;
  ogdf::GraphIO::drawSVG(A, os);
  std::string str;
  str = os.str();
  return str;
}

void defineFileformats () {
  class_<ogdf::GraphIO>("GraphIO")
    .constructor()
    .class_function("getSVG", getSVG)
    ;
}
