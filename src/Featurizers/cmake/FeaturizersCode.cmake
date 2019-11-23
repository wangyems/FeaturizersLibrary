# ----------------------------------------------------------------------
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License
# ----------------------------------------------------------------------
get_filename_component(_this_path ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

add_library(FeaturizersCode STATIC
    ${_this_path}/../CatImputerFeaturizer.h
    ${_this_path}/../DateTimeFeaturizer.h
    ${_this_path}/../DateTimeFeaturizer.cpp
    ${_this_path}/../SampleAddFeaturizer.h
    ${_this_path}/../SampleAddFeaturizer.cpp
    ${_this_path}/../StringFeaturizer.h
    ${_this_path}/../TimeSeriesImputerFeaturizer

    ${_this_path}/../Components/Components.h
    ${_this_path}/../Components/InferenceOnlyFeaturizerImpl.h
    ${_this_path}/../Components/PipelineExecutionEstimatorImpl.h
    ${_this_path}/../Components/TimeSeriesFrequencyEstimator.h
    ${_this_path}/../Components/TimeSeriesImputerTransformer.h
    ${_this_path}/../Components/TrainingOnlyEstimatorImpl.h

    ${_this_path}/../Components/Details/PipelineExecutionEstimatorImpl_details.h
)

file(GLOB JSON_DATA "${_this_path}/../DateTimeFeaturizerData/GeneratedCode/*.json")

foreach(df ${JSON_DATA})
    file(COPY ${df} DESTINATION Data/DateTimeFeaturizer)
endforeach()
