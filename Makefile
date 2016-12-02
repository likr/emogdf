SRCDIR = src
OBJDIR = build

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
CXX_OPTIONS := --bind -O3 -Iinclude -DNDEBUG

emogdf.js: $(OBJECTS)
	em++ $(CXX_OPTIONS) --memory-init-file 0 --pre-js js/pre.js --post-js js/post.js -o $@ $(OBJECTS) libOGDF.a libCOIN.a

demo: emogdf.js
	cp emogdf.js demo

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	em++ $(CXX_OPTIONS) -c $< -o $@
