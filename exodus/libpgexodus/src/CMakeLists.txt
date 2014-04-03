cmake_minimum_required(VERSION 2.8)


# CONFIGURE PostgreSQL

find_package(PostgreSQL REQUIRED)

execute_process(
	COMMAND 		pg_config --pkglibdir
	OUTPUT_VARIABLE		PostgreSQL_PKGLIBDIR
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(PostgreSQL_PKGLIBDIR=${PostgreSQL_PKGLIBDIR})

execute_process(
	COMMAND 		pg_config --includedir-server
	OUTPUT_VARIABLE		PostgreSQL_INCLUDEDIR_SERVER
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(PostgreSQL_INCLUDEDIR_SERVER=${PostgreSQL_INCLUDEDIR_SERVER})

include_directories(
	.
	..
	${PostgreSQL_INCLUDE_DIRS}
	${PostgreSQL_INCLUDEDIR_SERVER}
)


# BUILD libpgexodus

set (CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(pgexodus SHARED

 mvconfig.h callexodus.cpp callexodus_ms.cpp callexodus_posix.cpp
 naturalorder.cpp pgnaturalorder.cpp extract.c pgexodus.c

)

#set_target_properties(pgexodus PROPERTIES OUTPUT_NAME pgexodus)


# INSTALL libpgexodus postgres server shared object and postinstall scripts

#place pgexodus.so in server package lib directory (not ordinary lib)
install(TARGETS pgexodus
DESTINATION ${PostgreSQL_PKGLIBDIR})

#script to add exodus functions into all new databases and exodus database
install(PROGRAMS installexodus-postgresql installexodus-postgresql2
DESTINATION ${PostgreSQL_PKGLIBDIR})

#				pg_config --pkglibdir
install(
	CODE "
		execute_process(
			COMMAND 
				ldconfig
				OUTPUT_VARIABLE OUTVAR
				ERROR_VARIABLE ERRVAR
				RESULT_VARIABLE RESVAR
			
		)
		message(ldconfig <\${OUTVAR}>\n
			<\${ERRVAR}>\n
			<\${RESVAR}>\n
		)
	"
)
install(
	CODE "
		execute_process(
			COMMAND 
				${PostgreSQL_PKGLIBDIR}/installexodus-postgresql
#				OUTPUT_VARIABLE OUTVAR
#				ERROR_VARIABLE ERRVAR
				RESULT_VARIABLE RESVAR
			
		)
		message(<\${OUTVAR}>\n
			<\${ERRVAR}>\n
			<\${RESVAR}>\n
		)
	"
)

#install (SCRIPT "${CMAKE_SOURCE_DIR}/PostInstall.cmake")