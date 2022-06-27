if(NOT DEFINED CMAKE_CUDA_STANDARD)
    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endif()

list(APPEND CUDA_NVCC_FLAGS -stdpar=gpu -gpu=cc61,cuda11.6)

set_property(TARGET bookish-carnival_devicelib PROPERTY CUDA_ARCHITECTURES 70-real 72-virtual)
set_target_properties(bookish-carnival_devicelib PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(bookish-carnival_devicelib PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)

target_sources(bookish-carnival_devicelib PRIVATE 
    thrust.cu)
set_target_properties(bookish-carnival_devicelib PROPERTIES LINKER_LANGUAGE CUDA)

target_link_libraries(bookish-carnival_devicelib ${CUDA_LIBRARIES})

