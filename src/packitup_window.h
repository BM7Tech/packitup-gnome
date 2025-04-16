#ifndef GTKMM_PACKITUP_WINDOW_H
#define GTKMM_PACKITUP_WINDOW_H

#include "gtkmm/revealer.h"
#include "gtkmm/stringlist.h"
#include <gtkmm.h>

class PackitupWindow : public Gtk::ApplicationWindow
{
public:
  PackitupWindow (BaseObjectType *cobject,
                  const Glib::RefPtr<Gtk::Builder> &refBuilder);

  static PackitupWindow *create ();

  void open_result_view ();
  void on_result_clicked ();
  void on_result_changed ();
  void on_unit_dropdown_changed ();

protected:
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::Settings> m_refSettings;
  Glib::RefPtr<Gtk::TextMark> m_refMark;
  Glib::RefPtr<Gtk::TextTag> m_refTagFont;
  Glib::RefPtr<Gtk::TextBuffer> m_refBuffer;
  Glib::RefPtr<Gtk::CssProvider> m_refCssProvider;
  Glib::RefPtr<Gtk::StringList> m_bottle_size_list;
  Glib::RefPtr<Gtk::StringList> m_pack_size_list;
  Gtk::Revealer m_revealer;
  Gtk::Box *m_result_VBox{ nullptr };
  Gtk::Button *m_result_button{ nullptr };
  Gtk::DropDown *m_unit_dropdown{ nullptr };
  Gtk::DropDown *m_pack_size_dropdown{ nullptr };
  Gtk::DropDown *m_bottle_size_dropdown{ nullptr };

  Gtk::MenuButton *m_gears{ nullptr };
  Gtk::SpinButton *m_spin_button_more{ nullptr };
  Gtk::SpinButton *m_spin_button_alright{ nullptr };
  static void
  on_parsing_error (const Glib::RefPtr<const Gtk::CssSection> &section,
                    const Glib::Error &error);

private:
  int total_people_number;
  float amount_of_beer;
  float number_of_packs;
  std::string unit;
  std::string m_buffer;
  bool m_transition_in_progress = false;
};

#endif // PACKITUPWINDOW_H
// vim: sts=2 sw=2 et
