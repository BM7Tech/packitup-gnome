/* **************************************************************************
 */
/*                                                                            */
/*                                                        :::      :::::::: */
/*   packitup_window.cc                                 :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+ */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+ */
/*                                                +#+#+#+#+#+   +#+ */
/*   Created: 2025/04/14 06:55:37 by ebezerra          #+#    #+# */
/*   Updated: 2025/04/14 07:00:26 by ebezerra         ###   ########.fr */
/*                                                                            */
/* **************************************************************************
 */

#include "packitup_window.h"
#include "giomm/settings.h"
#include "gtkmm/enums.h"
#include "gtkmm/object.h"
#include "gtkmm/scrolledwindow.h"
#include <iomanip>
#include <iostream>
#include <stdexcept>

#define HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY                           \
  GTKMM_CHECK_VERSION (4, 9, 1)

PackitupWindow::PackitupWindow (BaseObjectType *cobject,
                                const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow (cobject), m_refBuilder (refBuilder)
{

  //  The app buttons to calculate the result
  m_spin_button_more
      = m_refBuilder->get_widget<Gtk::SpinButton> ("spin_button_more");
  if (!m_spin_button_more)
    throw std::runtime_error ("no \"spin_button_more\" object in window.ui");

  m_spin_button_alright
      = m_refBuilder->get_widget<Gtk::SpinButton> ("spin_button_alright");
  if (!m_spin_button_alright)
    throw std::runtime_error (
        "no \"spin_button_alright\" object in window.ui");
  m_spin_button_alright->set_numeric ();
  m_spin_button_more->set_numeric ();

  m_unit_dropdown = m_refBuilder->get_widget<Gtk::DropDown> ("unit_dropdown");
  if (!m_unit_dropdown)
    throw std::runtime_error ("no \"unit_dropdown\" object in window.ui");

  m_result_button = m_refBuilder->get_widget<Gtk::Button> ("result_button");
  if (!m_result_button)
    throw std::runtime_error ("no \"result_button\" object in window.ui");

  // Bind Button
  m_result_button->signal_clicked ().connect (
      sigc::mem_fun (*this, &PackitupWindow::on_result_clicked));

  // App results scrolled window
  m_result_VBox = m_refBuilder->get_widget<Gtk::Box> ("result_VBox");
  if (!m_result_VBox)
    throw std::runtime_error ("no \"result_VBox\" object in window.ui");

  // App info box
  auto m_info_VBox = m_refBuilder->get_widget<Gtk::Box> ("info_box");
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
  m_refSettings = Gio::Settings::create ("org.gtkmm.packitup");
  auto m_refTagInfoFont = m_refInfoBuffer->create_tag ();
  m_refSettings->bind ("font", m_refTagInfoFont->property_font ());
  m_refInfoBuffer->set_text (
      "This is our application for how many packs of beer you and your "
      "friends need to buy so you won't be short on beer :)\n\n"
      "The calculation takes how many people drinks more than moderated, and "
      "how many drinks alright\n\n"
      "More than moderated would be more than or equal to 2 packs(12 bottles "
      "of 330ml or 11.6oz) that day(~4L or ~140oz)");
  m_refInfoBuffer->apply_tag (m_refTagInfoFont, m_refInfoBuffer->begin (),
                              m_refInfoBuffer->end ());

  // Bind settings menu to the TextView Revealer
  m_refSettings->bind ("transition", m_revealer.property_transition_type ());

  // App TextBuffer and TextView of the 'Calculate' button
  auto m_text_view = Gtk::make_managed<Gtk::TextView> ();
  m_text_view->set_editable (false);
  m_text_view->set_cursor_visible (false);
  m_text_view->set_wrap_mode (Gtk::WrapMode::WORD);

  // Add result's frame
  auto m_frame = Gtk::make_managed<Gtk::Frame> ();
  m_frame->set_label ("Result");
  m_frame->set_label_align (Gtk::Align::CENTER);
  m_frame->set_expand ();
  m_result_VBox->append (*m_frame);

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
  // m_revealer.set_child (*m_text_view);
  m_scrolled_window->set_child (*m_text_view);
  m_revealer.set_reveal_child (true);

  // Set the View Buffer
  m_refBuffer = m_text_view->get_buffer ();

  // Bind font settings to TextBuffer results
  m_refTagFont = m_refBuffer->create_tag ();
  m_refSettings->bind ("font", m_refTagFont->property_font ());

  m_refBuffer->set_text (
      "Select the values above and click \"Calculate\" to begin...");
  m_refBuffer->apply_tag (m_refTagFont, m_refBuffer->begin (),
                          m_refBuffer->end ());
  m_revealer.property_child_revealed ().signal_changed ().connect (
      sigc::mem_fun (*this, &PackitupWindow::on_result_changed));
  //  App header menu, with preferences and quit button
  m_gears = m_refBuilder->get_widget<Gtk::MenuButton> ("gears");
  if (!m_gears)
    throw std::runtime_error ("no \"gears\" object in window.ui");

  m_gears->set_icon_name ("open-menu-symbolic");

  // Connect the menu(gears_menu.ui) to the MenuButton m_gears
  // The connection between action and menu item is specified in gears_menu.ui)
  auto menu_builder = Gtk::Builder::create_from_resource (
      "/org/gtkmm/packitup/src/gears_menu.ui");
  auto menu = menu_builder->get_object<Gio::MenuModel> ("menu");
  if (!menu)
    throw std::runtime_error ("No \"menu\" object in gears_menu.ui");
  m_gears->set_menu_model (menu);

  // Set the window icon from gresources
  auto icon_theme = Gtk::IconTheme::get_for_display (get_display ());
  if (icon_theme->has_icon ("packitup"))
    {
      set_icon_name ("packitup"); // Uses the theme icon
    }
  else
    {
      std::cerr << "Icon 'packitup' not found in theme!" << std::endl;
    }

  m_refCssProvider = Gtk::CssProvider::create ();
  m_refCssProvider->signal_parsing_error ().connect (
      [] (const auto &section, const auto &error) {
        on_parsing_error (section, error);
      });
  m_refCssProvider->load_from_resource ("/org/gtkmm/packitup/src/styles.css");
#if HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY
  Gtk::StyleProvider::add_provider_for_display (
      get_display (), m_refCssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);
#else
  Gtk::StyleContext::add_provider_for_display (
      get_display (), m_refCssProvider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#endif
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

PackitupWindow *
PackitupWindow::create ()
{
  auto refBuilder = Gtk::Builder::create_from_resource (
      "/org/gtkmm/packitup/src/window.ui");

  auto window = Gtk::Builder::get_widget_derived<PackitupWindow> (
      refBuilder, "app_window");

  if (!window)
    throw std::runtime_error ("No \"app_window\" object in window.ui");

  return window;
}

void
PackitupWindow::on_result_clicked ()
{

  if (m_transition_in_progress)
    return;

  m_transition_in_progress = true;

  m_revealer.set_reveal_child (false);
  //  Get app values
  auto more_Value = m_spin_button_more->get_value ();
  auto alright_Value = m_spin_button_alright->get_value ();
  auto unit_idx = m_unit_dropdown->get_selected ();

  // Set the Unit to use
  float single_bottle;
  if (unit_idx > 0)
    {
      single_bottle = 11.6;
      unit = "oz";
    }
  else
    {
      single_bottle = 0.33;
      unit = "L";
    }

  // Calculate the number of packs for that day they will have to buy
  auto pack = single_bottle * 6;
  amount_of_beer
      = ((more_Value * 2 * pack) + more_Value * single_bottle)
        + (pack * alright_Value + (alright_Value * 2 * single_bottle));
  number_of_packs = std::ceil (amount_of_beer / pack);
  // The total number of people that will consume that amount of beer
  total_people_number = more_Value + alright_Value;
}

void
PackitupWindow::on_result_changed ()
{
  if (!m_revealer.get_reveal_child ())
    {

      // Insert text with aproriate tags one at a time
      Glib::ustring initial_str = "The total number of packs you might need: ";
      auto refTagBold = m_refBuffer->create_tag ();
      refTagBold->property_weight () = 800;
      m_refBuffer->set_text (initial_str);
      auto offset_field = initial_str.size ();
      auto iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      // We need to convert int/float values to ustring, and to calculate
      // their strlen
      Glib::ustring str_number_of_packs = Glib::ustring::compose (
          "%1", Glib::ustring::format (std::fixed, std::setprecision (0),
                                       number_of_packs));
      m_refBuffer->insert_with_tag (iterBuffer, str_number_of_packs,
                                    refTagBold);

      offset_field += str_number_of_packs.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      Glib::ustring second_str = "\nThats a total of ";
      m_refBuffer->insert (iterBuffer, second_str);

      offset_field += second_str.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      Glib::ustring str_amount_of_beer = Glib::ustring::compose (
          "%1%2",
          Glib::ustring::format (std::fixed, ::std::setprecision (1),
                                 amount_of_beer),
          unit);
      m_refBuffer->insert_with_tag (iterBuffer, str_amount_of_beer,
                                    refTagBold);

      Glib::ustring third_str = " for ";
      offset_field += str_amount_of_beer.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);

      m_refBuffer->insert (iterBuffer, third_str);

      Glib::ustring str_total_ppl_number
          = Glib::ustring::compose ("%1", total_people_number);
      offset_field += third_str.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);
      m_refBuffer->insert_with_tag (iterBuffer, str_total_ppl_number,
                                    refTagBold);
      Glib::ustring fourth_str;
      if (total_people_number > 1)
        fourth_str = " people.\n";
      else
        fourth_str = " person.\n";

      offset_field += str_total_ppl_number.size ();
      iterBuffer = m_refBuffer->get_iter_at_offset (offset_field);
      m_refBuffer->insert (iterBuffer, fourth_str);
      m_refBuffer->apply_tag (m_refTagFont, m_refBuffer->begin (),
                              m_refBuffer->end ());

      m_revealer.set_reveal_child (true);
    }
  else if (m_revealer.get_reveal_child ())
    {

      m_transition_in_progress = false;
    }
}

// vim: sts=2 sw=2 et
