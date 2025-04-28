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

#include "packitup_window.h"
#include "gdkmm/event.h"
#include "packitup.h"
#include <glib/gi18n.h>
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/enums.h>
#include <gtkmm/frame.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/object.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/version.h>
extern "C"
{
#include <adwaita.h>
#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
}
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#define HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY                           \
  GTKMM_CHECK_VERSION (4, 9, 1)

#define HAS_LOAD_FROM_STRING GTK_CHECK_VERSION (4, 12, 0)

static void on_unit_changed_callback (GObject * /*combo_row*/,
                                      GParamSpec * /*pspec*/,
                                      gpointer user_data);

PackitupWindow::PackitupWindow (AdwApplicationWindow *win) : window_ (win)
{
  m_refBuilder = Gtk::Builder::create_from_resource (
      "/tech/bm7/packitup-gnome/src/window.ui");
  // For adwaita widgets we need the gtk C style builder
  auto *c_builder = m_refBuilder->gobj ();

  // SpinRow raw pointers Adwaita fix
  m_rawSpinRowMore
      = ADW_SPIN_ROW (gtk_builder_get_object (c_builder, "spin_row_more"));
  if (!m_rawSpinRowMore)
    throw std::runtime_error ("no \"spin_row_more\" object in window.ui");

  m_rawSpinRowAlright
      = ADW_SPIN_ROW (gtk_builder_get_object (c_builder, "spin_row_alright"));
  if (!m_rawSpinRowAlright)
    throw std::runtime_error ("no \"spin_row_alright\" object in window.ui");

  adw_spin_row_set_numeric (m_rawSpinRowMore, true);
  adw_spin_row_set_numeric (m_rawSpinRowAlright, true);

  m_rawHeader = ADW_HEADER_BAR (gtk_builder_get_object (c_builder, "header"));
  if (!m_rawHeader)
    throw std::runtime_error (
        "no \"header\" object in window.ui"); // Add scrollable window to the
                                              // whole application for WM
                                              // support

  auto layoutBox
      = gtk_builder_get_object (c_builder, "application_box_layout");
  auto applicationBoxLayout = Glib::wrap (GTK_BOX (layoutBox));
  if (!applicationBoxLayout)
    throw std::runtime_error (
        "no \"application_box_layout\" object in window.ui");

  auto app_clampWindow = adw_clamp_new ();
  auto app_scrolledWindow = gtk_scrolled_window_new ();
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (app_scrolledWindow),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  adw_clamp_set_maximum_size (ADW_CLAMP (app_clampWindow), 700);
  applicationBoxLayout->unparent ();
  auto toolview = gtk_builder_get_object (c_builder, "toolview");
  gtk_widget_unparent (GTK_WIDGET (toolview));
  adw_application_window_set_content (window_, GTK_WIDGET (toolview));
  adw_toolbar_view_set_content (ADW_TOOLBAR_VIEW (toolview), app_clampWindow);
  // gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (app_scrolledWindow),
  //                                GTK_WIDGET (layoutBox));
  adw_clamp_set_child (ADW_CLAMP (app_clampWindow),
                       GTK_WIDGET (app_scrolledWindow));
  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (app_scrolledWindow),
                                 GTK_WIDGET (layoutBox));
  // Adwaita ComboRow
  m_rawUnitComboRow
      = ADW_COMBO_ROW (gtk_builder_get_object (c_builder, "unit_comborow"));
  if (!m_rawUnitComboRow)
    throw std::runtime_error ("no \"unit_comborow\" object in window.ui");

  auto *unitList
      = GTK_STRING_LIST (gtk_builder_get_object (c_builder, "unit_list"));

  adw_combo_row_set_model (m_rawUnitComboRow, G_LIST_MODEL (unitList));
  g_signal_connect (m_rawUnitComboRow, "notify::selected",
                    G_CALLBACK (on_unit_changed_callback), this);

  auto resultBtn
      = GTK_BUTTON (gtk_builder_get_object (c_builder, "result_button"));
  m_resultButton = Glib::wrap (resultBtn);
  if (!resultBtn)
    throw std::runtime_error ("no \"result_button\" object in window.ui");

  // Bind Button
  m_resultButton->signal_clicked ().connect (
      sigc::mem_fun (*this, &PackitupWindow::on_result_clicked));

  // App results scrolled window
  auto resultVBox
      = GTK_BOX (gtk_builder_get_object (c_builder, "result_VBox"));
  m_resultVBox = Glib::wrap (resultVBox);
  if (!resultVBox)
    throw std::runtime_error ("no \"result_VBox\" object in window.ui");

  // Adwaita ComboRow
  m_rawBottleSizeComboRow = ADW_COMBO_ROW (
      gtk_builder_get_object (c_builder, "bottle_size_comborow"));
  if (!m_rawBottleSizeComboRow)
    throw std::runtime_error (
        "no \"Adwaita Combo Row bottle_size_comborow\" object in window.ui");

  // Bottle size dropdown menu
  bottleSizeList = GTK_STRING_LIST (
      gtk_builder_get_object (c_builder, "bottle_size_list"));
  m_refBottleSizeList = Glib::wrap (bottleSizeList);
  if (!m_refBottleSizeList)
    throw std::runtime_error ("no \"bottle_size_list\" object in window.ui");

  adw_combo_row_set_selected (m_rawBottleSizeComboRow, 0);
  adw_combo_row_set_model (m_rawBottleSizeComboRow,
                           G_LIST_MODEL (bottleSizeList));
  // Bottle size dropdown menu
  auto *packSizeList
      = GTK_STRING_LIST (gtk_builder_get_object (c_builder, "pack_size_list"));
  m_refPackSizeList = Glib::wrap (packSizeList);
  if (!m_refPackSizeList)
    throw std::runtime_error ("no \"pack_size_list\" object in window.ui");
  // Pack size Adwaita Combo Row
  m_rawPackSizeComboRow = ADW_COMBO_ROW (
      gtk_builder_get_object (c_builder, "pack_size_comborow"));
  if (!m_rawPackSizeComboRow)
    throw std::runtime_error (
        "no \"Adwaita Combo Row pack_size_comborow\" object in window.ui");

  adw_combo_row_set_selected (m_rawPackSizeComboRow, 0);
  adw_combo_row_set_model (m_rawPackSizeComboRow, G_LIST_MODEL (packSizeList));
  // App info box
  auto infoBox = GTK_BOX (gtk_builder_get_object (c_builder, "info_box"));
  auto m_info_VBox = Glib::wrap (infoBox);
  if (!m_info_VBox)
    throw std::runtime_error ("no \"info_box\" object in window.ui");

  // TextView of the 'Info' Gtk::Box
  auto m_info_view = Gtk::make_managed<Gtk::TextView> ();
  m_info_view->set_editable (false);
  m_info_view->set_cursor_visible (false);
  m_info_view->set_wrap_mode (Gtk::WrapMode::WORD);

  m_info_VBox->append (*m_info_view);
  // Set the View Buffer
  auto m_refInfoBuffer = m_info_view->get_buffer ();

  // Bind font settings to infoTextBuffer
  m_refSettings = Gio::Settings::create ("tech.bm7.packitup-gnome");
  auto m_refTagInfoFont = m_refInfoBuffer->create_tag ();
  m_refSettings->bind ("font", m_refTagInfoFont->property_font ());
  m_refInfoBuffer->set_text (_ (
      "This is our application for how many packs of beer you and your "
      "friends need to buy so you won't be out of beer :)\n\n"
      "The calculation takes how many people drinks more than moderated, and "
      "how many drinks alright\n\n"
      "More than moderated would be more than or equal to 2 packs that "
      "day(12 bottles "
      "of 269ml or 8oz, ~3.0L or ~96oz).\n\n"
      "Keep in mind that in order to not let you go out of beer this "
      "calculation will round up the amount of packs needed.\n\n"
      "The values in Liters and Ounces aren't direct convertable. It takes "
      "into account localization."));
  m_refInfoBuffer->apply_tag (m_refTagInfoFont, m_refInfoBuffer->begin (),
                              m_refInfoBuffer->end ());

  m_refGtkSettings = Gtk::Settings::get_default ();
  m_refGtkSettings->property_gtk_enable_animations ()
      .signal_changed ()
      .connect (sigc::mem_fun ((*this),
                               &PackitupWindow::update_revealer_transition));

  update_revealer_transition ();

  // App TextBuffer and TextView of the 'Calculate' button
  auto m_text_view = Gtk::make_managed<Gtk::TextView> ();
  m_text_view->set_editable (false);
  m_text_view->set_cursor_visible (false);
  m_text_view->set_wrap_mode (Gtk::WrapMode::WORD);

  // Add result's frame
  auto m_frame = Gtk::make_managed<Gtk::Frame> ();
  m_frame->set_label (_ ("Result"));
  m_frame->set_label_align (Gtk::Align::CENTER);
  m_frame->set_expand ();
  m_resultVBox->append (*m_frame);

  // Append the Revealer to the layout
  m_frame->set_child (m_revealer);
  m_revealer.set_margin_start (12);
  m_revealer.set_margin_end (12);
  m_revealer.set_transition_duration (500);
  auto m_scrolled_window = Gtk::make_managed<Gtk::ScrolledWindow> ();
  m_scrolled_window->set_policy (Gtk::PolicyType::AUTOMATIC,
                                 Gtk::PolicyType::ALWAYS);
  m_scrolled_window->set_expand ();
  m_revealer.set_child (*m_scrolled_window);
  m_scrolled_window->set_child (*m_text_view);
  m_revealer.set_reveal_child (true);

  // Set the View Buffer
  m_refBuffer = m_text_view->get_buffer ();

  // Bind font settings to TextBuffer results
  m_refTagFont = m_refBuffer->create_tag ();
  m_refSettings->bind ("font", m_refTagFont->property_font ());

  m_refBuffer->set_text (
      _ ("Select the values above and click \"Calculate\" to begin..."));
  m_refBuffer->apply_tag (m_refTagFont, m_refBuffer->begin (),
                          m_refBuffer->end ());
  m_revealer.property_child_revealed ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupWindow::on_result_changed));
  // App Gears menu, with preferences, about and quit button
  // auto gears = gtk_builder_get_object (c_builder, "gears");
  // auto gtkmm_MenuBuilder = Gtk::Builder::create_from_resource (
  //    "/tech/bm7/packitup-gnome/src/menu_button.ui");
  // auto c_gtkmmMenuBuilder = gtkmm_MenuBuilder->gobj ();
  // auto rawGears = gtk_builder_get_object (c_gtkmmMenuBuilder, "gears");
  gears = gtk_menu_button_new ();
  gtk_menu_button_set_direction (GTK_MENU_BUTTON (gears), GTK_ARROW_NONE);
  gtk_menu_button_set_icon_name (GTK_MENU_BUTTON (gears),
                                 "open-menu-symbolic");
  // Connect the menu(gears_menu.ui) to the MenuButton m_gears
  // The connection between action and menu item is specified in gears_menu.ui)
  auto menu_builder = Gtk::Builder::create_from_resource (
      "/tech/bm7/packitup-gnome/src/gears_menu.ui");
  auto c_menu_builder = menu_builder->gobj ();
  // auto c_menu_builder = menu_builder->gobj ();
  auto menu = gtk_builder_get_object (c_menu_builder, "menu");
  if (!menu)
    throw std::runtime_error ("No \"menu\" object in gears_menu.ui");

  gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (gears),
                                  G_MENU_MODEL (menu));

  new_decoration_layout ();

  // Ensure that the HeaderBar has at least icon:close decoration layout
  m_refGtkSettings->property_gtk_decoration_layout ()
      .signal_changed ()
      .connect (sigc::mem_fun (*this, &PackitupWindow::new_decoration_layout));

  // Set the window icon from gresources
  auto icon_theme = gtk_icon_theme_get_for_display (
      gtk_widget_get_display (GTK_WIDGET (window_)));
  if (gtk_icon_theme_has_icon (icon_theme, "packitup"))
    gtk_window_set_icon_name (GTK_WINDOW (window_),
                              "packitup"); // Uses the theme icon
  else
    std::cerr << "Icon 'packitup' not found in theme!" << std::endl;

  AppCustomCssProvider = gtk_css_provider_new ();
  ThemeCssProvider = gtk_css_provider_new ();
  // AppCustomCssProvider->signal_parsing_error ().connect (
  //     [] (const auto &section, const auto &error) {
  //       on_parsing_error (section, error);
  //     });

  m_providerAdded = false;
  reload_theme_css ();
  GdkDisplay *display = gtk_widget_get_display (GTK_WIDGET (window_));

  gtk_style_context_add_provider_for_display (
      display, GTK_STYLE_PROVIDER (ThemeCssProvider),
      GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_style_context_add_provider_for_display (
      display, GTK_STYLE_PROVIDER (AppCustomCssProvider),
      GTK_STYLE_PROVIDER_PRIORITY_USER);
  // GNOME Dark/Light theme switch
  auto gnome = Gio::Settings::create ("org.gnome.desktop.interface");
  gnome->signal_changed ().connect ([this] (const Glib::ustring &key) {
    if (key == "gtk-theme" || key == "color-scheme")
      reload_theme_css ();
  });
  m_refGtkSettings->property_gtk_theme_name ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupWindow::reload_theme_css));
  m_refGtkSettings->property_gtk_application_prefer_dark_theme ()
      .signal_changed ()
      .connect (sigc::mem_fun (*this, &PackitupWindow::reload_theme_css));
}

void
PackitupWindow::present ()
{
  gtk_window_present (GTK_WINDOW (window_));
}

void
PackitupWindow::register_with (AdwApplication *app)
{
  // same as set_application, manages lifecycle of windows
  gtk_application_add_window (GTK_APPLICATION (app), GTK_WINDOW (window_));

  // when the user clicks "close" button
  g_signal_connect (
      window_, "close-request",
      G_CALLBACK (+[] (GtkWindow *win, gpointer user_data) -> gboolean {
        g_application_quit (G_APPLICATION (user_data));
        return GDK_EVENT_STOP;
      }),
      app);
}

void
PackitupWindow::new_decoration_layout ()
{
  std::string default_layout
      = m_refGtkSettings->property_gtk_decoration_layout ().get_value ();

  std::string new_layout = default_layout;
  // Split decoration layout into left/right around ':'
  size_t pos = new_layout.find (":");
  std::string left_side
      = (pos == std::string::npos ? new_layout : new_layout.substr (0, pos));

  std::string right_side
      = (pos == std::string::npos ? "" : new_layout.substr (pos + 1));

  size_t pos_r_close = right_side.find ("close");
  size_t pos_l_close = left_side.find ("close");
  bool close_on_left = false;
  if (pos_r_close != std::string::npos)
    new_layout = "icon:" + right_side;
  else if (pos_l_close != std::string::npos)
    {
      new_layout = left_side + ":icon";
      close_on_left = true;
    }
  else
    new_layout = "icon:";

  adw_header_bar_set_decoration_layout (m_rawHeader, new_layout.c_str ());

  // Check if is a valid widget
  if (!GTK_IS_WIDGET (gears))
    {
      g_warning ("Attempted to move invalid button widget");
      return;
    }

  // Remove from parent if any
  GtkWidget *parent = gtk_widget_get_parent (gears);
  if (parent)
    {
      if (ADW_IS_HEADER_BAR (parent))
        {
          adw_header_bar_remove (m_rawHeader, gears);
        }
      else
        {
          gtk_widget_unparent (gears);
        }
    }
  if (close_on_left)
    adw_header_bar_pack_end (m_rawHeader, gears);
  else
    adw_header_bar_pack_start (m_rawHeader, gears);
}

void
PackitupWindow::reload_theme_css ()
{
  // Look up the current GTK theme name frok GtkSettings
  auto theme = Gtk::Settings::get_default ()->property_gtk_theme_name ();
  auto css_path = find_theme_css_path (theme);
  GdkDisplay *display = gtk_widget_get_display (GTK_WIDGET (window_));
  if (!css_path.empty ())
    {
      if (!m_providerAdded)
        {
          gtk_style_context_add_provider_for_display (
              display, GTK_STYLE_PROVIDER (AppCustomCssProvider),
              GTK_STYLE_PROVIDER_PRIORITY_USER);
          m_providerAdded = true;
        }
      gtk_css_provider_load_from_path (ThemeCssProvider, css_path.c_str ());

#if HAS_LOAD_FROM_STRING
      const char *css = "windowcontrols>image{ min-height:24px; "
                        "min-width:24px; margin: 0px; }";
      gtk_css_provider_load_from_string (AppCustomCssProvider, css);
#else
      const char *css = "windowcontrols>image{ min-height:24px; "
                        "min-width:24px; margin: 0px; }";
      gtk_css_provider_load_from_data (AppCustomCssProvider, css);
#endif
    }
  else
    {
      if (m_providerAdded)
        {
          gtk_style_context_add_provider_for_display (
              display, GTK_STYLE_PROVIDER (ThemeCssProvider),
              GTK_STYLE_PROVIDER_PRIORITY_USER);
          m_providerAdded = false;
        }
#if HAS_LOAD_FROM_STRING
      const char *css = "windowcontrols>image{ min-height:24px; "
                        "min-width:24px; margin: 0px 6px; }";
      gtk_css_provider_load_from_string (AppCustomCssProvider, css);
#else
      const char *css = "windowcontrols>image{ min-height:24px; "
                        "min-width:24px; margin: 0px 6px; }";
      gtk_css_provider_load_from_data (AppCustomCssProvider, css);
#endif
    }
}

void
PackitupWindow::update_revealer_transition ()
{
  bool animations_on = m_refGtkSettings->property_gtk_enable_animations ();
  if (animations_on)
    m_refSettings->bind ("transition", m_revealer.property_transition_type ());
  else
    m_revealer.set_transition_type (Gtk::RevealerTransitionType::NONE);
}

Glib::ustring
PackitupWindow::find_theme_css_path (const Glib::ustring &theme)
{
  // Handle Dark Theme Switch
  auto dark = Gtk::Settings::get_default ()
                  ->property_gtk_application_prefer_dark_theme ();
  // Split "Theme:dark" -> base="Theme" variant="dark"
  auto sep = theme.find (":");
  auto base = (sep == Glib::ustring::npos ? theme : theme.substr (0, sep));
  auto variant = (sep == Glib::ustring::npos ? Glib::ustring{}
                                             : theme.substr (sep + 1));
  auto css_file = ((variant == "dark" || dark) ? "gtk-dark.css" : "gtk.css");

  {
    // User themes in ~/.config/gtk-4.0/gtk.css will always be applied
    auto cfg = Glib::build_filename (Glib::get_user_config_dir (), "gtk-4.0",
                                     css_file);
    if (Gio::File::create_for_path (cfg)->query_exists ())
      return cfg;
  }

  // helper to see if theme exist
  auto exists = [&] (const Glib::ustring &p) {
    return Gio::File::create_for_path (p)->query_exists ();
  };

  // User .themes dir
  {
    auto user_themes_dir = Glib::build_filename (
        Glib::get_home_dir (), ".themes", base, "gtk-4.0", css_file);
    if (exists (user_themes_dir))
      return user_themes_dir;
  }

  // User data dirs
  {
    auto user_data = Glib::build_filename (
        Glib::get_user_data_dir (), "themes", base, "gtk-4.0", css_file);
    if (exists (user_data))
      return user_data;
  }

  // user system data dirs
  for (auto &dir : Glib::get_system_data_dirs ())
    {
      auto path
          = Glib::build_filename (dir, "themes", base, "gtk-4.0", css_file);
      if (exists (path))
        return path;
    }

  return {};
}

void
PackitupWindow::on_parsing_error (
    const Glib::RefPtr<const Gtk::CssSection> &section,
    const Glib::Error &error)
{
  std::cerr << "on_parsing_error(): " << error.what () << std::endl;
  if (section)
    {
      const auto file = section->get_file ();
      if (file)
        {
          std::cerr << "  URI = " << file->get_uri () << std::endl;
        }

      auto start_location = section->get_start_location ();
      auto end_location = section->get_end_location ();
      std::cerr << "  start_line = " << start_location.get_lines () + 1
                << ", end_line = " << end_location.get_lines () + 1
                << std::endl;
      std::cerr << "  start_position = " << start_location.get_line_chars ()
                << ", end_position = " << end_location.get_line_chars ()
                << std::endl;
    }
}

extern "C" void
on_packitup_window_hide (GtkWidget * /* widget */, gpointer user_data)
{
  delete static_cast<PackitupWindow *> (user_data);
}

PackitupWindow *
PackitupWindow::create ()
{
  auto c_builder = gtk_builder_new_from_resource (
      "/tech/bm7/packitup-gnome/src/window.ui");
  auto c_window = gtk_builder_get_object (c_builder, "app_window");
  if (!c_window)
    throw std::runtime_error ("No \"app_window\" object in window.ui");

  AdwApplicationWindow *raw_window = ADW_APPLICATION_WINDOW (c_window);
  g_object_ref (raw_window);

  auto *self = new PackitupWindow (raw_window);
  g_signal_connect (raw_window, "hide", G_CALLBACK (on_packitup_window_hide),
                    self);

  return self;
}

static void
on_unit_changed_callback (GObject * /*combo_row*/, GParamSpec * /*pspec*/,
                          gpointer user_data)
{
  auto *self = static_cast<PackitupWindow *> (user_data);
  self->on_unit_dropdown_changed ();
}

AdwApplicationWindow *
PackitupWindow::raw ()
{
  return window_;
}

void
PackitupWindow::on_unit_dropdown_changed ()
{
  while (m_refBottleSizeList->get_n_items ())
    m_refBottleSizeList->remove (0);
  auto unit_idx = adw_combo_row_get_selected (m_rawUnitComboRow);
  if (unit_idx > 0)
    {
      m_refBottleSizeList->append (_ ("8oz"));
      m_refBottleSizeList->append (_ ("11.2oz"));
      m_refBottleSizeList->append (_ ("11.5oz"));
      m_refBottleSizeList->append (_ ("12oz"));
    }
  else
    {
      m_refBottleSizeList->append (_ ("250ml"));
      m_refBottleSizeList->append (_ ("269ml"));
      m_refBottleSizeList->append (_ ("330ml"));
      m_refBottleSizeList->append (_ ("355ml"));
    }
}

void
PackitupWindow::on_result_clicked ()
{

  if (m_transitionInProgress)
    return;
  m_transitionInProgress = true;

  // Start the hide animation
  m_revealer.set_reveal_child (false);
}

void
PackitupWindow::on_result_changed ()
{
  if (!m_revealer.get_reveal_child ())
    {
      //  Get app values
      auto more_Value = adw_spin_row_get_value (m_rawSpinRowMore);
      auto alright_Value = adw_spin_row_get_value (m_rawSpinRowAlright);
      auto unit_idx = adw_combo_row_get_selected (m_rawUnitComboRow);
      auto single_bottle_idx
          = adw_combo_row_get_selected (m_rawBottleSizeComboRow);
      auto pack_size_idx = adw_combo_row_get_selected (m_rawPackSizeComboRow);
      auto single_bottle
          = std::stof (m_refBottleSizeList->get_string (single_bottle_idx));
      auto pack_size
          = std::stoi (m_refPackSizeList->get_string (pack_size_idx));

      // Set the Unit to use
      if (unit_idx > 0)
        m_unit = "oz";
      else
        {
          m_unit = "L";
          single_bottle /= 1000;
        }

      // Calculate the number of packs for that day they will have to buy
      auto pack = single_bottle * pack_size;
      float pack_size_max_value;
      if (pack > 1.5)
        pack_size_max_value = 1.5;
      else
        pack_size_max_value = pack;
      m_amountOfBeer = ((more_Value * 2 * pack_size_max_value)
                        + more_Value * single_bottle)
                       + (pack_size_max_value * alright_Value
                          + (alright_Value * 2 * single_bottle));
      m_numberOfPacks = std::ceil (m_amountOfBeer / pack);
      m_amountOfBeerPacks = pack * m_numberOfPacks;
      // The total number of people that will consume that amount of beer
      m_totalPeopleNumber = more_Value + alright_Value;

      // Insert text with aproriate tags one at a time
      Glib::ustring initial_str
          = _ ("The total number of packs you might need: ");
      auto refTagBold = m_refBuffer->create_tag ();
      refTagBold->property_weight () = 800;
      m_refBuffer->set_text (initial_str);
      auto offset_field = initial_str.size ();
      auto iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      // We need to convert int/float values to ustring, and to calculate
      // their strlen
      Glib::ustring str_number_of_packs = Glib::ustring::compose (
          _ ("%1"), Glib::ustring::format (std::fixed, std::setprecision (0),
                                           m_numberOfPacks));
      m_refBuffer->insert_with_tag (iterBuffer, str_number_of_packs,
                                    refTagBold);

      offset_field += str_number_of_packs.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      Glib::ustring second_str = _ ("\nYou might need ");
      m_refBuffer->insert (iterBuffer, second_str);

      offset_field += second_str.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      Glib::ustring str_amount_of_beer = Glib::ustring::compose (
          _ ("%1%2"),
          Glib::ustring::format (std::fixed, ::std::setprecision (1),
                                 m_amountOfBeer),
          m_unit);
      m_refBuffer->insert_with_tag (iterBuffer, str_amount_of_beer,
                                    refTagBold);

      Glib::ustring third_str = _ (" of beer for ");
      offset_field += str_amount_of_beer.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      m_refBuffer->insert (iterBuffer, third_str);

      Glib::ustring str_total_ppl_number
          = Glib::ustring::compose (_ ("%1"), m_totalPeopleNumber);
      offset_field += third_str.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);
      m_refBuffer->insert_with_tag (iterBuffer, str_total_ppl_number,
                                    refTagBold);
      Glib::ustring fourth_str;
      if (m_totalPeopleNumber > 1)
        fourth_str = _ (" people.\n");
      else
        fourth_str = _ (" person.\n");

      offset_field += str_total_ppl_number.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);
      m_refBuffer->insert (iterBuffer, fourth_str);

      Glib::ustring str_packs;
      if (m_numberOfPacks > 1)
        str_packs = _ ("packs");
      else
        str_packs = _ ("pack");
      Glib::ustring str_total = Glib::ustring::compose (
          _ ("%1 %2 of beer have %3%4."), m_numberOfPacks, str_packs,
          m_amountOfBeerPacks, m_unit);
      offset_field += fourth_str.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);
      m_refBuffer->insert_with_tag (iterBuffer, str_total, refTagBold);
      m_refBuffer->apply_tag (m_refTagFont, m_refBuffer->begin (),
                              m_refBuffer->end ());

      m_revealer.set_reveal_child (true);
    }
  else
    m_transitionInProgress = false;
}

// vim: sts=2 sw=2 et
