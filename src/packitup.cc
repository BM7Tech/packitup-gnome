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

#include "packitup.h"
#include "packitup_prefs.h"
#include "packitup_window.h"
#include <exception>
#include <glibmm.h>
#include <glibmm/i18n.h>
#include <gtkmm/aboutdialog.h>
#include <iostream>
#include <memory>
extern "C"
{
#include <adwaita.h>
#include <gtk/gtk.h>
}

Packitup::Packitup (AdwApplication *app) : app_ (app) {}

std::unique_ptr<Packitup>
Packitup::create (AdwApplication *app)
{
  return std::make_unique<Packitup> (app);
}

PackitupWindow *
Packitup::create_appwindow ()
{
  auto packitup_window = PackitupWindow::create ();

  return packitup_window;
}

void
Packitup::on_activate ()
{
  try
    {
      win_ = create_appwindow ();
      win_->register_with (app_);
      win_->present ();
    }
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

// vim: sts=2 sw=2 et
