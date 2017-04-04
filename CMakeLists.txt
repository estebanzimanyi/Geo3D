cmake_minimum_required(VERSION 2.8)
project(Geo3D)

if(WIN32)
	set(CMAKE_FIND_ROOT_PATH "C:/Program\ Files/PostgreSQL/9.4")
	set(PostgreSQL_ROOT_DIRECTORIES "C:/Program\ Files/PostgreSQL/9.4")
	set(PostgreSQL_ROOT "C:/Program\ Files/PostgreSQL/9.4")
endif()

find_package(PostgreSQL 9.4 REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})

if(WIN32)
	include_directories("${PostgreSQL_INCLUDE_DIR}/server/port/win32")
endif()

link_directories(${PostgreSQL_LIBRARY_DIRS})

file(GLOB SRCS "*.c")
add_library(Geo3D MODULE ${SRCS})


if(WIN32)
	target_link_libraries(Geo3D libpq postgres)
endif()

find_program(PGCONFIG pg_config)
execute_process(COMMAND ${PGCONFIG} --sharedir OUTPUT_VARIABLE PostgreSQL_SHARE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE) # FIXME: I don't think this work on Windows

file(GLOB SQL "*.in.sql")
list(SORT SQL)
add_custom_command(
	OUTPUT geo3d--0.0.1.sql
	COMMAND cat ${SQL} > ${CMAKE_BINARY_DIR}/geo3d--0.0.1.sql
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	DEPENDS ${SQL})

add_custom_target(sqlscript ALL DEPENDS ${CMAKE_BINARY_DIR}/geo3d--0.0.1.sql)

install(TARGETS Geo3D DESTINATION "${PostgreSQL_LIBRARY_DIR}/postgresql")
install(FILES geo3d.control DESTINATION "${PostgreSQL_SHARE_DIR}/extension")
install(FILES "${CMAKE_BINARY_DIR}/geo3d--0.0.1.sql" DESTINATION "${PostgreSQL_SHARE_DIR}/extension")
