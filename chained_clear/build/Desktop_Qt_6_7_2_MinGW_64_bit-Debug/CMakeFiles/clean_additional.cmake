# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\chained_clear_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\chained_clear_autogen.dir\\ParseCache.txt"
  "chained_clear_autogen"
  )
endif()
