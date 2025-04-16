#ifndef GTKMM_PACKITUP_PREFS_H
#define GTKMM_PACKITUP_PREFS_H

#include <gtkmm.h>

#ifdef GLIBMM_CHECK_VERSION
#define HAS_GIO_SETTINGS_BIND_WITH_MAPPING GLIBMM_CHECK_VERSION (2, 75, 0)
#else
#define HAS_GIO_SETTINGS_BIND_WITH_MAPPING 0
#endif

class PackitupPrefs : public Gtk::Window
{
public:
  PackitupPrefs (BaseObjectType *cobject,
                 const Glib::RefPtr<Gtk::Builder> &refBuilder);

  static PackitupPrefs *create (Gtk::Window &parent);
  void on_button_close ();

protected:
#if HAS_GIO_SETTINGS_BIND_WITH_MAPPING
  // Mappings from Gio::Settings to properties
  static std::optional<unsigned int>
  map_from_ustring_to_int (const Glib::ustring &transition);
  static std::optional<Glib::ustring>
  map_from_int_to_ustring (const unsigned int &pos);
#else
  // Signal handlers
  void on_font_setting_changed (const Glib::ustring &key);
  void on_font_selection_changed ();
  void on_transition_setting_changed (const Glib::ustring &key);
  void on_transition_selection_changed ();
#endif
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::Settings> m_settings;
  Gtk::FontDialogButton *m_font{ nullptr };
  Gtk::DropDown *m_transition{ nullptr };
  Gtk::Button *m_prefs_close{ nullptr };
};

#endif // PACKITUP_PREFS_H
// vim: sts=2 sw=2 et
