cmake_minimum_required (VERSION 2.6)
project (BarcodeReader)
MESSAGE( STATUS "PROJECT_NAME: " ${PROJECT_NAME} )

# The version number.
set(BarcodeReader_VERSION_MAJOR 7)
set(BarcodeReader_VERSION_MINOR 5)

# Check platforms
if(CMAKE_HOST_UNIX)
    set(LINUX 1)
endif()

# Set RPATH
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/dynamsoft/lib")
MESSAGE( STATUS "CMAKE_INSTALL_RPATH: " "${CMAKE_INSTALL_PREFIX}/dynamsoft/lib" )
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/BarcodeReaderConfig.h.in"
    "${PROJECT_BINARY_DIR}/BarcodeReaderConfig.h"
)

# Add search path for include and lib files
if(LINUX)
    link_directories("${PROJECT_SOURCE_DIR}/platforms/linux/")
endif()
include_directories("${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/include/")

# Add the executable
add_executable(BarcodeReader BarcodeReader.cxx)
target_link_libraries (BarcodeReader "DynamsoftBarcodeReader")

# Set installation directory
if(LINUX)
    install (DIRECTORY  "${PROJECT_SOURCE_DIR}/platforms/linux/" DESTINATION "${CMAKE_INSTALL_PREFIX}/dynamsoft/lib")
endif()

install (TARGETS BarcodeReader DESTINATION "${CMAKE_INSTALL_PREFIX}/dynamsoft/bin")
install (FILES "${PROJECT_BINARY_DIR}/BarcodeReaderConfig.h" DESTINATION "${CMAKE_INSTALL_PREFIX}/dynamsoft/include")
install (DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_PREFIX}/dynamsoft/include")

# Use CTest
include(CTest)
add_test (BarcodeReaderRuns BarcodeReader)
