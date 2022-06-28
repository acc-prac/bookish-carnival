set_target_properties(irregularia_devicelib PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(irregularia_devicelib PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)

target_sources(irregularia_devicelib PRIVATE 
    thrust.cu)
set_target_properties(irregularia_devicelib PROPERTIES LINKER_LANGUAGE CUDA)

target_link_libraries(irregularia_devicelib ${CUDA_LIBRARIES})

