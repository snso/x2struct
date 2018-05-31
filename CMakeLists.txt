
#Project Make
cmake_minimum_required(VERSION 2.8)


SET(project_name runX2struct)
PROJECT(${project_name})

#Set Build Target System
SET(CMAKE_SYSTEM_NAME Linux)
# SET(CMAKE_SYSTEM_NAME Android)

#Choose Compiler
if(CMAKE_SYSTEM_NAME MATCHES "Android")
	MESSAGE(STATUS "Target-----Android")
	SET(CMAKE_C_COMPILER "aarch64-linux-android-gcc")
	SET(CMAKE_CXX_COMPILER "aarch64-linux-android-g++")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "Target-----Linux")
	SET(CMAKE_C_COMPILER "gcc")
	SET(CMAKE_CXX_COMPILER "g++")
endif()


#Set Build Type
SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_BUILD_TYPE "release")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -pie  -fPIE -std=gnu++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -pie -fPIE -std=gnu++11 -fvisibility=hidden")

MESSAGE(STATUS "Target C Compiler : ${CMAKE_C_FLAGS_DEBUG}")
MESSAGE(STATUS "Compiler Flags: ${CMAKE_CXX_FLAGS_RELEASE}")

#include path
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/x2struct
					${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty
					${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/libbson/include
					${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/libconfig/include
					${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/rapidjson/include
					${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/rapidxml/include
					${PROJECT_SOURCE_DIR}/lib/jsoncpp/include
					)

#source directory
AUX_SOURCE_DIRECTORY( ${PROJECT_SOURCE_DIR}/lib/x2struct x2structSrc)
AUX_SOURCE_DIRECTORY( ${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/libbson bsonSrc)
AUX_SOURCE_DIRECTORY( ${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/libconfig configSrc)
AUX_SOURCE_DIRECTORY( ${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/librapidjson rapidjsonSrc)
AUX_SOURCE_DIRECTORY( ${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/rapidxml rapidxmlSrc)
AUX_SOURCE_DIRECTORY( ${PROJECT_SOURCE_DIR}/lib/jsoncpp/src/json jsoncppSrc)
AUX_SOURCE_DIRECTORY( ${PROJECT_SOURCE_DIR}/src Src)

#set target lib property
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#Link Libraries
# SET(THIRD_LIB 	${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/libconfig/lib/libconfig++.a
				# ${PROJECT_SOURCE_DIR}/lib/x2struct/thirdparty/libbson/lib/libbson-1.0.a)
				
#Add Executable
add_library(x2struct STATIC ${x2structSrc} ${rapidxmlSrc} ${bsonSrc} ${configSrc} ${rapidjsonSrc})

#add executable file
ADD_EXECUTABLE(${project_name} ${Src} ${jsoncppSrc})

# add link library
TARGET_LINK_LIBRARIES(${project_name} x2struct)

