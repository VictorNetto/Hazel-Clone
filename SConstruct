#!/usr/bin/env python

# -----------------------------------------------------------------------------
# -- Hazel --------------------------------------------------------------------
# -----------------------------------------------------------------------------

generic_include_path = './Hazel/src/'
spdlog_include_path = './Hazel/vendor/spdlog/include/'
include_path = [generic_include_path, spdlog_include_path]

hazel_bin_path = './Hazel/bin/'

hazel_source_files = Glob('./Hazel/src/Hazel/*.cpp') + Glob('./Hazel/src/Platform/*.cpp')
hazel_name = 'hazel'
hazel_target = hazel_bin_path + hazel_name

StaticLibrary(target=hazel_target, source=hazel_source_files, CPPPATH=include_path)


# -----------------------------------------------------------------------------
# -- Sandbox ------------------------------------------------------------------
# -----------------------------------------------------------------------------

sandbox_bin_path = './Sandbox/bin/'

sandbox_source_files = Glob('./Sandbox/*.cpp')
sabdbox_name = 'App'
sandbox_target = sandbox_bin_path + sabdbox_name

Program(target=sandbox_target, source=sandbox_source_files, LIBS=['hazel', 'glfw'], LIBPATH=[hazel_bin_path], CPPPATH=include_path)
