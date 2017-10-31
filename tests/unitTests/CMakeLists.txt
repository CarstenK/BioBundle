
# boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)


SET(seq_tests_src ./seq/seq_tests.cpp)
SET(seq_tests_exe seq_tests)
ADD_EXECUTABLE(${seq_tests_exe} ${seq_tests_src})
target_link_libraries(${seq_tests_exe}
	${Boost_LIBRARIES} BioBundleLib
)

SET(utility_tests_src ./utility/utility_tests.cpp)
SET(utility_tests_exe utility_tests)
ADD_EXECUTABLE(${utility_tests_exe} ${utility_tests_src})
target_link_libraries(${utility_tests_exe}
	${Boost_LIBRARIES} BioBundleLib
)
