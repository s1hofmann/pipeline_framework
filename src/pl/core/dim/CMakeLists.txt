add_library(dimensionality_reduction_core rpca)

target_link_libraries(dimensionality_reduction_core ${OpenCV_LIBS}
                                                    exception)
