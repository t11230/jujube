#include "Song.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>

#include <memon/memon.hpp>

namespace fs = ghc::filesystem;

namespace {
    std::optional<int> difficulty_to_ordinal(const std::string& difficulty) {
        if (difficulty == "BSC") {
            return 0;
        } else if (difficulty == "ADV") {
            return 1;
        } else if (difficulty == "EXT") {
            return 2;
        } else {
            return {};
        }
    }
}

namespace Data {

    bool cmp_dif_name::operator()(const std::string &a, const std::string &b) const {
        // Difficulty name ordering : BSC > ADV > EXT > anything else in lexicographical order
        auto a_ord = difficulty_to_ordinal(a);
        auto b_ord = difficulty_to_ordinal(b);

        if (a_ord and b_ord) {
            return a_ord.value() < b_ord.value();
        } else if (a_ord) {
            return true;
        } else if (b_ord) {
            return false;
        } else {
            return a < b;
        }
    }

    std::optional<fs::path> Song::full_cover_path() const {
        if (cover) {
            return folder/cover.value();
        } else {
            return {};
        } 
    }

    std::optional<fs::path> Song::full_audio_path() const {
        if (audio) {
            return folder/audio.value();
        } else {
            return {};
        } 
    }

    SongList::SongList(const fs::path& jujube_path) :
        songs()
    {
        fs::path song_folder = jujube_path/"songs";

        if (fs::exists(song_folder) and fs::is_directory(song_folder)) {
            for (const auto& dir_item : fs::directory_iterator(song_folder)) {
                if (dir_item.is_directory()) {
                    songs.splice(songs.end(), recursiveSongSearch(dir_item.path()));
                }
            }
        }
        std::cout << "Loaded Data::SongList, found " << songs.size() << " songs" << '\n';
    }

    std::list<std::shared_ptr<Song>> recursiveSongSearch(fs::path song_or_pack) {
        std::list<std::shared_ptr<Song>> res;

        // First try : any .memo file in the folder ?
        fs::directory_iterator folder_memo{song_or_pack};
        if (
            std::any_of(
                fs::begin(folder_memo),
                fs::end(folder_memo),
                [](const fs::directory_entry& de) {return de.path().extension() == ".memo";}
            )
        ) {
            throw std::invalid_argument("jujube does not support .memo files for now ...");
        }

        // Second try : any .memon file in the folder ?
        // if yes get the first one
        fs::directory_iterator folder_memon{song_or_pack};
        auto memon_path = std::find_if(
            fs::begin(folder_memon),
            fs::end(folder_memon),
            [](const fs::directory_entry& de) {return de.path().extension() == ".memon";}
        );
        if (memon_path != fs::end(folder_memon)) {
            auto song = std::make_shared<MemonSong>(memon_path->path());
            if (not song->chart_levels.empty()) {
                res.push_back(song);
            }
            return res;
        }
        // Nothing found : recurse in subfolders
        for (auto& p : fs::directory_iterator(song_or_pack)) {
            if (p.is_directory()) {
                res.splice(res.end(), recursiveSongSearch(p));
            }
        }
        return res;
    }

    TimeBounds SongDifficulty::get_time_bounds() const {
        auto chart = song.get_chart(difficulty);
        if (not chart) {
            throw std::invalid_argument("Song "+song.title+" has no '"+difficulty+"' chart");
        }
        TimeBounds time_bounds = {sf::Time::Zero, sf::seconds(1)};
        time_bounds += chart->get_time_bounds_from_notes();
        sf::Music m;
        if (m.openFromFile(*song.full_audio_path())) {
            time_bounds += {sf::Time::Zero, m.getDuration()};
        }
        time_bounds.end += sf::seconds(1);
        return time_bounds;
    }

    MemonSong::MemonSong(const fs::path& t_memon_path) :
        memon_path(t_memon_path)
    {
        auto song_folder = t_memon_path.parent_path();
        folder = song_folder;
        stepland::memon m;
        {
            std::ifstream file(t_memon_path);
            file >> m;
        }
        this->title = m.song_title;
        this->artist = m.artist;
        if (not m.album_cover_path.empty()) {
            this->cover.emplace(m.album_cover_path);
        }
        if (not m.music_path.empty()) {
            this->audio.emplace(m.music_path);
        }
        if (m.preview) {
            this->preview.emplace(
                sf::seconds(m.preview->position),
                sf::seconds(m.preview->duration)
            );
        }
        for (const auto& [difficulty, chart] : m.charts) {
            this->chart_levels[difficulty] = chart.level;
        }
    }

    std::optional<Chart> MemonSong::get_chart(const std::string& difficulty) const {
        stepland::memon m;
        {
            std::ifstream file(memon_path);
            file >> m;
        }
        auto chart = m.charts.find(difficulty);
        if (chart == m.charts.end()) {
            return {};
        } else {
            return Chart(m, difficulty);
        }
    }
}
