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
EXPERIMENT_OBJS = Experiment.o
RL_OBJS = RLExperiment.o RLEnvironment.o RLAgent.o

OBJS = $(addprefix $(SRC_DIR), $(SRC_OBJS)) $(addprefix $(UTIL_GFX_DIR), $(UTIL_GFX_OBJS)) $(addprefix $(SCENE_DIR), $(SCENE_OBJS)) $(addprefix $(SCENE_OBJECTS_DIR), $(SCENE_OBJECTS_OBJS)) $(addprefix $(CREATURE_DIR), $(CREATURE_OBJS)) $(addprefix $(RESOURCE_DIR), $(RESOURCE_OBJS)) $(addprefix $(GRAPHICS_MESH_DIR), $(GRAPHICS_MESH_OBJS)) $(addprefix $(GRAPHICS_SHADER_DIR), $(GRAPHICS_SHADER_OBJS)) $(addprefix $(EXPERIMENT_DIR), $(EXPERIMENT_OBJS)) $(addprefix $(RL_DIR), $(RL_OBJS))
PROGRAM = gui


#================================================================

#================================================================
CC = g++
#INCLUDE_OPTS = -I/usr/share/doc/NVIDIA_GLX-1.0/include -I$(HOME)/usr/include $(addprefix -I, $(SRC_DIRS)) -I/usr/local/include/bullet -I/opt/local/include
INCLUDE_OPTS = -I$(addprefix -I, $(SRC_DIRS)) -I/usr/local/include/bullet
CFLAGS = $(INCLUDE_OPTS) -Wno-deprecated -LANG:std -c -DdSingle #-D_REENTRANT
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
    -lglut -lpthread -lGL -lGLU -lXi -lXmu -lX11 -ldl -lm -lstdc++ -lpng -llapack -lyaml-cpp \
    -lBulletDynamics -lBulletCollision -lLinearMath -lBulletMultiThreaded
else
LDFLAGS += -L$(X11LIB) -L$(HOME)/usr/lib -L/opt/local/lib \
    -lpthread -lGL -lGLU -lXi -lXmu -lX11 -ldl -lm -lstdc++ -lpng -llapack -lglut -framework Accelerate -lyaml-cpp \
    -lBulletDynamics -lBulletCollision -lLinearMath 


#-framework Cocoa -framework OpenGL -framework GLUT -framework Accelerate
endif

#================================================================
.C.o:
	$(CC) $(OPTFLAGS) $(CFLAGS) -o $@ $<
.cpp.o:
	$(CC) $(OPTFLAGS) $(CFLAGS) -o $@ $<

ALL: gui

gui: $(OBJS)
	$(RM) $@
	$(CC) -o $@ $(OBJS) $(OPTFLAGS) $(LDFLAGS)


dbg: gui
opt: gui

clean:
	rm -f $(OBJS) $(PROGRAM)

#-include $(OBJS:.o=.d)
#================================================================

