SRC = src/*.cpp src/*/*.cpp
COMPILER = g++
COMPILER_FLAGS = -w

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

# librerias
LINKER_FLAGS = -lSDL2 -lSDL2_image

OUTPUT = EZ

all : $(SRC)
	$(COMPILER) $(SRC) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o build/$(OUTPUT)

cache : $(SRC)
	ccache $(COMPILER) $(SRC) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o build/$(OUTPUT)

fault : $(SRC)
	$(COMPILER) $(SRC) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o build/$(OUTPUT) -g
