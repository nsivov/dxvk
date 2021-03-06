project('dxvk', ['c', 'cpp'], version : 'v0.70')

cpu_family = target_machine.cpu_family()

add_project_arguments('-DNOMINMAX', language : 'cpp')

dxvk_compiler = meson.get_compiler('cpp')
if dxvk_compiler.get_id() == 'msvc'
  dxvk_cpp_std='c++latest'
  dxvk_msvc=true
else
  dxvk_cpp_std='c++1z'
  dxvk_msvc=false
endif

if dxvk_compiler.get_id() == 'msvc'
  add_project_arguments('/std:' + dxvk_cpp_std, language : 'cpp')
endif

dxvk_include_path = include_directories('./include')

if (cpu_family == 'x86_64')
  dxvk_library_path = meson.source_root() + '/lib'
else
  dxvk_library_path = meson.source_root() + '/lib32'
endif

if meson.get_cross_property('winelib', false)
  lib_vulkan  = declare_dependency(link_args: [ '-lwinevulkan' ])
  lib_d3d11   = declare_dependency(link_args: [ '-ld3d11' ])
  lib_dxgi    = declare_dependency(link_args: [ '-ldxgi' ])
  lib_d3dcompiler_43 = declare_dependency(link_args: [ '-L'+dxvk_library_path, '-ld3dcompiler_43' ])
  lib_d3dcompiler_47 = declare_dependency(link_args: [ '-ld3dcompiler_47' ])
  exe_ext = '.exe.so'
  dll_ext = '.dll'
  def_spec_ext = '.spec'
else
  lib_vulkan  = dxvk_compiler.find_library('vulkan-1', dirs : dxvk_library_path)
  lib_d3d11   = dxvk_compiler.find_library('d3d11')
  lib_dxgi    = dxvk_compiler.find_library('dxgi')
  lib_d3dcompiler_43 = dxvk_compiler.find_library('d3dcompiler_43', dirs : dxvk_library_path)
  
  if dxvk_compiler.get_id() == 'msvc'
    lib_d3dcompiler_47 = dxvk_compiler.find_library('d3dcompiler')
  else
    lib_d3dcompiler_47 = dxvk_compiler.find_library('d3dcompiler_47')
  endif
  exe_ext = ''
  dll_ext = ''
  def_spec_ext = '.def'
endif

glsl_compiler = find_program('glslangValidator')
glsl_generator = generator(glsl_compiler,
  output    : [ '@BASENAME@.h' ],
  arguments : [ '-V', '--vn', '@BASENAME@', '@INPUT@', '-o', '@OUTPUT@' ])

dxvk_version = vcs_tag(
  command: ['git', 'describe'],
  input:  'version.h.in',
  output: 'version.h')

subdir('src')

enable_tests = get_option('enable_tests')

if enable_tests
  subdir('tests')
endif

if dxvk_compiler.get_id() != 'msvc'
  subdir('utils')
endif
