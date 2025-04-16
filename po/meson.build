i18n = import('i18n')
locale_dir = get_option('prefix') / get_option('localedir')
# define GETTEXT_PACKAGE
add_project_arguments('-DGETTEXT_PACKAGE="packitup"', '-DLOCALE_DIR="@0@"'.format(locale_dir), language:'cpp')
i18n.gettext(meson.project_name(),
  args: [
    '--from-code=UTF-8',
    '--keyword=_',  # Ensure it catches gettext() calls
  ],
  install: true,
  install_dir: locale_dir)
