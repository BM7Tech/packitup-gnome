/*
 * PackItUP! Never run out of beer again.
 * Copyright (C) 2025  edu-bm7 <edubm7@bm7.dev>
 *
 * This file is part of PackItUP!.
 *
 * PackItUP! is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PackItUP! is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PackItUP!. If not, see <https://www.gnu.org/licenses/>.
 * */

#ifndef PACKITUP_WINDOW_H
#define PACKITUP_WINDOW_H

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
  void update_revealer_transition ();
  void reload_all_css ();
  void reload_app_css ();
  void reload_theme_css ();
  Glib::ustring find_theme_css_path (const Glib::ustring &theme);

protected:
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::Settings> m_refSettings;
  Glib::RefPtr<Gtk::TextTag> m_refTagFont;
  Glib::RefPtr<Gtk::TextBuffer> m_refBuffer;
  Glib::RefPtr<Gtk::CssProvider> m_refAppCssProvider;
  Glib::RefPtr<Gtk::CssProvider> m_refThemeCssProvider;
  Glib::RefPtr<Gtk::StringList> m_bottle_size_list;
  Glib::RefPtr<Gtk::StringList> m_pack_size_list;
  Glib::RefPtr<Gtk::Settings> m_gtkSettings;
  Gtk::Revealer m_revealer;
  Gtk::Box *m_result_VBox{ nullptr };
  Gtk::Button *m_result_button{ nullptr };
  Gtk::DropDown *m_unit_dropdown{ nullptr };
  Gtk::DropDown *m_pack_size_dropdown{ nullptr };
  Gtk::DropDown *m_bottle_size_dropdown{ nullptr };
  Gtk::HeaderBar *m_header{ nullptr };

  Gtk::MenuButton *m_gears{ nullptr };
  Gtk::SpinButton *m_spin_button_more{ nullptr };
  Gtk::SpinButton *m_spin_button_alright{ nullptr };
  static void
  on_parsing_error (const Glib::RefPtr<const Gtk::CssSection> &section,
                    const Glib::Error &error);

private:
  int total_people_number;
  float amount_of_beer;
  float amount_of_beer_packs;
  float number_of_packs;
  std::string unit;
  std::string m_buffer;
  bool m_transition_in_progress = false;
};

#endif // PACKITUPWINDOW_H
// vim: sts=2 sw=2 et
