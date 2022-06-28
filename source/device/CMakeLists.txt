list(APPEND CUDA_NVCC_FLAGS -stdpar=gpu -gpu=cc61,cuda11.6)

include(FindCUDA/select_compute_arch)

CUDA_DETECT_INSTALLED_GPUS(INSTALLED_GPU_CCS_1)
string(STRIP "${INSTALLED_GPU_CCS_1}" INSTALLED_GPU_CCS_2)
string(REPLACE " " ";" INSTALLED_GPU_CCS_3 "${INSTALLED_GPU_CCS_2}")
string(REPLACE "." "" CUDA_ARCH_LIST "${INSTALLED_GPU_CCS_3}")

# https://stackoverflow.com/a/72332793... One day...
set_property(TARGET irregularia_devicelib PROPERTY CUDA_ARCHITECTURES ${CUDA_ARCH_LIST})
set_target_properties(irregularia_devicelib PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(irregularia_devicelib PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)

target_sources(irregularia_devicelib PRIVATE 
    thrust.cu)
set_target_properties(irregularia_devicelib PROPERTIES LINKER_LANGUAGE CUDA)

target_link_libraries(irregularia_devicelib ${CUDA_LIBRARIES})

