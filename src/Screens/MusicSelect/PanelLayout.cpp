#include "PanelLayout.hpp"

#include "Panels/Panel.hpp"

namespace MusicSelect {
    PanelLayout::PanelLayout(
        const CategoryContainer& categories,
        ScreenResources& t_resources
    ) {
        for (auto &&[category, panels] : categories) {
            if (not panels.empty()) {
                std::vector<std::shared_ptr<Panel>> current_column;

                current_column.emplace_back(std::make_shared<CategoryPanel>(t_resources, category));

                for (auto& panel : panels) {
                    if (current_column.size() == 3) {
                        push_back({current_column[0],current_column[1],current_column[2]});
                        current_column.clear();
                    }
                    current_column.push_back(panel);
                }
                if (not current_column.empty()) {
                    while (current_column.size() < 3) {
                        current_column.emplace_back(std::make_shared<EmptyPanel>(t_resources));
                    }
                    push_back({current_column[0],current_column[1],current_column[2]});
                }
            }
        }
        fill_layout(t_resources);
    }

    PanelLayout::PanelLayout(
        const std::vector<std::shared_ptr<Panel>>& panels,
        ScreenResources& t_resources
    ) {
        std::vector<std::shared_ptr<Panel>> current_column;
        for (auto& panel : panels) {
            if (current_column.size() == 3) {
                push_back({current_column[0],current_column[1],current_column[2]});
                current_column.clear();
            }
            current_column.push_back(panel);
        }
        if (not current_column.empty()) {
            while (current_column.size() < 3) {
                current_column.emplace_back(std::make_shared<EmptyPanel>(t_resources));
            }
            push_back({current_column[0],current_column[1],current_column[2]});
        }
        fill_layout(t_resources);
    }

    PanelLayout PanelLayout::red_empty_layout(ScreenResources& t_resources) {
        std::vector<std::shared_ptr<Panel>> panels;
        for (size_t i = 0; i < 3*4; i++) {
            panels.emplace_back(std::make_shared<ColoredMessagePanel>(t_resources, sf::Color::Red, "- EMPTY -"));
        }
        return PanelLayout{panels, t_resources};
    }

    PanelLayout PanelLayout::title_sort(const Data::SongList& song_list, ScreenResources& t_resources) {
        using SongPtr = std::shared_ptr<const Data::Song>;

        std::vector<SongPtr> songs;
        songs.reserve(song_list.songs.size());

        auto pred = [](SongPtr a, SongPtr b){return Data::Song::sort_by_title(*a, *b);};

        for (auto &&song : song_list.songs) {
            songs.insert(std::lower_bound( songs.begin(), songs.end(), song, pred), song);
        }

        auto category_pred = [](const CategoryContainer::value_type& a, const std::string& b) { return a.first < b; };

        CategoryContainer categories;        
        std::string category;

        for (const auto &song : songs) {
            if (song->title.size() > 0) {
                char letter = song->title[0];
                if ('A' <= letter and letter <= 'Z') {
                    category = std::string(1, letter);
                } else if ('a' <= letter and letter <= 'z') {
                    category = std::string(1, 'A' + (letter - 'a'));
                } else {
                    category = "?";
                }
            } else {
                category = "?";
            }

            auto it = std::lower_bound(categories.begin(), categories.end(), category, category_pred);

            if (it != categories.end() and it->first == category) {
                it->second.emplace_back(std::make_shared<SongPanel>(t_resources, song));
            } else {
                categories.insert(it, {category, {std::make_shared<SongPanel>(t_resources, song)}});
            }
        }
        return PanelLayout{categories, t_resources};
    }

    PanelLayout PanelLayout::level_sort(const Data::SongList& song_list, ScreenResources& t_resources) {
        using SongPtr = std::shared_ptr<const Data::Song>;
        using DiffSong = std::tuple<int, std::string, SongPtr>;
    
        std::vector<DiffSong> songs;

        // Reserve each song 3 times, once for each difficulty
        songs.reserve(song_list.songs.size()*3);
        
        auto pred = [&](const DiffSong& a, const DiffSong& b) { return std::get<0>(a) < std::get<0>(b); };

        // Expand each song into all of its difficulties and sort them by level
        for (const auto& song : song_list.songs) {
            for (const auto& [diff, level] : song->chart_levels) {
                DiffSong item(level, diff, song);
                songs.insert(std::lower_bound(songs.begin(), songs.end(), item, pred), std::move(item));
            }
        }

        if (songs.empty()) {
            return red_empty_layout(t_resources);
        }

        auto category_pred = [](const CategoryContainer::value_type& a, int b) { return std::atoi(a.first.data()) < b; };

        CategoryContainer categories;

        for (const auto& [level, diff, song] : songs) {
            auto it = std::lower_bound(categories.begin(), categories.end(), level, category_pred);
            if (it != categories.end() and it->first == std::to_string(level)) {
                it->second.emplace_back(std::make_shared<SongPanel>(t_resources, song, diff));
            } else {
                categories.insert(it, {std::to_string(level), {std::make_shared<SongPanel>(t_resources, song, diff)}});
            }
        }

        return PanelLayout{categories, t_resources};
    }
    
    void PanelLayout::fill_layout(ScreenResources& t_resources) {
        while (size() < 4) {
            push_back({
                std::make_shared<EmptyPanel>(t_resources),
                std::make_shared<EmptyPanel>(t_resources),
                std::make_shared<EmptyPanel>(t_resources)
            });
        }
    }
}
