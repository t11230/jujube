#include "MusicSelect.hpp"

#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"

#include <iostream>

#include "../../Data/KeyMapping.hpp"

MusicSelect::Screen::Screen(const Data::SongList& t_song_list) :
    song_list(t_song_list),
    resources(),
    ribbon(resources, m_panel_size, m_panel_spacing),
    song_info(resources, m_upper_part_width, m_upper_part_height),
    selected_panel(),
    button_highlight(m_panel_size, m_panel_spacing),
    key_mapping()
{
    ribbon.title_sort(song_list);
    std::cout << "loaded MusicSelect::Screen" << std::endl;
}

void MusicSelect::Screen::select_chart(sf::RenderWindow& window) {
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    bool chart_selected = false;
    sf::Clock imguiClock;
    ribbon.setPosition(8.f, 602.f);
    button_highlight.setPosition(8.f, 602.f);
    while ((not chart_selected) and window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type) {
            case sf::Event::KeyPressed:
                handle_key_press(event.key);
                break;
            case sf::Event::MouseButtonPressed:
                handle_mouse_click(event.mouseButton);
                break;
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }
        ImGui::SFML::Update(window, imguiClock.restart());
        window.clear(sf::Color::Black);
        ribbon.draw_debug();
        window.draw(ribbon);
        window.draw(button_highlight);
        window.draw(song_info);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}

void MusicSelect::Screen::handle_key_press(const sf::Event::KeyEvent& key_event) {
    auto button = key_mapping.key_to_button(key_event.code);
    if (button) {
        press_button(*button);
    } else {
        switch (key_event.code){
        case sf::Keyboard::F12:
            ribbon.debug = not ribbon.debug;
            break;
        default:
            break;
        }
    }
}

void MusicSelect::Screen::handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event) {
    if (mouse_button_event.button == sf::Mouse::Left) {
        int clicked_panel_index = (mouse_button_event.x / m_panel_size) + 4 * (mouse_button_event.y / m_panel_size);
        auto button = fromIndex(clicked_panel_index);
        if (button) {
            press_button(*button);
        }
    }
}

void MusicSelect::Screen::press_button(const Button& button) {
    button_highlight.button_pressed(button);
    auto button_index = toIndex(button);
    if (button_index < 12) {
        ribbon.click_on(button_index);
        // ribbon.at(button_index)->click(ribbon);
    } else {
        switch (button) {
        case Button::B13:
            ribbon.move_left();
            break;
        case Button::B14:
            ribbon.move_right();
        default:
            break;
        }
    }
}
