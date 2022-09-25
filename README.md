# qt-sample-opengl
sudo apt-get install libgl-dev libglu1-mesa libglu1-mesa-dev



sudo apt-get install libgl-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev freeglut3-dev






#for building glew
cmake -DCMAKE_INSTALL_PREFIX:PATH=`pwd`/../../../out-glew/ .
make
make install



#for building glfw

cmake -DCMAKE_INSTALL_PREFIX:PATH=`pwd`/../out-glfw/ .
make
make install








# link dynamic
g++ main.cpp WindowCreator.cpp Source2_sample_drawing_triangle_index.cpp Learn6.cpp Learn6_transform.cpp -I`pwd`/ext_lib/out-glew/include/ -I`pwd`/ext_lib/out-glfw/include/ -L`pwd`/ext_lib/out-glew/lib -L`pwd`/ext_lib/out-glfw/lib -lGLEW -lglfw3 -lGL -lX11 -ldl -lpthread

# link static
g++ main.cpp WindowCreator.cpp Source2_sample_drawing_triangle_index.cpp Learn6.cpp Learn6_transform.cpp -I`pwd`/ext_lib/out-glew/include/ -I`pwd`/ext_lib/out-glfw/include/ `pwd`/ext_lib/out-glew/lib/libGLEW.a `pwd`/ext_lib/out-glfw/lib/libglfw3.a -lGL -lX11 -ldl -lpthread


