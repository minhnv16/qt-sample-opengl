



# qt-sample-opengl
sudo apt-get install libgl-dev libglu1-mesa libglu1-mesa-dev



sudo apt-get install libgl-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev freeglut3-dev





#for building glfw
wget https://github.com/glfw/glfw/archive/refs/tags/3.3.4.tar.gz -O glfw-3.3.4.tar.gz
sudo apt-get install cmake3 libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

cmake -DCMAKE_INSTALL_PREFIX:PATH=`pwd`/../out-glfw/ .
make
make install



#for building glew
wget https://udomain.dl.sourceforge.net/project/glew/glew/2.2.0/glew-2.2.0.tgz
cmake -DCMAKE_INSTALL_PREFIX:PATH=`pwd`/../../../out-glew/ .
make
make install












# link dynamic
g++ -std=c++11 main.cpp WindowCreator.cpp Source2_sample_drawing_triangle_index.cpp Learn6.cpp Learn6_transform.cpp -I`pwd`/ext_lib/out-glew/include/ -I`pwd`/ext_lib/out-glfw/include/ -L`pwd`/ext_lib/out-glew/lib -L`pwd`/ext_lib/out-glfw/lib -lGLEW -lglfw3 -lGL -lX11 -ldl -lpthread

# link static
g++ -std=c++11 main.cpp WindowCreator.cpp Source2_sample_drawing_triangle_index.cpp Learn6.cpp Learn6_transform.cpp -I`pwd`/ext_lib/out-glew/include/ -I`pwd`/ext_lib/out-glfw/include/ `pwd`/ext_lib/out-glew/lib/libGLEW.a `pwd`/ext_lib/out-glfw/lib/libglfw3.a -lGL -lX11 -ldl -lpthread


# link static for sample texture
g++ -std=c++11 \
main.cpp \
main_l3.cpp \
texturebase.h \
texturebase.cpp \
texturewall.h \
texturewall.cpp \
 -I`pwd`/../../ext_lib/out-glew/include/ -I`pwd`/../../ext_lib/out-glfw/include/ `pwd`/../../ext_lib/out-glew/lib/libGLEW.a `pwd`/../../ext_lib/out-glfw/lib/libglfw3.a \
 -lGL -lX11 -ldl -lpthread

