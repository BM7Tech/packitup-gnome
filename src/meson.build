# PackItUP! Never run out of beer again.
# Copyright (C) 2025  edu-bm7 <edubm7@bm7.dev>
# 
# This file is part of PackItUP!.
#
# PackItUP! is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# PackItUP! is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with PackItUP!. If not, see <https://www.gnu.org/licenses/>.

program_name = 'packitup'

schemas_dir =  get_option('prefix') / get_option('datadir') / 'glib-2.0' / 'schemas'
install_data(
  'dev.bm7.packitup.gschema.xml',
  install_dir: schemas_dir )

gnome = import('gnome')
gtkmm_dep = dependency('gtkmm-4.0')

resources = gnome.compile_resources('resources',
                                    'packitup.gresource.xml',
                                    source_dir: '.')

schemas_file = files('dev.bm7.packitup.gschema.xml')
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
  dependencies: [
    dependency('glibmm-2.68'),
    dependency('giomm-2.68'),
    dependency('gtkmm-4.0'),
    dependency('intl'),
  ],
  install: true
)

bin_dir = get_option('prefix') / get_option('bindir')

conf_data = configuration_data()
conf_data.set('bindir', bin_dir)
desktop_file = configure_file(
  input: 'packitup.desktop.in',
  output: '@BASENAME@',
  configuration: conf_data,
)

apps_dir = get_option('prefix') / get_option('datadir') / 'applications'

install_data(
  desktop_file,
  install_dir: apps_dir
)
