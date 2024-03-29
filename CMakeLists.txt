cmake_minimum_required (VERSION 3.8)

project(mbi_proj VERSION 0.1 LANGUAGES CXX)

add_subdirectory(lib/rabbitmq-c)
add_subdirectory(lib/yaml-cpp)

add_executable (mbi src/mbi.cpp)
add_library (YAMLParcer STATIC src/YAMLParcer.cpp src/YAMLParcer.h)
add_library (MBIClass STATIC src/MBIClass.cpp src/MBIClass.h)
add_library (RMQClient STATIC src/RMQClient.cpp src/RMQClient.h)
add_library (AMQPUtils STATIC src/amqp_utils.cpp src/amqp_utils.h)

target_include_directories(YAMLParcer PUBLIC lib/yaml-cpp/include)
target_include_directories(MBIClass PUBLIC lib/yaml-cpp/include)
target_include_directories(RMQClient PUBLIC lib/rabbitmq-c/librabbitmq)
target_include_directories(AMQPUtils PUBLIC lib/rabbitmq-c/librabbitmq)
if(MSVC)
	target_link_libraries(mbi PUBLIC lib/yaml-cpp/$(Configuration)/yaml-cpp)
	target_link_libraries(mbi PUBLIC lib/rabbitmq-c/librabbitmq/$(Configuration)/librabbitmq.4)
else()
	target_link_libraries(mbi PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/yaml-cpp/libyaml-cpp.a)
	target_link_libraries(mbi PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/rabbitmq-c/librabbitmq/librabbitmq.a)
	target_link_libraries(YAMLParcer PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/yaml-cpp/libyaml-cpp.a)
	target_link_libraries(RMQClient PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/rabbitmq-c/librabbitmq/librabbitmq.a)
endif()
target_link_libraries(mbi PUBLIC YAMLParcer)
target_link_libraries(mbi PUBLIC MBIClass)
target_link_libraries(mbi PUBLIC RMQClient)
target_link_libraries(mbi PUBLIC AMQPUtils)

if(WIN32)
	target_link_libraries(mbi PUBLIC ws2_32.lib)
endif()

target_compile_definitions(mbi PUBLIC AMQP_STATIC)
target_compile_definitions(RMQClient PUBLIC AMQP_STATIC)
target_compile_definitions(AMQPUtils PUBLIC AMQP_STATIC)
target_compile_features(mbi PUBLIC cxx_std_17)

add_dependencies(mbi yaml-cpp)
add_dependencies(mbi rabbitmq-static)