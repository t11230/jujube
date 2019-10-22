#pragma once

#include <SFML/Window.hpp>
#include "../Model/Chart.hpp"
#include "../Model/Score.hpp"

namespace Screen {
    class Gameplay {
        const Chart& chart;
    public:
        explicit Gameplay(const Chart& selected_chart);
        Score display(sf::Window& window) const;
    };
};

