#!/usr/bin/env python

# -----------------------------------------------------------------------------
# -- Hazel --------------------------------------------------------------------
# -----------------------------------------------------------------------------

generic_include_path = './Hazel/src/'
spdlog_include_path = './Hazel/vendor/spdlog/include/'
glad_include_path = './Hazel/vendor/glad/include/'
include_path = [generic_include_path, spdlog_include_path, glad_include_path]

hazel_bin_path = './Hazel/bin/'

hazel_source_files = Glob('./Hazel/src/**/*.cpp') + ['./Hazel/vendor/glad/src/glad.o']
hazel_name = 'hazel'
hazel_target = hazel_bin_path + hazel_name

glad_bin_path = './Hazel/vendor/glad/bin/'

StaticLibrary(target=hazel_target, source=hazel_source_files, CPPPATH=include_path)


# -----------------------------------------------------------------------------
# -- Sandbox ------------------------------------------------------------------
# -----------------------------------------------------------------------------

sandbox_bin_path = './Sandbox/bin/'

sandbox_source_files = Glob('./Sandbox/*.cpp')
sabdbox_name = 'App'
sandbox_target = sandbox_bin_path + sabdbox_name

glad_bin_path = './Hazel/vendor/glad/bin/'

sandbox_libs = ['hazel', 'glfw', 'dl']
sandbox_libpath = [hazel_bin_path]

Program(target=sandbox_target, source=sandbox_source_files, LIBS=sandbox_libs, LIBPATH=sandbox_libpath, CPPPATH=include_path)
