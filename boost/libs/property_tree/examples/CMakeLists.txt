#
# Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
# Copyright (c) 2021 Richard Hodges (hodges.r@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/boostorg/json
#

source_group("test" FILES
    custom_data_type.cpp
    debug_settings.cpp
    empty_ptree_trick.cpp
    info_grammar_spirit.cpp
    speed_test.cpp
)

#

add_executable(custom_data_type
    custom_data_type.cpp
)
set_property(TARGET custom_data_type PROPERTY FOLDER "example")
target_link_libraries(custom_data_type PRIVATE Boost::property_tree)

#

add_executable(debug_settings
    debug_settings.cpp
)
set_property(TARGET debug_settings PROPERTY FOLDER "example")
target_link_libraries(debug_settings PRIVATE Boost::property_tree)

#

add_executable(empty_ptree_trick
    empty_ptree_trick.cpp
)
set_property(TARGET empty_ptree_trick PROPERTY FOLDER "example")
target_link_libraries(empty_ptree_trick PRIVATE Boost::property_tree)

#

add_executable(info_grammar_spirit
    info_grammar_spirit.cpp
)
set_property(TARGET info_grammar_spirit PROPERTY FOLDER "example")
target_link_libraries(info_grammar_spirit PRIVATE Boost::property_tree)

#

add_executable(speed_test
    speed_test.cpp
)
set_property(TARGET speed_test PROPERTY FOLDER "example")
target_link_libraries(speed_test PRIVATE Boost::property_tree)
