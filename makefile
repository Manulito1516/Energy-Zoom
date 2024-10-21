CXX = g++
SRC := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
SRC2 := $(patsubst %.cpp,%.o,$(wildcard src/*/*.cpp))
EXE = EZ
LDFLAGS = -lSDL2 -lSDL2_image
#CXXFLAGS = -j

$(EXE): $(SRC) $(SRC2)
	ccache $(CXX) -o build/$(EXE) $(SRC) $(SRC2) $(LDFLAGS) $(CXXFLAGS)

-include $(SRC:.o=.d)
-include $(SRC2:.o=.d)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)
	@$(CXX) -MM $(CXXFLAGS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	rm -f $(EXE) src/*.o src/*.d src/*/*.o src/*/*.d
