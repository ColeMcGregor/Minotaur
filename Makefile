CXX = g++
CXXFLAGS = -Isrc -IC:\raylib\include
LDFLAGS = -LC:\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = \
	src/visualization/harness.cpp \
	src/visualization/MazeRenderer.cpp \
	src/visualization/MazePanel.cpp \
	src/world/MazeState.cpp \
	src/world/MazeGenerator.cpp \
	src/world/MazeCarver.cpp \
	src/world/MazeStepPrinter.cpp \
	src/simulation/SimGenerator.cpp \
	src/visualization/EntityRenderer.cpp \
	src/visualization/ShaderRenderer.cpp \
	src/core/RNG.cpp

OUT = harness.exe

all:
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(OUT)

clean:
	del $(OUT)