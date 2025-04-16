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
};

#endif // GTKMM_PACKITUP_H
// vim: sts=2 sw=2 et
