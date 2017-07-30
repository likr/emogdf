SRCDIR = src
OBJDIR = build

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
CXX_OPTIONS := --bind -O3 -Iogdf/include -Iogdf-build/include -DNDEBUG

all: emogdf-asmjs.js emogdf-wasm.js

emogdf-asmjs.js: $(OBJECTS)
	em++ $(CXX_OPTIONS) --memory-init-file 0 --pre-js js/pre.js --post-js js/post.js -o $@ $(OBJECTS) ogdf-build/libOGDF.a ogdf-build/libCOIN.a

emogdf-wasm.js: $(OBJECTS)
	em++ $(CXX_OPTIONS) -s WASM=1 --memory-init-file 0 --pre-js js/pre.js --post-js js/post.js -o $@ $(OBJECTS) ogdf-build/libOGDF.a ogdf-build/libCOIN.a

demo: emogdf-asmjs.js
	cp emogdf-asmjs.js demo/emogdf.js

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	em++ $(CXX_OPTIONS) -c $< -o $@
