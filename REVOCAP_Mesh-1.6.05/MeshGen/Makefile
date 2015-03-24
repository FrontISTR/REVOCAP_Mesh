include ../OPTIONS
include ../MakefileConfig.in

LIBDIR = ../lib
STATIC_LIB = libRcapMeshGen.a

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

static : $(LIBDIR)/$(ARCH)/$(STATIC_LIB)

$(LIBDIR)/$(ARCH)/$(STATIC_LIB) : $(OBJS)
	@if [ ! -d $(LIBDIR) ]; then mkdir $(LIBDIR); fi;
	@if [ ! -d $(LIBDIR)/$(ARCH) ]; then mkdir $(LIBDIR)/$(ARCH); fi;
	$(AR) $(ARFLAGS) $(LIBDIR)/$(ARCH)/$(STATIC_LIB) $(OBJS)

.PHONY: clean
clean:
	@-$(RM) $(LIBDIR)/$(ARCH)/$(STATIC_LIB)
	@-$(RM) *.o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -I.. $<
