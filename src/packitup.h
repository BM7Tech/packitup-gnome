/*
 * PackItUP! Never run out of beer again.
 * Copyright (C) 2025  edu-bm7 <edubm7@bm7.tech>
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

#include <gtkmm/application.h>
#include <gtkmm/cssprovider.h>
extern "C"
{
#include <adwaita.h>
}

class PackitupWindow;

class Packitup
{
public:
  Packitup (AdwApplication *app);
  static std::unique_ptr<Packitup> create (AdwApplication *app);

  // Override default signal handlers
  void on_activate ();

protected:
  Glib::RefPtr<Gtk::CssProvider> m_refCssProvider;

private:
  AdwApplication *const app_;
  PackitupWindow *create_appwindow ();
  PackitupWindow *win_;
};

#endif // PACKITUP_H
// vim: sts=2 sw=2 et
