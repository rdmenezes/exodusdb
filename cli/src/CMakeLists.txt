cmake_minimum_required(VERSION 2.8)

#2014/06/01 exodus doesnt require c++11 but we enable it for
#removing this to avoid problem linking to filesystem at the moment with c++11
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
	../../exodus/libexodus
)

project(exodus)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}../../exodus/libexodus/exodus)


### 1. COMPILE ###

#NB add any NEW programs here and in TWO MORE sections further down
#target_link_libraries(
#install(

add_executable(compile compile.cpp)
add_executable(createfile createfile.cpp)
add_executable(createindex createindex.cpp)
add_executable(delete delete.cpp)
add_executable(clearfile clearfile.cpp)
add_executable(deletefile deletefile.cpp)
add_executable(deleteindex deleteindex.cpp)
add_executable(edir edir.cpp)
add_executable(edic edic.cpp)

#name is name only ... input and output files are exodus
add_executable(exodus-cli exodus.cpp)
set_target_properties(exodus-cli PROPERTIES OUTPUT_NAME exodus)

add_executable(list list.cpp printtext.h)
add_executable(listfiles listfiles.cpp)
add_executable(listindexes listindexes.cpp)
add_executable(testsort testsort.cpp)
add_executable(configexodus configexodus.cpp)


### 2. LINK ###

#dl is needed for dynamic link library code

target_link_libraries(compile exodus ${LIBS})
target_link_libraries(createfile exodus ${LIBS})
target_link_libraries(createindex exodus ${LIBS})
target_link_libraries(delete exodus ${LIBS})
target_link_libraries(clearfile exodus ${LIBS})
target_link_libraries(deletefile exodus ${LIBS})
target_link_libraries(deleteindex exodus ${LIBS})
target_link_libraries(edir exodus ${LIBS})
target_link_libraries(edic exodus ${LIBS})
target_link_libraries(exodus-cli exodus ${LIBS})
target_link_libraries(list exodus ${LIBS})
target_link_libraries(listfiles exodus ${LIBS})
target_link_libraries(listindexes exodus ${LIBS})
target_link_libraries(testsort exodus ${LIBS})
target_link_libraries(configexodus exodus ${LIBS})


### 3. INSTALL ###

install(TARGETS compile
createfile
createindex
delete
clearfile
deletefile
deleteindex
edir
edic
exodus-cli
list
listfiles
listindexes
testsort
configexodus
 DESTINATION bin)
