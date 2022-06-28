list(APPEND CUDA_NVCC_FLAGS -stdpar=gpu -gpu=cc61,cuda11.6)

set_property(TARGET irregularia_devicelib PROPERTY CUDA_ARCHITECTURES ${SUPPORTED_CUDA_ARCHITECTURES})
set_target_properties(irregularia_devicelib PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(irregularia_devicelib PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)

target_sources(irregularia_devicelib PRIVATE 
    thrust.cu)
set_target_properties(irregularia_devicelib PROPERTIES LINKER_LANGUAGE CUDA)

target_link_libraries(irregularia_devicelib ${CUDA_LIBRARIES})

