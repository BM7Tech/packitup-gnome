
program_name = 'packitup'

schemas_dir =  get_option('prefix') / get_option('datadir') / 'glib-2.0' / 'schemas'
install_data(
  'org.gtkmm.packitup.gschema.xml',
  install_dir: schemas_dir )

gnome = import('gnome')
gtkmm_dep = dependency('gtkmm-4.0')

resources = gnome.compile_resources('resources',
                                    'packitup.gresource.xml',
                                    source_dir: '.')

schemas_file = files('org.gtkmm.packitup.gschema.xml')
install_data(
  'icons/hicolor/16x16/apps/packitup.png',
  install_dir: get_option('datadir') / 'icons/hicolor/16x16/apps'
)
install_data(
  'icons/hicolor/22x22/apps/packitup.png',
  install_dir: get_option('datadir') / 'icons/hicolor/22x22/apps'
)
install_data(
  'icons/hicolor/24x24/apps/packitup.png',
  install_dir: get_option('datadir') / 'icons/hicolor/24x24/apps'
)
install_data(
  'icons/hicolor/32x32/apps/packitup.png',
  install_dir: get_option('datadir') / 'icons/hicolor/32x32/apps'
)
install_data(
  'icons/hicolor/64x64/apps/packitup.png',
  install_dir: get_option('datadir') / 'icons/hicolor/64x64/apps'
)
install_data(
  'icons/hicolor/128x128/apps/packitup.png',
  install_dir: get_option('datadir') / 'icons/hicolor/128x128/apps'
)
install_data(
  'icons/hicolor/256x256/apps/packitup.png',
  install_dir: get_option('datadir') / 'icons/hicolor/256x256/apps'
)
install_data(
  'icons/hicolor/512x512/apps/packitup.png',
  install_dir: get_option('datadir') / 'icons/hicolor/512x512/apps'
)
install_data(
  'icons/hicolor/16x16/actions/window-close.svg',
  install_dir: get_option('datadir') / 'icons/hicolor/16x16/actions'
)
install_data(
  'icons/hicolor/22x22/actions/window-close.svg',
  install_dir: get_option('datadir') / 'icons/hicolor/22x22/actions'
)
install_data(
  'icons/hicolor/24x24/actions/window-close.svg',
  install_dir: get_option('datadir') / 'icons/hicolor/24x24/actions'
)
schemas = gnome.compile_schemas(depend_files: schemas_file)

meson.add_install_script('glib-compile-schemas', schemas_dir)

cpp_sources = [
  'main.cc',
  'packitup.cc',
  'packitup_window.cc',
  'packitup_prefs.cc',
]

executable(program_name,
  cpp_sources, resources, schemas,
  dependencies: gtkmm_dep,
  win_subsystem: 'windows',
)

python3 = import('python').find_installation()
install_cmd = meson.current_source_dir() / 'install-cmd.py'

conf_data = configuration_data()
conf_data.set('bindir', meson.current_build_dir())
desktop_file = configure_file(
  input: 'packitup.desktop.in',
  output: '@BASENAME@',
  configuration: conf_data,
)

run_target('install-desktop-file',
  command: [
    python3, install_cmd, 'desktop',
    program_name,
    desktop_file,
    meson.current_source_dir() / 'packitup.png',
  ]
)

run_target('install-gschema-file',
  command: [
    python3, install_cmd, 'gschema',
    meson.current_source_dir() / 'org.gtkmm.packitup.gschema.xml',
  ]
)
