
TARGET = dynamo

CXX = clang++ -stdlib=libc++ -std=c++11
LD = $(CXX)
PROFFLAGS = 
CXXOPTFLAGS = -O2
CXXFLAGS += $(PROFFLAGS) $(CXXOPTFLAGS) -I.
LDFLAGS += $(PROFFLAGS)

DOXYGEN = doxygen

CXXSRC = main.cc box.cc particle.cc vector3.cc random.cc \
boltzmann.cc potential.cc integrator.cc

CXXOBJ = $(CXXSRC:%.cc=%.o)

$(TARGET) : $(CXXOBJ)
	$(LD) -o $(TARGET) $(LDFLAGS) $(LDLIBS) $^

.PHONY: doc
.PHONY: clean distclean

doc:
	$(DOXYGEN) doxygen.cfg

clean:
	-rm -rf *.o $(TARGET) *~

distclean:
	-rm -rf html

