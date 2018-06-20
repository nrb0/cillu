find_package(Boost REQUIRED)

SET(
  cillu_FILES
    main.cpp
    KontrolS49.cpp
    KontrolS49.h
    net/OSCReceiver.cpp
    net/OSCReceiver.h
    MidiMessage.cpp
    MidiMessage.h
    EnvelopeGenerator.cpp
    EnvelopeGenerator.h
    Illuminator.cpp
    Illuminator.h
    KeyColorModule.cpp
    KeyColorModule.h
    IlluminatorHelpers.cpp
    IlluminatorHelpers.h
    utils/Color.cpp
    utils/Color.h
    utils/ColorHelpers.cpp
    utils/ColorHelpers.h
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)

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
