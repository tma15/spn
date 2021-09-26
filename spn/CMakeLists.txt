
set(SOURCE_FILES
  dictionary.cc
  link_weight_calculator.cc
  network_builder.cc
  wordnet_reader.cc
)

set(HEADER_FILES
  argparser.h
  dictionary.h
  error.h
  link_weight_calculator.h
  network_builder.h
  string_util.h
  wordnet_reader.h
)


add_library(spn SHARED ${SOURCE_FILES} ${HEADER_FILES})
message("${SQLite3_LIBRARY} ${SQLite3_INCLUDE_DIR}")
target_link_directories(spn PUBLIC ${MECAB_PREFIX}/lib)
target_link_libraries(spn ${SQLite3_LIBRARY} mecab)
target_include_directories(spn PUBLIC ${SQLite3_INCLUDE_DIR} ${MECAB_PREFIX}/include)

add_executable(spn_build build.cc)
target_link_libraries(spn_build spn)


install(TARGETS spn_build RUNTIME DESTINATION bin)
install(TARGETS spn LIBRARY DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/spn)
