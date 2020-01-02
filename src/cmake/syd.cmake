cmake_minimum_required(VERSION 3.7)

function(syd_add_library)
  set(options "")
  set(oneValueArgs NAME)
  set(multiValueArgs COMPONENTS DEPENDENCIES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  set(sources "")
  
  foreach(component ${ARGS_COMPONENTS})
    set(source "${component}.cpp")
    if(EXISTS ${source})
      list(APPEND sources "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
    endif()
  endforeach(component)

  add_library(${ARGS_NAME} ${sources})
  target_link_libraries(${ARGS_NAME} ${ARGS_DEPENDENCIES})

  foreach(component ${ARGS_COMPONENTS})
    set(source "${CMAKE_CURRENT_SOURCE_DIR}/${component}.t.cpp")
    if(EXISTS ${source})
      set(target "${component}.t.tsk")
      add_executable(${target} ${source})
      target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
      target_link_libraries(${target} ${ARGS_NAME})
      add_test(NAME ${source} COMMAND ${target})
    endif()
  endforeach(component)
  
  
endfunction(syd_add_library)
