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

#include "packitup_prefs.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include "gtkmm/builder.h"
#include "packitup_window.h"
#include <array>
#include <glibmm/i18n.h>
#include <stdexcept>
#include <string>
extern "C"
{
#include <adwaita.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
}

namespace
{

struct TransitionTypeStruct
{
  Glib::ustring id;
  Glib::ustring text;
};

const std::array<TransitionTypeStruct, 3> transitionTypes = {
  TransitionTypeStruct{ "crossfade", "Fade" },
  TransitionTypeStruct{ "slide-right", "Slide Right" },
  TransitionTypeStruct{ "slide-down", "Slide Down" },
};
}

typedef struct TransitionTypeCStruct
{
  const char *id;
  const char *text;
} TransitionCTypeStruct;

PackitupPrefs::PackitupPrefs (AdwApplicationWindow *parent, AdwDialog *dialog,
                              const Glib::RefPtr<Gtk::Builder> &builder)
    : parent_ (parent), dialog_ (dialog), m_refBuilder (builder)
{
  m_font = m_refBuilder->get_widget<Gtk::FontDialogButton> ("font");
  if (!m_font)
    throw std::runtime_error ("no \"font\" object in prefs.ui");

  m_transition = m_refBuilder->get_widget<Gtk::DropDown> ("transition");
  if (!m_transition)
    throw std::runtime_error ("no \"transition\" object in prefs.ui");

  m_prefs_close = m_refBuilder->get_widget<Gtk::Button> ("prefs_close");
  if (!m_prefs_close)
    throw std::runtime_error ("no \"prefs_close\" object in prefs.ui");
  m_prefs_close->set_expand (true);

  m_prefs_close->signal_clicked ().connect (
      sigc::mem_fun (*this, &PackitupPrefs::on_button_close));

  const char *transition_fade = _ ("Fade");
  const char *transition_slideRight = _ ("Slide Right");
  const char *transition_slideDown = _ ("Slide Down");

  auto string_list = Gtk::StringList::create ();
  // So we can translate the text
  string_list->append (transition_fade);
  string_list->append (transition_slideRight);
  string_list->append (transition_slideDown);

  m_transition->set_model (string_list);

  m_settings = Gio::Settings::create ("tech.bm7.packitup-gnome");

  // Connect preferences properties to the Gio::Settings.
#if HAS_GIO_SETTINGS_BIND_WITH_MAPPING
  m_settings->bind<Glib::ustring, Pango::FontDescription> (
      "font", m_font->property_font_desc (), Gio::Settings::BindFlags::DEFAULT,
      [] (const auto &font) { return Pango::FontDescription (font); },
      [] (const auto &fontdesc) { return fontdesc.to_string (); });
  m_settings->bind<Glib::ustring, unsigned int> (
      "transition", m_transition->property_selected (),
      Gio::Settings::BindFlags::DEFAULT,
      [] (const auto &transition) {
        return map_from_ustring_to_int (transition);
      },
      [] (const auto &pos) { return map_from_int_to_ustring (pos); });
#else
  m_settings->signal_changed ("font").connect (
      sigc::mem_fun (*this, &PackitupPrefs::on_font_setting_changed));
  m_font->property_font_desc ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupPrefs::on_font_selection_changed));

  m_settings->signal_changed ("transition")
      .connect (sigc::mem_fun (*this,
                               &PackitupPrefs::on_transition_setting_changed));
  m_transition->property_selected ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupPrefs::on_transition_selection_changed));

  on_font_setting_changed ("font");
  on_transition_setting_changed ("transition");
#endif
}

// static
PackitupPrefs *
PackitupPrefs::create (AdwApplicationWindow *parent)
{
  auto refBuilder = Gtk::Builder::create_from_resource (
      "/tech/bm7/packitup-gnome/src/prefs.ui");
  auto c_refBuilder = refBuilder->gobj ();
  auto adw_dialogWindow
      = gtk_builder_get_object (c_refBuilder, "prefs_dialog");
  AdwDialog *raw_dialog = ADW_DIALOG (adw_dialogWindow);
  if (!raw_dialog)
    throw std::runtime_error ("No \"prefs_dialog\" object in prefs.ui");
  g_object_ref (raw_dialog);

  auto *self = new PackitupPrefs (parent, raw_dialog, refBuilder);

  g_signal_connect (G_OBJECT (self->dialog_), "hide",
                    G_CALLBACK (+[] (AdwDialog *dialog, gpointer user_data) {
                      delete static_cast<PackitupPrefs *> (user_data);
                    }),
                    self);

  return self;
}

void
PackitupPrefs::on_button_close ()
{
  adw_dialog_close (dialog_);
}

void
PackitupPrefs::present ()
{
  adw_dialog_present (dialog_, GTK_WIDGET (parent_));
}

#if HAS_GIO_SETTINGS_BIND_WITH_MAPPING
std::optional<unsigned int>
PackitupPrefs::map_from_ustring_to_int (const Glib::ustring &transition)
{
  for (std::size_t i = 0; i < transitionTypes.size (); ++i)
    {
      if (transitionTypes[i].id == transition)
        {
          return i;
        }
    }
  return std::nullopt;
}

std::optional<Glib::ustring>
PackitupPrefs::map_from_int_to_ustring (const unsigned int &pos)
{
  if (pos >= transitionTypes.size ())
    return std::nullopt;
  return transitionTypes[pos].id;
}
#else
void
PackitupPrefs::on_font_setting_changed (const Glib::ustring &key)
{
  const auto font_settings = m_settings->get_string ("font");
  const auto font_button = m_font->get_font_desc ().to_string ();
  if (font_settings != font_button)
    m_font->set_font_desc (Pango::FontDescription (font_settings));
}
void
PackitupPrefs::on_font_selection_changed ()
{
  const auto font_settings = m_settings->get_string ("font");
  const auto font_button = m_font->get_font_desc ().to_string ();
  if (font_settings != font_button)
    m_settings->set_string ("font", font_button);
}
void
PackitupPrefs::on_transition_setting_changed (const Glib::ustring &key)
{
  const auto transition_setting = m_settings->get_string ("transition");
  const auto transition_button
      = transitionTypes[m_transition->get_selected ()].id;

  if (transition_setting != transition_button)
    {
      for (std::size_t i = 0; i < transitionTypes.size (); ++i)
        {
          if (transitionTypes[i].id == transition_setting)
            {
              m_transition->set_selected (i);
              break;
            }
        }
    }
}
void
PackitupPrefs::on_transition_selection_changed ()
{
  const auto pos = m_transition->get_selected ();
  if (pos >= transitionTypes.size ())
    return;
  const auto transition_setting = m_settings->get_string ("transition");
  const auto transition_button = transitionTypes[pos].id;
  if (transition_setting != transition_button)
    m_settings->set_string ("transition", transition_button);
}
#endif
// vim: sts=2 sw=2 et
