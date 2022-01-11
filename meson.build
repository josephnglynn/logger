check_for_logger = get_variable('check_for_logger', false)


if check_for_logger == false
   logger_includes = include_directories('.')
   logger_sources = ['logger/logger.cpp']
   logger_deps = []

   check_for_logger = true

   logger = shared_library(
       'logger',
        logger_sources,
        include_directories : logger_includes,
        dependencies: logger_deps,
   )

endif

