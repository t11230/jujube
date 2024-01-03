#pragma once

#include <array>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "../../Input/Buttons.hpp"
#include "../../Data/Preferences.hpp"
#include "../../Data/Song.hpp"
#include "../../Toolkit/AffineTransform.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "../../Toolkit/EasingFunctions.hpp"
#include "Resources.hpp"
#include "PanelLayout.hpp"
#include "Drawables/ControlPanels.hpp"

namespace MusicSelect {

    class Panel;

    enum class Direction {
        Right,
        Left,
    };

    struct MoveAnimation {
        MoveAnimation(int previous_pos, int next_pos, size_t ribbon_size, Direction direction, float& t_m_time_factor);
        Toolkit::AffineTransform<float> normalized_to_pos;
        Toolkit::AffineTransform<float> seconds_to_normalized;
        float& m_time_factor;
        sf::Clock clock;
        Toolkit::EaseExponential ease_expo;
        float get_position();
        bool ended();
    private:
        Toolkit::AffineTransform<float> create_transform(int previous_pos, int next_pos, size_t ribbon_size, Direction direction);
    };

    // A Ribbon is a visual representation of a PanelLayout,
    // You can scroll it using the left and right buttons
    class Ribbon : public sf::Drawable, public sf::Transformable, public HoldsResources, public Toolkit::Debuggable {
    public:
        Ribbon(PanelLayout layout, ScreenResources& t_resources);
        std::shared_ptr<Panel>& get_panel_under_button(const Input::Button& button);
        void click_on(const Input::Button& button);
        void move_right();
        void move_left();
        void move_to_next_category(const Input::Button& button);
        void set_layout(PanelLayout layout);
        void draw_debug() override;
        virtual ~Ribbon() = default;
    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        void draw_with_animation(sf::RenderTarget& target, sf::RenderStates states) const;
        void draw_without_animation(sf::RenderTarget& target, sf::RenderStates states) const;
        std::size_t get_layout_column(const Input::Button& button) const;
        mutable PanelLayout m_layout;
        std::size_t m_position = 0;
        mutable std::optional<MoveAnimation> m_move_animation;
        float m_time_factor = 1.f;
        mutable LeftButton left_button;
        mutable RightButton right_button;
    };
}