#pragma once

#include <map>
#include <stack>

#include <jbcoe/polymorphic_value.h>
#include <SFML/Window.hpp>

#include "../../Data/Song.hpp"
#include "../../Data/Chart.hpp"
#include "../../Data/KeyMapping.hpp"
#include "../../Drawables/BlackFrame.hpp"
#include "../../Resources/Marker.hpp"
#include "../../Toolkit/AffineTransform.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "Ribbon.hpp"
#include "SongInfo.hpp"
#include "SharedResources.hpp"
#include "Options/OptionPage.hpp"
#include "Drawables/ControlPanels.hpp"

namespace MusicSelect {

    class SongPanel;
    // The music select screen is created only once
    // it loads a cache of available songs in the song_list attribute
    class Screen : public Toolkit::Debuggable, public HoldsSharedResources {
    public:
        Screen(
            const Data::SongList& t_song_list,
            SharedResources& t_resources
        );
        Data::SongDifficulty select_chart(sf::RenderWindow& window);
        void draw_debug() override;
    private:
        const Data::SongList song_list;

        Ribbon ribbon;
        SongInfo song_info;
        MainOptionPage main_option_page;
        OptionsButton options_button;
        StartButton start_button;
        bool chart_selected = false;

        Drawables::BlackFrame black_frame;
        sf::RectangleShape panel_filter;
    
        // converts a key press into a button press
        void handle_key_press(const sf::Event::KeyEvent& key_event);
        // converts a mouse click into a button press
        void handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event);
        // chooses what happens for each button
        void press_button(const Data::Button& button);
    };
}
