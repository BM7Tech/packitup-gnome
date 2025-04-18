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

#include "packitup_license.h"
#include "gtkmm/scrolledwindow.h"
#include "packitup_window.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

PackitupLicense::PackitupLicense (BaseObjectType *cobject,
                                  const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::Window (cobject), m_refLicenseBuilder (refBuilder)
{
  auto license_box = m_refLicenseBuilder->get_widget<Gtk::Box> ("box");
  if (!license_box)
    throw std::runtime_error ("no \"box\" object in license.ui");

  auto scrolled = m_refLicenseBuilder->get_widget<Gtk::ScrolledWindow> (
      "scrolled_window");
  if (!scrolled)
    throw std::runtime_error (" no \"scrolled_window\" object in lencense.ui");

  auto label = m_refLicenseBuilder->get_widget<Gtk::Label> ("license_label");
  if (!label)
    throw std::runtime_error (" no \"license_label\" object in lencense.ui");

  set_child (*license_box);
  // auto scrolled_window = Gtk::make_managed<Gtk::ScrolledWindow> ();
  // // Only show the scrollbars when they are necessary:
  scrolled->set_policy (Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::ALWAYS);
  scrolled->set_expand ();
  label->set_expand ();
  // scrolled->set_child(*label);
  // auto view = Gtk::make_managed<Gtk::TextView> ();
  // view->set_editable (false);
  // view->set_cursor_visible (false);
  // view->set_expand ();
  // scrolled_window->set_child (*view);
  // license_box->append (*scrolled_window);
  // auto buffer = view->get_buffer ();
}
// static
PackitupLicense *
PackitupLicense::create (Gtk::Window &parent)
{
  auto refBuilder = Gtk::Builder::create_from_resource (
      "/dev/bm7/packitup/src/license.ui");

  auto dialog = refBuilder->get_widget_derived<PackitupLicense> (
      refBuilder, "license_dialog");
  if (!dialog)
    throw std::runtime_error ("No \"license_dialog\" object in license.ui");

  dialog->set_transient_for (parent);

  return dialog;
}
// vim: sts=2 sw=2 et
