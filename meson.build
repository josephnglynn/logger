check_for_logger = get_variable('check_for_logger', false)


if check_for_logger == false
   logger_includes = include_directories('.')
   logger_sources = ['logger/logger.cpp']
   logger_deps = []
   
   arg = ''
   if get_option('buildtype').startswith('debug')
      arg = '-DDEBUG'
   endif


   check_for_logger = true

   logger = shared_library(
       'logger',
        logger_sources,
        include_directories : logger_includes,
        dependencies: logger_deps,
        cpp_args: arg
   )

endif

