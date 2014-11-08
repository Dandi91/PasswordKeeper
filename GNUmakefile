# User-defined section (flags, compiler, include paths, libraries)
CXXFLAGS = -Wall -std=gnu++11
CXX = g++
INC = inc /usr/include/cryptopp
LIB = cryptopp
BIN = pkeep
WXTOOLKIT = gtk3
WXVERSION = 3.0

# Builds
ifneq ($(STATIC),yes)
override STATIC = no
endif

ifneq ($(BUILD),debug)
override BUILD = release
CXXFLAGS := $(CXXFLAGS) -O2
LDFLAGS = -s
WXDEBUG = no
else
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

# Sources from
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

$(OBJPATH) $(BINPATH):
	mkdir -p $@

$(BIN) : $(OBJPATH) $(BINPATH) $(OBJS)
	$(CXX) -o $(BINPATH)/$(BIN) $(OBJRES) $(LDFLAGS) $(WXLIBS) $(LIB)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(WXFLAGS) $(INC) -c $< -o $(OBJPATH)/$@
