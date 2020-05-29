hazel_bin_path = './Hazel/bin/'
hazel_include_path = './Hazel/src/'
spdlog_include_path = './Hazel/vendor/spdlog/include/'
include_path = [hazel_include_path, spdlog_include_path]

hazel_source_files = Glob('./Hazel/src/Hazel/*.cpp') + Glob('./Hazel/src/Platform/*.cpp')
hazel_target = hazel_bin_path + 'hazel'

StaticLibrary(target=hazel_target, source=hazel_source_files, CPPPATH=include_path)

sandbox_src_path = './Sandbox/'
sandbox_bin_path = './Sandbox/bin/'

sandbox_source_files = Glob(sandbox_src_path + '*.cpp')
sandbox_target = sandbox_bin_path + 'App'

Program(target=sandbox_target, source=sandbox_source_files, LIBS=['hazel', 'glfw'], LIBPATH=[hazel_bin_path], CPPPATH=['./Hazel/src/', spdlog_include_path])