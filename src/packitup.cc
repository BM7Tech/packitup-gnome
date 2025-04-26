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

  packitup_window->register_with (app_);

  return packitup_window;
}

void
Packitup::on_activate ()
{
  try
    {
      win_ = create_appwindow ();
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

void
Packitup::on_startup ()
{

  static GActionEntry app_entries[] = {
    { "preferences", on_action_preferences, NULL, NULL, NULL },
    { "about", on_action_about, NULL, NULL, NULL },
    { "quit", on_action_quit, NULL, NULL, NULL },
  };
  g_action_map_add_action_entries (G_ACTION_MAP (app_), app_entries,
                                   G_N_ELEMENTS (app_entries), app_);

  const char *quit_accels[2] = { "<Ctrl>Q", NULL };
  gtk_application_set_accels_for_action (GTK_APPLICATION (app_), "app.quit",
                                         quit_accels);
}

void
Packitup::on_action_preferences (GSimpleAction *action, GVariant *state,
                                 gpointer user_data)
{
  try
    {
      GtkApplication *app = GTK_APPLICATION (user_data);
      GtkWindow *window_ref
          = gtk_application_get_active_window (GTK_APPLICATION (app));

      auto cpp_wrap = Glib::wrap (window_ref);
      auto prefs_dialog = PackitupPrefs::create (*cpp_wrap);
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
Packitup::on_action_about (GSimpleAction *action, GVariant *state,
                           gpointer user_data)
{
  try
    {
      auto refBuilder = Gtk::Builder::create_from_resource (
          "/tech/bm7/packitup-gnome/src/about.ui");
      auto dialog = refBuilder->get_widget<Gtk::AboutDialog> ("about_window");

      GtkApplication *app = GTK_APPLICATION (user_data);
      GtkWindow *active_window
          = gtk_application_get_active_window (GTK_APPLICATION (app));
      auto cpp_wrap = Glib::wrap (active_window);

      dialog->set_transient_for (*cpp_wrap);
      dialog->set_program_name ("PackItUP!");
      dialog->set_copyright ("Copyright (C) 2025  edu-bm7 <edubm7@bm7.tech>");
      dialog->set_logo (Gdk::Texture::create_from_resource (
          "/tech/bm7/packitup-gnome/src/packitup.png"));
      dialog->set_license_type (Gtk::License::GPL_3_0);
      dialog->set_wrap_license (true);
      dialog->set_comments (_ ("Never run out of beer again."));
      dialog->set_website ("https://github.com/BM7Tech/packitup.git");
      dialog->set_website_label ("PackItUP! Website");
      std::vector<Glib::ustring> list_authors;
      list_authors.push_back ("edu-bm7 https://github.com/edu-bm7");
      list_authors.push_back ("BM7Tech https://github.com/BM7Tech");
      dialog->set_authors (list_authors);
      dialog->set_visible (true);
      dialog->present ();
      dialog->signal_hide ().connect ([dialog] () { delete dialog; });
    }
  catch (const Glib::Error &ex)
    {
      std::cerr << "Glib::Error: Packitup::on_action_about(): " << ex.what ()
                << std::endl;
    }
  catch (const std::exception &ex)
    {
      std::cerr << "std::exception: Packitup::on_action_about(): "
                << ex.what () << std::endl;
    }
}

void
Packitup::on_action_quit (GSimpleAction *action, GVariant *state,
                          gpointer user_data)
{
  GtkApplication *app = GTK_APPLICATION (user_data);
  auto windows = gtk_application_get_windows (app);
  GList *window;
  window = windows;
  while (window)
    {
      gtk_widget_set_visible (GTK_WIDGET (window), false);
      window = window->next;
    }

  g_application_quit (G_APPLICATION (app));
}

// vim: sts=2 sw=2 et
