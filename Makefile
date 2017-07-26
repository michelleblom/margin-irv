PROGRAM = marginirv

RM = rm -rf
OBJDIR = obj

BASEDIRS = \
	-I. \
	-I$(CPLEX)/cplex/include \
	-I$(CPLEX)/concert/include 


INCLUDEDIRS = $(BASEDIRS)

CXX = g++
LD =
SUFFIX = o

CXXFLAGS = -Wall -pedantic -g $(INCLUDEDIRS) -m64 -fPIC \
	-fexceptions -DNEBUG -DIL_STD -Wno-long-long \
	-Wno-attributes  -fpermissive -Wno-sign-compare


LDFLAGS =  -lboost_system  -lboost_filesystem \
	-L$(CPLEXLIB) -lilocplex -lcplex \
	-L$(CONCERTLIB) -lconcert -m64 -lm -pthread -lrt


RENAME = -o

CXXSOURCES = \
	marginirv.cpp \
	sim_irv.cpp \
	model.cpp \
	tree_irv.cpp \
	irv_distance.cpp
	
CXXOBJECTS = $(patsubst %.cpp, $(OBJDIR)/%.$(SUFFIX), $(CXXSOURCES))

all : $(PROGRAM)

$(PROGRAM) : $(CXXOBJECTS)
	$(CXX) -o ${@} $(CXXOBJECTS) $(LD) $(LDFLAGS) 

$(OBJDIR)/%.$(SUFFIX) : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(RENAME) $(@D)/$(@F) -c $(<)

clean:
	$(RM) $(CXXOBJECTS) $(PROGRAM) $(OBJDIR)


