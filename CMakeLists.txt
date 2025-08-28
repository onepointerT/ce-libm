
cmake_minimum_required( VERSION 3.22 )

project( libm_ce C ASM )

set( PROJECT_NAME libm_ce )
set( PROJECT_VERSION "9899.1.m.1" )
set( PROJECT_BRIEF "The ISO C standard math and analysis library, ISO/IEC 9899" )

add_definitions( -D__x86_64__)

include_directories( ${PROJECT_SOURCE_DIR}/lib/endian_h )
include_directories( src/ce-libm )
#include_directories( src/ce-liba )

file( GLOB LIBM_SOURCES src/ce-libm/*.c )
add_library( m_ce ${LIBM_SOURCES} )

#file( GLOB LIBA_SOURCES src/ce-liba/*.c )
#add_library( a_ce ${LIBA_SOURCES} )
#target_link_libraries( a_ce m_ce )

