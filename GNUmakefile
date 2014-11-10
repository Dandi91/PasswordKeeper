# User-defined section (flags, compiler, include paths, libraries)
CXXFLAGS = -Wall -std=gnu++11
LDFLAGS =
CXX = g++
INC = inc /usr/include/cryptopp
# Name for crypto++ library
LIB = cryptopp
# Name for the output binary
BIN = pkeep
# Features of your wxWidgets installation (toolkit gtk3/gtk2, version 3.0/2.8)
WXTOOLKIT = gtk3
WXVERSION = 3.0

# Builds
# Static/shared build
ifneq ($(STATIC),yes)
# Shared build
override STATIC = no
endif

# Debug/release build
ifneq ($(BUILD),debug)
# Release build
override BUILD = release
CXXFLAGS := $(CXXFLAGS) -O2
LDFLAGS := $(LDFLAGS) -s
WXDEBUG = no
else
# Debug build
CXXFLAGS := $(CXXFLAGS) -g
WXDEBUG = yes
endif

# Paths
OBJPATH = obj/$(BUILD)
BINPATH = bin/$(BUILD)

# wx-config output
WXCONFIG = wx-config --version=$(WXVERSION) --toolkit=$(WXTOOLKIT) --static=$(STATIC) --unicode=yes --debug=$(WXDEBUG)
WXFLAGS = $(shell $(WXCONFIG) --cflags)
WXLIBS = $(shell $(WXCONFIG) --libs)

# Includes
INC := $(patsubst %,-I%,$(INC))

# Libs
LIB := $(patsubst %,-l%,$(LIB))

vpath %.cpp src
vpath %.o $(OBJPATH)
vpath $(BIN) $(BINPATH)

# Sources
SRCS = $(patsubst src/%,%,$(wildcard src/*.cpp))

# Object files
OBJS = $(SRCS:.cpp=.o)
OBJRES = $(patsubst %,$(OBJPATH)/%,$(OBJS))

.PHONY: all clean delhist install remove

all: $(BIN)

install:
	cp -f $(BINPATH)/$(BIN) /usr/local/bin/

remove:
	rm /usr/local/bin/$(BIN)

delhist:
	find -type f -name *~* -exec rm -i {} \;

clean:
	rm -rf $(BINPATH) $(OBJPATH)

$(OBJPATH) $(BINPATH) :
	mkdir -p $@

$(BIN) : $(OBJPATH) $(BINPATH) $(OBJS)
	$(CXX) -o $(BINPATH)/$(BIN) $(OBJRES) $(LDFLAGS) $(WXLIBS) $(LIB)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(WXFLAGS) $(INC) -c $< -o $(OBJPATH)/$@
