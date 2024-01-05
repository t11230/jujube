#pragma once

#include <SFML/Graphics.hpp>

#include "../Resources.hpp"

namespace MusicSelect {
    class LeftButton final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        using HoldsResources::HoldsResources;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    class RightButton final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        using HoldsResources::HoldsResources;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    class OptionsButton final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        using HoldsResources::HoldsResources;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    class StartButton final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        using HoldsResources::HoldsResources;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    class BackButton final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        using HoldsResources::HoldsResources;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    class SortButton final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        SortButton(ScreenResources& t_resources, const std::string& t_label) : HoldsResources(t_resources), m_label(t_label) {};

        void set_label(const std::string& t_label) {
            m_label = t_label;
        }

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        std::string m_label;
    };
}