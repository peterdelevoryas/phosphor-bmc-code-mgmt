executable(
  'phosphor-cpld-fw-manager',
  'main.cpp',
  dependencies: [
    libpldm_dep,
    pdi_dep,
    phosphor_logging_dep,
    sdbusplus_dep,
  ],
  include_directories: [
    common_include,
  ],
  link_with: [libpldmutil, software_common_lib],
  install: true,
)
