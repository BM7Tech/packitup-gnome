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

#include "packitup.h"
#include "packitup_license.h"
#include "packitup_prefs.h"
#include "packitup_window.h"
#include <exception>
#include <iostream>

Packitup::Packitup ()
    : Gtk::Application ("dev.bm7.packitup",
                        Gio::Application::Flags::HANDLES_OPEN)
{
}

Glib::RefPtr<Packitup>
Packitup::create ()
{
  return Glib::make_refptr_for_instance<Packitup> (new Packitup ());
}

PackitupWindow *
Packitup::create_appwindow ()
{
  auto packitup_window = PackitupWindow::create ();

  // same as set_application, manages lifecycle of windows
  add_window (*packitup_window);

  // Closes app when all windows are gone
  packitup_window->signal_hide ().connect (
      [packitup_window] () { delete packitup_window; });

  return packitup_window;
}

void
Packitup::on_activate ()
{
  try
    {
      // The application has been started, so let's show a window.
      auto appwindow = create_appwindow ();
      appwindow->present ();
    }
  // If create_appwindow() throws an exception (perhaps from Gtk::Builder),
  // no window has been created, no window has been added to the application,
  // and therefore the application will stop running.
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error Packitup::on_activate(): " << ex.what ()
                << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception Packitup::on_activate(): " << ex.what ()
                << std::endl;
    }
}

void
Packitup::on_startup ()
{
  // base class implementation
  Gtk::Application::on_startup ();

  // Actions and keyboard accelerators for the menu
  add_action ("preferences",
              sigc::mem_fun (*this, &Packitup::on_action_preferences));
  add_action ("license", sigc::mem_fun (*this, &Packitup::on_action_license));
  add_action ("quit", sigc::mem_fun (*this, &Packitup::on_action_quit));
  set_accel_for_action ("app.quit", "<Ctrl>Q");
}

void
Packitup::on_action_preferences ()
{
  try
    {
      auto prefs_dialog = PackitupPrefs::create (*get_active_window ());
      prefs_dialog->present ();

      // Delete when its hidden
      prefs_dialog->signal_hide ().connect (
          [prefs_dialog] () { delete prefs_dialog; });
    }
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error: Packitup::on_action_preferences(): "
                << ex.what () << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception: Packitup::on_action_preferences(): "
                << ex.what () << std::endl;
    }
}

void
Packitup::on_action_license ()
{
  try
    {
      auto license_window = PackitupLicense::create (*get_active_window ());
      license_window->present ();

      // Delete when its hidden
      license_window->signal_hide ().connect (
          [license_window] () { delete license_window; });
    }
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error: Packitup::on_action_license(): " << ex.what ()
                << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception: Packitup::on_action_license(): "
                << ex.what () << std::endl;
    }
}

void
Packitup::on_action_quit ()
{
  auto windows = get_windows ();
  for (auto window : windows)
    window->set_visible (false);

  quit ();
}

// vim: sts=2 sw=2 et
