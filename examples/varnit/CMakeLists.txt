add_executable(varnit varnit.cpp)

# MySQL configuration
set(MYSQL_INCLUDE_DIR "C:/Program Files/MySQL/MySQL Server 8.0/include")
set(MYSQL_LIB_DIR "C:/Program Files/MySQL/MySQL Server 8.0/lib")

target_include_directories(varnit PRIVATE ${MYSQL_INCLUDE_DIR})
target_link_directories(varnit PRIVATE ${MYSQL_LIB_DIR})

# Link against the framework and mysql
target_link_libraries(varnit PRIVATE bro libmysql.lib)
