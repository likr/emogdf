SRCDIR = src
OBJDIR = build

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
CXX_OPTIONS := --bind -O3 -Iinclude -DNDEBUG

demo/emogdf.js: $(OBJECTS)
	em++ $(CXX_OPTIONS) -o $@ $(OBJECTS) libOGDF.a libCOIN.a

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	em++ $(CXX_OPTIONS) -c $< -o $@
