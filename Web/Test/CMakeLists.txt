cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

# set(CMAKE_C_COMPILER "emcc")

project(Siv3D_Test CXX)
enable_language(C)

set(CMAKE_CXX_COMPILER "em++")
#set(CMAKE_CXX_COMPILER "g++")
# Emscripten needs -std=c++1z instead of c++17
# ( https://github.com/kripken/emscripten/issues/5513 )
# Use Emscripten ports (GLFW etc. https://kripken.github.io/emscripten-site/docs/compiling/Building-Projects.html#build-system-issues )
# Use WebGL 2.0 and Desktop OpenGL emuation ( https://kripken.github.io/emscripten-site/docs/porting/multimedia_and_graphics/OpenGL-support.html )
# Package resources and example directories ( https://kripken.github.io/emscripten-site/docs/porting/files/packaging_files.html )
# If LEGACY_GL_EMULATION is enabled, some code is added to shader sources.
# It causes error when the shader has #version directive.
# Therefore, LEGACY_GL_EMULATION is not used here.
# See: https://github.com/kripken/emscripten/issues/2105
#      https://github.com/kripken/emscripten/pull/2082
# FULL_ES3 is necessary for glMapBuffer
# See: https://kripken.github.io/emscripten-site/docs/optimizing/Optimizing-WebGL.html
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall -Wextra -Wno-unknown-pragmas -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s USE_ZLIB=1 -s USE_LIBPNG=1 -s USE_FREETYPE=1 --preload-file resources --preload-file example -s TOTAL_MEMORY=268435456 -s ASSERTIONS=2 -s DEMANGLE_SUPPORT=1")# -s WASM=1")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

#set(CMAKE_BUILD_TYPE Release)

#add_definitions(-D)

include_directories(
    #"/usr/include"
	"../deps/include"
    "../deps/include/freetype2"
	"../../Siv3D/include"
	"../../Siv3D/include/ThirdParty"
)

link_directories(
    #"/usr/lib"
    "../deps/lib"
)

# add_executable("Siv3D_Test.bc"
add_executable("Siv3D_Test"
	"../../Siv3D-Test/Main.cpp"
	# "../../Siv3D-Test/Test/Test.cpp"
)

# target_link_libraries("Siv3D_Test.bc"
target_link_libraries("Siv3D_Test"
	-lboost_filesystem
	-lboost_system
	-lpthread
	-lGL
	-lGLEW
    -lglfw  # use system GLFW
	# -lglib-2.0
	# -lgobject-2.0
	# -lgio-2.0
	-lpng
    #-lturbojpeg
    ${PROJECT_SOURCE_DIR}/../deps/lib/libturbojpeg.so
	# -lX11
	# -lXi
	# -lXrandr
	# -lXinerama
	# -lXcursor
	# -ldl
    #-lgif
    ${PROJECT_SOURCE_DIR}/../deps/lib/libgif.so
    #-lfreetype
    ${PROJECT_SOURCE_DIR}/../deps/lib/libfreetype.so
    #-lharfbuzz
    ${PROJECT_SOURCE_DIR}/../deps/lib/libharfbuzz.so
	-lopenal
	# -ludev
	# -lopencv_core
	# -lopencv_imgproc
	# -lBox2D

	${PROJECT_SOURCE_DIR}/../Debug/libSiv3D.a
	# ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Linux/angelscript/libangelscript.a
    ${PROJECT_SOURCE_DIR}/../deps/lib/libangelscript.a
)

# To emit HTML as executable
# http://d.hatena.ne.jp/mjt/20160511/p1
set_target_properties("Siv3D_Test"
  PROPERTIES
  SUFFIX ".html"
)

