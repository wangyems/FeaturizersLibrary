# ----------------------------------------------------------------------
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License
# ----------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5.0)

project(Featurizer_UnitTests LANGUAGES CXX)

set(_includes "$ENV{INCLUDE}")
set(_libs "$ENV{LIB}")
set(CMAKE_MODULE_PATH "$ENV{DEVELOPMENT_ENVIRONMENT_CMAKE_MODULE_PATH}")

if(NOT WIN32)
    string(REPLACE ":" ";" CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}")
    string(REPLACE ":" ";" _includes "$ENV{INCLUDE}")
    string(REPLACE ":" ";" _libs "$ENV{LIB}")
endif()

include(CppCommon OPTIONAL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

get_filename_component(_this_path ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

include(${_this_path}/../cmake/FeaturizersCode.cmake)

enable_testing()

foreach(_test_name IN ITEMS
    AnalyticalRollingWindowFeaturizer_UnitTest
    BackwardFillImputerFeaturizer_UnitTest
    CatImputerFeaturizer_UnitTests
    CountVectorizerFeaturizer_UnitTests
    DateTimeFeaturizer_UnitTests
    ForecastingPivotFeaturizer_UnitTests
    ForwardFillImputerFeaturizer_UnitTest
    FromStringFeaturizer_UnitTest
    HashOneHotVectorizerFeaturizer_UnitTests
    ImputationMarkerFeaturizer_UnitTests
    LabelEncoderFeaturizer_UnitTests
    LagLeadOperatorFeaturizer_UnitTests
    MaxAbsScalerFeaturizer_UnitTests
    MeanImputerFeaturizer_UnitTest
    MedianImputerFeaturizer_UnitTest
    MinMaxImputerFeaturizer_UnitTest
    MinMaxScalerFeaturizer_UnitTests
    MissingDummiesFeaturizer_UnitTests
    NormalizeFeaturizer_UnitTests
    ModeImputerFeaturizer_UnitTest
    NumericalizeFeaturizer_UnitTests
    OneHotEncoderFeaturizer_UnitTests
    PCAFeaturizer_UnitTests
    RobustScalerFeaturizer_UnitTests
    SampleAddFeaturizer_UnitTest
    ShortGrainDropperFeaturizer_UnitTests
    SimpleRollingWindowFeaturizer_UnitTest
    StandardScaleWrapperFeaturizer_UnitTest
    StringFeaturizer_UnitTest
    Structs_UnitTest
    TfidfVectorizerFeaturizer_UnitTests
    TimeSeriesImputerFeaturizer_UnitTest
    TruncatedSVDFeaturizer_UnitTests
)
    add_executable(${_test_name} ${_test_name}.cpp)

    target_include_directories(${_test_name} PRIVATE ${_includes})
    target_link_directories(${_test_name} PRIVATE ${_libs})

    target_link_libraries(${_test_name} PRIVATE
        FeaturizersCode
    )

    add_test(NAME ${_test_name} COMMAND ${_test_name} --success)
endforeach()
