file(GLOB CPP_FILENAMES *.cpp)

foreach(FILENAME ${CPP_FILENAMES})
    cmake_path(GET FILENAME STEM FILENAME_NO_EXT)
    add_executable(${FILENAME_NO_EXT} ${FILENAME})
endforeach()