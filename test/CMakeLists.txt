#=============================================================================#
# Googletest deg:                                                             #
#=============================================================================#

INCLUDE(FetchContent)
ENABLE_TESTING()

FIND_PACKAGE(GTest)

IF (NOT GTest_FOUND)
  MESSAGE("GTest not found, installing via FetchContent")
  FETCHCONTENT_DECLARE(
    GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
  )

  FetchContent_MakeAvailable(GTest)
ENDIF()

INCLUDE(GoogleTest)
FOREACH(TEST base csv_data_source) 
  ADD_EXECUTABLE(test_${TEST} test_${TEST}.cpp)

  # TARGET_INCLUDE_DIRECTORIES(
  #   test_${TEST} PRIVATE include
  # )

  TARGET_COMPILE_OPTIONS(test_${TEST} PRIVATE ${CXXFLAGS})
  TARGET_LINK_LIBRARIES(test_${TEST} PRIVATE GTest::gtest_main quant_backtest)
  GTEST_DISCOVER_TESTS(test_${TEST})
ENDFOREACH()
