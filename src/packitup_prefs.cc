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

const TransitionTypeCStruct transitionTypesC[] = {
  { "crossfade", "Fade" },
  { "slide-right", "Slide Right" },
  { "slide-down", "Slide Down" },
};
#define N_TRANSITIONS G_N_ELEMENTS (transitionTypesC)

static void on_combo_notify_selected (GObject *obj, GParamSpec *pspec,
                                      gpointer user_data);

static void on_transition_changed (GSettings *settings, gchar *key,
                                   gpointer user_data);

static GVariant *transition_set_mapping (const GValue *value,
                                         const GVariantType *expected_type,
                                         gpointer user_data);

static gboolean transition_get_mapping (GValue *value, GVariant *variant,
                                        gpointer user_data);

PackitupPrefs::PackitupPrefs (BaseObjectType *cobject,
                              const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::Window (cobject), m_refBuilder (refBuilder)
{

  m_font = m_refBuilder->get_widget<Gtk::FontDialogButton> ("font");
  if (!m_font)
    throw std::runtime_error ("no \"font\" object in window.ui");

  m_transition = m_refBuilder->get_widget<Gtk::DropDown> ("transition");
  if (!m_transition)
    throw std::runtime_error ("no \"transition\" object in window.ui");

  m_rawTransition = ADW_COMBO_ROW (m_transition->gobj ());
  if (!m_rawTransition)
    throw std::runtime_error (
        "no \"Adwaita Combo Row transition\" in window.ui");

  m_prefs_close = m_refBuilder->get_widget<Gtk::Button> ("prefs_close");
  if (!m_prefs_close)
    throw std::runtime_error ("no \"prefs_close\" object in window.ui");
  m_prefs_close->set_expand (true);

  m_prefs_close->signal_clicked ().connect (
      sigc::mem_fun (*this, &PackitupPrefs::on_button_close));

  char *listArray[]
      = { _ ("Fade"), _ ("Slide Right"), _ ("Slide Down"), NULL };

  auto raw_StringList = gtk_string_list_new ((const char *const *)listArray);

  adw_combo_row_set_model (m_rawTransition, G_LIST_MODEL (raw_StringList));

  m_font_settings = Gio::Settings::create ("tech.bm7.packitup-gnome");
  GSettings *transition_settings = g_settings_new ("tech.bm7.packitup-gnome");

  g_settings_bind_with_mapping (
      transition_settings, "transition", G_OBJECT (m_rawTransition),
      "selected", G_SETTINGS_BIND_DEFAULT, transition_get_mapping,
      transition_set_mapping, NULL, NULL);
  g_signal_connect (transition_settings, "changed::transition",
                    G_CALLBACK (on_transition_changed), m_rawTransition);
  g_signal_connect (m_rawTransition, "notify::selected",
                    G_CALLBACK (on_combo_notify_selected),
                    transition_settings);

  // Connect preferences properties to the Gio::Settings.
#if HAS_GIO_SETTINGS_BIND_WITH_MAPPING
  m_font_settings->bind<Glib::ustring, Pango::FontDescription> (
      "font", m_font->property_font_desc (), Gio::Settings::BindFlags::DEFAULT,
      [] (const auto &font) { return Pango::FontDescription (font); },
      [] (const auto &fontdesc) { return fontdesc.to_string (); });
#else
  m_font_settings->signal_changed ("font").connect (
      sigc::mem_fun (*this, &PackitupPrefs::on_font_setting_changed));
  m_font->property_font_desc ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupPrefs::on_font_selection_changed));

  on_font_setting_changed ("font");
#endif
}

// static
PackitupPrefs *
PackitupPrefs::create (Gtk::Window &parent)
{
  auto refBuilder = Gtk::Builder::create_from_resource (
      "/tech/bm7/packitup-gnome/src/prefs.ui");

  auto dialog = refBuilder->get_widget_derived<PackitupPrefs> (refBuilder,
                                                               "prefs_dialog");
  if (!dialog)
    throw std::runtime_error ("No \"prefs_dialog\" object in prefs.ui");

  dialog->set_transient_for (parent);

  return dialog;
}

void
PackitupPrefs::on_button_close ()
{
  set_visible (false);
}

static gboolean
transition_get_mapping (GValue *value, GVariant *variant, gpointer user_data)
{
  const char *str = g_variant_get_string (variant, NULL);
  guint idx = 0;
  for (guint i = 0; i < N_TRANSITIONS; ++i)
    {
      if (g_strcmp0 (transitionTypesC[i].id, str) == 0)
        {
          idx = i;
          break;
        }
    }
  g_value_set_uint (value, idx);
  return TRUE;
}

static GVariant *
transition_set_mapping (const GValue *value, const GVariantType *expected_type,
                        gpointer user_data)
{
  guint idx = g_value_get_uint (value);
  if (idx >= N_TRANSITIONS)
    idx = 0;
  return g_variant_new_string (transitionTypesC[idx].id);
}

static void
on_transition_changed (GSettings *settings, gchar *key, gpointer user_data)
{
  AdwComboRow *comboRow = ADW_COMBO_ROW (user_data);
  gchar *new_id = g_settings_get_string (settings, "transition");
  for (guint i = 0; i < N_TRANSITIONS; ++i)
    {
      if (g_strcmp0 (transitionTypesC[i].id, new_id) == 0)
        {
          adw_combo_row_set_selected (comboRow, i);
          break;
        }
    }
  g_free (new_id);
}

static void
on_combo_notify_selected (GObject *obj, GParamSpec *pspec, gpointer user_data)
{
  AdwComboRow *comboRow = ADW_COMBO_ROW (obj);
  GSettings *settings = G_SETTINGS (user_data);
  guint sel = adw_combo_row_get_selected (comboRow);
  const char *id = transitionTypesC[sel % N_TRANSITIONS].id;

  gchar *curr = g_settings_get_string (settings, "transition");
  if (g_strcmp0 (curr, id) != 0)
    g_settings_set_string (settings, "transition", id);
  g_free (curr);
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
