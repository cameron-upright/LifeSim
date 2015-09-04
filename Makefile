#================================================================

SRC_DIR = src/
UTIL_DIR = src/Util/
UTIL_MATH_DIR = src/Util/Math/
UTIL_GFX_DIR = src/Util/Graphics/
SCENE_DIR = src/Scene/
SCENE_CONSTRAINT_DIR = src/Scene/Constraint/
SCENE_OBJECTS_DIR = src/Scene/Objects/
CREATURE_DIR = src/Creature/
RESOURCE_DIR = src/Resource/
GRAPHICS_DIR = src/Graphics/
GRAPHICS_MESH_DIR = src/Graphics/Mesh/
GRAPHICS_SHADER_DIR = src/Graphics/Shader/
EXPERIMENT_DIR = src/Experiment/
RL_DIR = src/RL/

SRC_DIRS = $(SRC_DIR) $(UTIL_DIR) $(UTIL_GFX_DIR) $(UTIL_MATH_DIR) $(SCENE_DIR) $(SCENE_CONSTRAINT_DIR) $(SCENE_OBJECTS_DIR) $(CREATURE_DIR) $(RESOURCE_DIR) $(GRAPHICS_DIR) $(GRAPHICS_MESH_DIR) $(GRAPHICS_SHADER_DIR) $(EXPERIMENT_DIR) $(RL_DIR)

SRC_OBJS = Main.o
UTIL_GFX_OBJS = g_axis.o
SCENE_OBJS = Scene.o SceneVis.o ScenePhysics.o
SCENE_OBJECTS_OBJS = ScenePlane.o SceneSphere.o SceneBox.o SceneBoxVis.o ScenePlaneVis.o
CREATURE_OBJS = Creature.o
RESOURCE_OBJS = ResourceManager.o
GRAPHICS_MESH_OBJS = Mesh.o
GRAPHICS_SHADER_OBJS = VertexProgram.o FragmentProgram.o
RL_OBJS = RLExperiment.o RLEnvironment.o RLAgent.o RLAgentDummy.o

LIB_OBJS = $(addprefix $(UTIL_GFX_DIR), $(UTIL_GFX_OBJS)) $(addprefix $(SCENE_DIR), $(SCENE_OBJS)) $(addprefix $(SCENE_OBJECTS_DIR), $(SCENE_OBJECTS_OBJS)) $(addprefix $(CREATURE_DIR), $(CREATURE_OBJS)) $(addprefix $(RESOURCE_DIR), $(RESOURCE_OBJS)) $(addprefix $(GRAPHICS_MESH_DIR), $(GRAPHICS_MESH_OBJS)) $(addprefix $(GRAPHICS_SHADER_DIR), $(GRAPHICS_SHADER_OBJS)) $(addprefix $(EXPERIMENT_DIR), $(EXPERIMENT_OBJS)) $(addprefix $(RL_DIR), $(RL_OBJS))
GUI_OBJS = $(addprefix $(SRC_DIR), $(SRC_OBJS))
DUMMY_CREATURE_AGENT_OBJS = src/DummyCreatureAgent.o
DUMMY_CREATURE_EXPERIMENT_OBJS = src/DummyCreatureExperiment.o
PROGRAM = gui

PROTO_DIR = src/Proto
PROTO_DEFINITION = src/Proto/LifeSim.proto
PROTO_INCLUDES = src/Proto/LifeSim.pb.h
PROTO_SOURCES = src/Proto/LifeSim.pb.cc
PROTO_OBJS = src/Proto/LifeSim.pb.o

#================================================================

#================================================================
CC = g++
#INCLUDE_OPTS = -I/usr/share/doc/NVIDIA_GLX-1.0/include -I$(HOME)/usr/include $(addprefix -I, $(SRC_DIRS)) -I/usr/local/include/bullet -I/opt/local/include
INCLUDE_OPTS = -I$(addprefix -I, $(SRC_DIRS)) -I/usr/local/include/bullet -I/usr/include/bullet -Isrc/Proto
CFLAGS = $(INCLUDE_OPTS) -std=c++11 -Wno-deprecated -LANG:std -c -DdSingle #-D_REENTRANT
#================================================================

ifdef DEBUG
	OPTFLAGS += -O0 -g -pg
else
	OPTFLAGS += -O3
endif

#================================================================
X11LIB=/usr/X11R6/lib
  #-lg2c
ifeq ($(shell uname),Linux)
LDFLAGS += -L/usr/lib/nvidia-331 -L/usr/lib/nvidia-current -L$(X11LIB) -L$(HOME)/usr/lib \
    -lglut -lpthread -lGL -lGLU -lXi -lXmu -lX11 -ldl -lm -lstdc++ -lpng -llapack \
    -lBulletDynamics -lBulletCollision -lLinearMath -lprotobuf

else
LDFLAGS += -L$(X11LIB) -L$(HOME)/usr/lib -L/opt/local/lib \
    -lpthread -lGL -lGLU -lXi -lXmu -lX11 -ldl -lm -lstdc++ -lpng -llapack -lglut -framework Accelerate \
    -lBulletDynamics -lBulletCollision -lLinearMath -lprotobuf


#-framework Cocoa -framework OpenGL -framework GLUT -framework Accelerate
endif

#================================================================

ALL: gui DummyCreatureAgent DummyCreatureExperiment

#src/Proto/LifeSim.pb.o: src/Proto/LifeSim.pb.cc src/Proto/LifeSim.pb.h
#	protoc -I $(PROTO_DIR) --cpp_out $(PROTO_DIR) $(PROTO_DIR)/LifeSim.proto

src/Proto/LifeSim.pb.cc src/Proto/LifeSim.pb.h: src/Proto/LifeSim.proto
	protoc -I $(PROTO_DIR) --cpp_out $(PROTO_DIR) $(PROTO_DIR)/LifeSim.proto

$(GUI_OBJS) $(LIB_OBJS): $(PROTO_INCLUDES)

.cc.o:
	$(CC) $(OPTFLAGS) $(CFLAGS) -o $@ $<
.C.o: $(PROTO_INCLUDES)
	$(CC) $(OPTFLAGS) $(CFLAGS) -o $@ $<
.cpp.o: $(PROTO_INCLUDES)
	$(CC) $(OPTFLAGS) $(CFLAGS) -o $@ $<

gui: $(GUI_OBJS) $(LIB_OBJS) $(PROTO_OBJS)
	$(RM) $@
	$(CC) -o $@ $(GUI_OBJS) $(LIB_OBJS) $(PROTO_OBJS) $(OPTFLAGS) $(LDFLAGS) -lrlutils -lrlenvironment -lrlgluenetdev


DummyCreatureAgent: $(DUMMY_CREATURE_AGENT_OBJS) $(PROTO_OBJS)
	$(RM) $@
	$(CC) -o $@ $(DUMMY_CREATURE_AGENT_OBJS) $(PROTO_OBJS) $(OPTFLAGS) $(LDFLAGS) -lrlutils -lrlagent -lrlgluenetdev

DummyCreatureExperiment: $(DUMMY_CREATURE_EXPERIMENT_OBJS) $(PROTO_OBJS)
	$(RM) $@
	$(CC) -o $@ $(DUMMY_CREATURE_EXPERIMENT_OBJS) $(PROTO_OBJS) $(OPTFLAGS) $(LDFLAGS) -lrlutils -lrlexperiment -lrlgluenetdev


dbg: gui
opt: gui

clean:
	rm -f $(GUI_OBJS) $(LIB_OBJS) $(PROTO_OBJS) $(PROTO_INCLUDES) $(PROTO_SOURCES) $(PROGRAM)
	rm -f src/DummyCreatureAgent.o src/DummyCreatureExperiment.o

#-include $(OBJS:.o=.d)
#================================================================

