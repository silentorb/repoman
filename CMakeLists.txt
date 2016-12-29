create_library(repoman)

#target_link_libraries(repoman ${boost_lib_filesystem} ${boost_lib_system} ${GIT_PATH}/lib/libgit231.dll)

include_directories(
  ${BOOST_PATH}
  ${GIT_PATH}/include
)

file(COPY ${GIT_PATH}/bin/libgit2.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
