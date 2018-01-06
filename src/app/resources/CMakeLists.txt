# Generic look-and-feel resources
set(ui_rcs app/resources/icomoon.qrc PARENT_SCOPE)

# Platform-specific look-and-feel resources
#if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    #set(ui_rcs app/resources/tango.qrc PARENT_SCOPE)
#endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)
