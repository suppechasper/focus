PROJECT(R)

################ focus #################

ADD_CUSTOM_TARGET(R_focus
    COMMAND rm -rf ./focus
    COMMAND rsync -aC ${R_SOURCE_DIR}/focus .
    COMMAND cp -a ${CMAKE_SOURCE_DIR}/external/annmod/lib/ann/ANN ./focus/src/ANN
    COMMAND cp -a ${CMAKE_SOURCE_DIR}/external/gmra/lib/* ./focus/src/. 
    COMMAND cp  ${CMAKE_SOURCE_DIR}/lib/*.h ./focus/src/. 
    COMMAND cp  ${CMAKE_SOURCE_DIR}/lib/projectors/* ./focus/src/.
    COMMAND cp  ${CMAKE_SOURCE_DIR}/lib/dels/* ./focus/src/. 
    COMMAND cp  
    ${CMAKE_SOURCE_DIR}/external/gmra/external/metrics/lib/EigenMetric.h
    ${CMAKE_SOURCE_DIR}/external/gmra/external/metrics/lib//EigenEuclideanMetric.h 
    ${CMAKE_SOURCE_DIR}/external/gmra/external/metrics/lib//EigenL1Metric.h 
    ${CMAKE_SOURCE_DIR}/external/gmra/external/metrics/lib//EigenSquaredEuclideanMetric.h 
    ${CMAKE_SOURCE_DIR}/external/gmra/external/metrics/lib//EigenWasserstein.h
    ${CMAKE_SOURCE_DIR}/external/gmra/external/randomsvd/lib//EigenRandomSVD.h
    ${CMAKE_SOURCE_DIR}/external/gmra/external/randomsvd/lib//EigenRandomRange.h
    ${CMAKE_SOURCE_DIR}/external/gmra/external/utils/lib//Random.h
    ./focus/src
)

ADD_CUSTOM_TARGET(R_focus_build  
    COMMAND R CMD build --resave-data focus)
ADD_DEPENDENCIES(R_focus_build R_focus)

ADD_CUSTOM_TARGET(R_focus_install
    COMMAND R CMD INSTALL --no-multiarch focus)
ADD_DEPENDENCIES(R_focus_install R_focus)

ADD_CUSTOM_TARGET(R_focus_check 
    COMMAND R CMD check --no-multiarch focus)
ADD_DEPENDENCIES(R_focus_check R_focus)


