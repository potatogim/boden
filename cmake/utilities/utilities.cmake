macro(group_sources_automatically ROOT)
	if(${CMAKE_VERSION} VERSION_GREATER 3.8.2)
		source_group(TREE ${ROOT} FILES ${ARGN})
	endif()
endmacro()