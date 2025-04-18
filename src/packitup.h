/*
 * PackItUP! Never run out of beer again.
 * Copyright (C) 2025  edu-bm7
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

#ifndef PACKITUP_H
#define PACKITUP_H

#include <gtkmm.h>

class PackitupWindow;

class Packitup : public Gtk::Application
{
protected:
  Packitup (); // Canonical Default Constructor

public:
  static Glib::RefPtr<Packitup> create ();

protected:
  // Override default signal handlers
  void on_startup () override;
  void on_activate () override;
  Glib::RefPtr<Gtk::CssProvider> m_refCssProvider;

private:
  PackitupWindow *create_appwindow ();
  void on_action_quit ();
  void on_action_preferences ();
  void on_action_license ();
};

#endif // GTKMM_PACKITUP_H
// vim: sts=2 sw=2 et
