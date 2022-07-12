target_sources(irregularia_devicelib PRIVATE 
    thrust.cpp)
#set_target_properties(irregularia_devicelib PROPERTIES LINKER_LANGUAGE CUDA)

#target_link_libraries(irregularia_devicelib ${CUDA_LIBRARIES})

