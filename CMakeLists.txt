cmake_minimum_required (VERSION 3.15)
project ("p2p-chat"
    VERSION 1.0.0
    DESCRIPTION "Simple udp chat with STUN client to traverse NAT"
    LANGUAGES CXX)

set(STUN_SRC 
	"src/chat_client.cc"
	"src/chat_client.h"
	"src/chat_view.h"
	"src/endpoint.h"
	"src/exception.h"
	"src/hex.h"
	"src/log.h"
	"src/stun_client.cc"
	"src/stun_client.h"
	"src/stun_protocol.h"
	"src/udp_socket.h"
	"src/udp_timeout_socket.cc")

add_executable(main "src/main.cc" ${STUN_SRC})
target_compile_features(main PUBLIC cxx_std_20)
target_link_libraries(main PRIVATE winmm.lib ws2_32.lib)
set_target_properties(main 
    PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        OUTPUT_NAME "p2p-chat-${CMAKE_PROJECT_VERSION}")
