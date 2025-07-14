
cmake_minimum_required( VERSION 3.22 )

project( libm_ce C )

set( PROJECT_NAME libm_ce )
set( PROJECT_VERSION "9899.1.m.0" )
set( PROJECT_BRIEF "The ISO C standard math library, ISO/IEC 9899" )

add_definitions( -D__x86_64__)

include_directories( ${PROJECT_SOURCE_DIR}/lib/endian_h )
include_directories( src/ce-libm )

file( GLOB LIBM_SOURCES src/ce-libm/*.c )
add_library( m_ce ${LIBM_SOURCES} )

