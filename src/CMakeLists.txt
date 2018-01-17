
find_package(Boost REQUIRED)

SET(
  cillu_FILES
    main.cpp
    OSCReceiver.cpp
    OSCReceiver.h
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
  ${PROJECT_NAME}
  ${cillu_FILES}
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)

target_link_libraries(${PROJECT_NAME} PRIVATE cabl-static boost_system oscpack)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(${PROJECT_NAME} PUBLIC "-framework CoreFoundation" "-framework IOKit")
  target_link_libraries(${PROJECT_NAME} PUBLIC "-framework CoreAudio" "-framework CoreMIDI" objc)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
