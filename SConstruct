hazel_src_path = './Hazel/src/'
hazel_bin_path = './Hazel/bin/'

hazel_source_files = Glob(hazel_src_path + '*.cpp')
hazel_target = hazel_bin_path + 'hazel'

StaticLibrary(target=hazel_target, source=hazel_source_files)

sandbox_src_path = './Sandbox/'
sandbox_bin_path = './Sandbox/'

sandbox_source_files = Glob(sandbox_src_path + '*.cpp')
sandbox_target = sandbox_bin_path + 'hazel'

Program(target=sandbox_target, source=sandbox_source_files, LIBS=['hazel'], LIBPATH=[hazel_bin_path], CPPPATH=[hazel_src_path, hazel_src_path])