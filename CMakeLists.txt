cmake_minimum_required (VERSION 3.14)
project ("p2p-chat"
    VERSION 0.0.3
    DESCRIPTION "Simple udp chat, nothing special"
    LANGUAGES CXX)

set(STUN_SRC 
	"src/exception.h"
	"src/hex.h"
	"src/packet.h"
	"src/stun_client.h"
	"src/stun_client.cc")

add_executable(main "src/main.cpp" ${STUN_SRC})
target_compile_features(main PUBLIC cxx_std_20)
target_link_libraries(main PRIVATE winmm.lib ws2_32.lib)
set_target_properties(main 
    PROPERTIES
        OUTPUT_NAME "p2p-chat-${CMAKE_PROJECT_VERSION}")
