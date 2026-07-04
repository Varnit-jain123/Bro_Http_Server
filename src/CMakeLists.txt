add_library(bro 
            bro_global.cpp 
            bro_gmt_date_time.cpp 
            bro_utilities.cpp 
            bro_file_system_utilties.cpp 
            bro_string_utility.cpp 
            bro_http_error_status_utility.cpp 
            bro_validator.cpp 
            bro_error.cpp 
            bro_cookies.cpp
            bro_header_utility.cpp
            bro_request.cpp
            bro_response.cpp
            bro_http_response_utility.cpp
            bro_simple_function.cpp
            bro_application_level_Container_Dependent_Function.cpp
            bro_simple_startup_function.cpp
            bro_application_level_Container_Dependent_Startup_Function.cpp
            bro_startup_function_comparator.cpp
            bro_template_engine.cpp
            bro_container.cpp
            bro_thread_wrapper_node.cpp
            bro.cpp
            )

# The include directory where the headers are located.
# We set it as PUBLIC so that targets linking against `bro` will automatically
# get this include directory. We expose the parent of `bro/` so that users
# can use `#include <bro/bro.h>`. However, since existing code uses `#include <bro.h>`,
# we can also add the direct path as an include directory for backward compatibility.
target_include_directories(bro PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/bro
)

# Link against threads
target_link_libraries(bro PUBLIC Threads::Threads)

if(WIN32)
    target_link_libraries(bro PUBLIC ws2_32)
endif()
