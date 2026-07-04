add_executable(bobby bobby.cpp)

# Link against the framework
target_link_libraries(bobby PRIVATE bro)

# Copy necessary resources to the build directory (optional, but good for runtime)
# For example, copying the static files over so it works when running from build dir.
# But for now, we just assume the user runs it from the bobby directory.
