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

#ifndef PACKITUP_LICENSE_H
#define PACKITUP_LICENSE_H

#include <gtkmm.h>

class PackitupLicense : public Gtk::Window
{
public:
  PackitupLicense (BaseObjectType *cobject,
                   const Glib::RefPtr<Gtk::Builder> &refBuilder);
  static PackitupLicense *create (Gtk::Window &parent);
  // void on_button_close ();

protected:
  Glib::RefPtr<Gtk::Builder> m_refLicenseBuilder;
  Glib::RefPtr<Gtk::TextBuffer> m_refLicenseBuffer;
};

#endif // PACKITUP_LICENSE_H
// vim: sts=2 sw=2 et
