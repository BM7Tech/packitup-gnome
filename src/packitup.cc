#include "packitup.h"
#include "packitup_prefs.h"
#include "packitup_window.h"
#include <exception>
#include <iostream>

Packitup::Packitup ()
    : Gtk::Application ("org.gtkmm.packitup",
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
Packitup::on_action_quit ()
{
  // Gio::Application::quit() will make Gio::Application::run() return,
  // but it's a crude way of ending the program. The window is not removed
  // from the application. Neither the window's nor the application's
  // destructors will be called, because there will be remaining reference
  // counts in both of them. If we want the destructors to be called, we
  // must remove the window from the application. One way of doing this
  // is to hide the window. See comment in create_appwindow().
  auto windows = get_windows ();
  for (auto window : windows)
    window->set_visible (false);

  // Not really necessary, when Gtk::Widget::set_visible(false) is called,
  // unless Gio::Application::hold() has been called without a corresponding
  // call to Gio::Application::release().
  quit ();
}

// vim: sts=2 sw=2 et
