project(
        'logger',
        'cpp',
        version: '0.0.1', 
        default_options : [
            'warning_level=3',
            'cpp_std=c++17'
        ]
)

logger_includes = include_directories('.')
logger_sources = ['logger/logger.cpp']
logger_deps = []
   
arg = ''
if get_option('buildtype').startswith('debug')
   arg = '-DDEBUG'
endif


logger = shared_library(
    'logger',
    logger_sources,
    include_directories : logger_includes,
    dependencies: logger_deps,
    cpp_args: arg
)


logger_dep = declare_dependency(include_directories: logger_includes, link_with: logger)
