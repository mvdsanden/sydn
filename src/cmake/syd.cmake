cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "-Werror -std=c++17 -ggdb")

function(syd_add_library)
  set(options "")
  set(oneValueArgs NAME)
  set(multiValueArgs COMPONENTS DEPENDENCIES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  set(sources "")
  
  foreach(component ${ARGS_COMPONENTS})    
    set(source "${component}.cpp")
    message("Found source ${source}")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
      list(APPEND sources "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
    endif()
  endforeach(component)

  add_library(${ARGS_NAME} ${sources})
  target_include_directories (${ARGS_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${ARGS_NAME} ${ARGS_DEPENDENCIES})

  foreach(component ${ARGS_COMPONENTS})
    set(source "${CMAKE_CURRENT_SOURCE_DIR}/${component}.t.cpp")
    if(EXISTS ${source})
      set(target "${component}.t.tsk")
      add_executable(${target} ${source})
      target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
      target_link_libraries(${target} ${ARGS_NAME} gtest_main)
      add_test(NAME ${source} COMMAND ${target})
    endif()
  endforeach(component)
  
  
endfunction(syd_add_library)
