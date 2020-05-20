hazel_src_path = './Hazel/src/Hazel/'
hazel_bin_path = './Hazel/bin/'
spdlog_include_path = './Hazel/vendor/spdlog/include/'

hazel_source_files = Glob(hazel_src_path + '*.cpp')
hazel_target = hazel_bin_path + 'hazel'

StaticLibrary(target=hazel_target, source=hazel_source_files, CPPPATH=spdlog_include_path)

sandbox_src_path = './Sandbox/'
sandbox_bin_path = './Sandbox/bin/'

sandbox_source_files = Glob(sandbox_src_path + '*.cpp')
sandbox_target = sandbox_bin_path + 'App'

Program(target=sandbox_target, source=sandbox_source_files, LIBS=['hazel'], LIBPATH=[hazel_bin_path], CPPPATH=['./Hazel/src/', spdlog_include_path])