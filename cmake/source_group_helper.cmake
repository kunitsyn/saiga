macro(GroupSources startDir curdir)
    file(GLOB children RELATIVE ${startDir}/${curdir} ${startDir}/${curdir}/*)
    foreach(child ${children})
           if(IS_DIRECTORY ${startDir}/${curdir}/${child})
                   GroupSources(${startDir} ${curdir}/${child})
           else()
                   string(REPLACE "/" "\\" groupname ${curdir})
                source_group(${groupname} FILES ${startDir}/${curdir}/${child})
           endif()
    endforeach()
endmacro()
 
macro(GroupSources2 startDir)
    file(GLOB children RELATIVE ${startDir} ${startDir}/*)
    foreach(child ${children})
           if(IS_DIRECTORY ${startDir}/${child})
                   GroupSources(${startDir} ${child})
           else()
                  source_group("" FILES ${startDir}/${child})
           endif()
    endforeach()
 endmacro()