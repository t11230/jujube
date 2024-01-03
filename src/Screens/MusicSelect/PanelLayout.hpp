#pragma once

#include <array>
#include <functional>
#include <memory>
#include <vector>

#include "../../Data/Song.hpp"
#include "Resources.hpp"

namespace MusicSelect {

    class Panel;

    // PanelLayout restricts the ways you can create a scrollable grid of panels usable in a Ribbon
    class PanelLayout : public std::vector<std::array<std::shared_ptr<Panel>,3>> {
    public:
        using PanelPtr = std::shared_ptr<Panel>;
        using CategoryContainer = std::vector<std::pair<std::string,std::vector<PanelPtr>>>;

        // Takes of sorted vector of category name and associated Panels, useful for all the sorted layouts
        explicit PanelLayout(const CategoryContainer& categories, ScreenResources& t_resources);

        // Arranges all the panels in the vector in columns of three
        explicit PanelLayout(const std::vector<std::shared_ptr<Panel>>& panels, ScreenResources& t_resources);

        // Stepmania-like empty layout with big red panels that say EMPTY
        static PanelLayout red_empty_layout(ScreenResources& t_resources);

        // Standard title sort with categories for each letter
        static PanelLayout title_sort(const Data::SongList& song_list, ScreenResources& t_resources);

        // Difficulty sort with categories for each level
        static PanelLayout level_sort(const Data::SongList& song_list, ScreenResources& t_resources);

    private:
        void fill_layout(ScreenResources& t_resources);
    };
}