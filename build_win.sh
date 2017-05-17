#!/bin/bash

#nasm -f elf src/math/vector_sse.nasm -o obj/vector_sse.o
#nasm -f elf src/math/matrix_sse.nasm -o obj/matrix_sse.o

cflags='-O3 -DWIN64 -DNOSOUND '

echo '----------------------------------------------------------------------------'
echo 'Compiling warbot3d...'

gcc $cflags -c -Isrc/ src/math/matrix.c -o obj/matrix.o
gcc $cflags -c -Isrc/ src/math/vector.c -o obj/vector.o
gcc $cflags -c -Isrc/ src/math/line.c -o obj/line.o
gcc $cflags -c -Isrc/ src/math/triangle.c -o obj/triangle.o

gcc $cflags -c -Isrc/ src/engine/bitmap.c -o obj/bitmap.o
gcc $cflags -c -Isrc/ src/engine/audio.c -o obj/audio.o
gcc $cflags -c -Isrc/ src/engine/font.c -o obj/font.o
gcc $cflags -c -Isrc/ src/engine/input.c -o obj/input.o
gcc $cflags -c -Isrc/ src/engine/display.c -o obj/display.o

gcc $cflags -c -Isrc/ src/3d/material.c -o obj/material.o
gcc $cflags -c -Isrc/ src/3d/mesh.c -o obj/mesh.o
gcc $cflags -c -Isrc/ src/3d/rig.c -o obj/rig.o
gcc $cflags -c -Isrc/ src/3d/mesh/mesh_io_obj.c -o obj/mesh_io_obj.o
gcc $cflags -c -Isrc/ src/3d/mesh/vbo_mesh.c -o obj/vbo_mesh.o
gcc $cflags -c -Isrc/ src/3d/mesh/vbo_rig.c -o obj/vbo_rig.o
gcc $cflags -c -Isrc/ src/3d/light.c -o obj/light.o
gcc $cflags -c -Isrc/ src/3d/shader.c -o obj/shader.o
gcc $cflags -c -Isrc/ src/3d/skybox.c -o obj/skybox.o

gcc $cflags -c -Isrc/ src/robot.c -o obj/robot.o
gcc $cflags -c -Isrc/ src/scene/level.c -o obj/level.o
gcc $cflags -c -Isrc/ src/scene/level_creation.c -o obj/level_creation.o
gcc $cflags -c -Isrc/ src/scene/scene_input.c -o obj/scene_input.o
gcc $cflags -c -Isrc/ src/scene/scene_rendering.c -o obj/scene_rendering.o
gcc $cflags -c -Isrc/ src/laser.c -o obj/laser.o
gcc $cflags -c -Isrc/ src/smoke.c -o obj/smoke.o
gcc $cflags -c -Isrc/ src/scene/camera.c -o obj/camera.o
gcc $cflags -c -Isrc/ src/scene.c -o obj/scene.o
gcc $cflags -c -Isrc/ src/main.c -o obj/main.o


gcc $cflags -Isrc/ obj/*.o -o warbot3d -lglew32 -lopengl32 -lglu32 -lOpenAL32.dll -lsndfile -lglut

