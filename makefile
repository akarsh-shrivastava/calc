TARGET = calc


FX        =     flex++
CX        =     g++
LD        =     g++

INCLUDES  =     include
CXOBJ     =     .obj
CXSRC     =     src/cpp

FXSRC     =     src/flex
FXTAR     =     src/cpp

FXSOURCE  =     $(FXSRC)/*.l
FXTARGET  =     $(FXTAR)/lexer.cpp

HEADERS   =     $(INCLUDES)/*.h
CXSOURCE  =     $(CXSRC)/*.cpp
CXOBJECT  =     $(CXOBJ)/lexer.o \
                $(CXOBJ)/segments.o \
                $(CXOBJ)/dsparser.o \
                $(CXOBJ)/csparser.o \
                $(CXOBJ)/main.o

CXFLAGS   =     -c -I$(INCLUDES) -fPIC -O3 -std=c++17 -gdwarf


$(TARGET): $(CXOBJECT)
	$(LD) $^ -o $@ 

$(CXOBJ)/%.o: $(CXSRC)/%.cpp $(HEADERS)
	$(CX) $(CXFLAGS) $< -o $@

$(FXTAR)/%.cpp: $(FXSRC)/%.l $(HEADERS)
	$(FX) -o$@ $<
