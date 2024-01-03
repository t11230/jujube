#pragma once

#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <unordered_map>

#include <ghc/filesystem.hpp>
#include <SFML/Audio.hpp>

#include "Chart.hpp"
#include "TimeBounds.hpp"

namespace fs = ghc::filesystem;

namespace Data {

    /*
    * Difficulty name ordering : BSC > ADV > EXT > anything else in lexicographical order
    */
    struct cmp_dif_name {
        bool operator()(const std::string& a, const std::string& b) const;
    };

    // Basic metadata about a song
    struct Song {
        fs::path folder;
        std::string title;
        std::string artist;
        // Path to the album cover
        std::optional<fs::path> cover;
        // Path the the audio file
        std::optional<fs::path> audio;
        std::optional<sf::Music::TimeSpan> preview;
        // Mapping from chart difficulty (BSC, ADV, EXT ...) to the numeric level,
        std::map<std::string, unsigned int, cmp_dif_name> chart_levels;

        virtual std::optional<fs::path> full_cover_path() const;
        virtual std::optional<fs::path> full_audio_path() const;

        virtual std::optional<Chart> get_chart(const std::string& difficulty) const = 0;

        static bool sort_by_title(const Data::Song& a, const Data::Song& b) {
            return a.title < b.title;
        }

        static bool order_by_level(const Data::Song& a, const Data::Song& b, const std::string& difficulty) {
            auto a_level = a.chart_levels.find(difficulty);
            auto b_level = b.chart_levels.find(difficulty);

            if (a_level == a.chart_levels.end() && b_level == b.chart_levels.end()) {
                return a.title < b.title;
            } else if (a_level == a.chart_levels.end()) {
                return false;
            } else if (b_level == b.chart_levels.end()) {
                return true;
            } else {
                return a_level->second < b_level->second;
            }
        }

        virtual ~Song() = default;
    };

    struct SongDifficulty {
        const Data::Song& song;
        const std::string difficulty;

        std::optional<Chart> get_chart() const {return song.get_chart(difficulty);};

        // Get the total play interval for this chart :
        // if there's no audio and no notes, returns a one second inteval starting at sf::Time::Zero
        // otherwise the start bound is either zero or the first note if for some reason it happens before 0s
        // and the end bound is either the end of the song of the last timing event if a note or the
        // release of a long note happens after the audio ends
        TimeBounds get_time_bounds() const;
        
        bool operator==(const SongDifficulty &other) const {
            return (
                song.folder == other.song.folder and
                difficulty == other.difficulty
            );
        }
    };

    struct MemonSong : public Song {
        explicit MemonSong(const fs::path& memon_path);
        std::optional<Chart> get_chart(const std::string& difficulty) const;
    private:
        fs::path memon_path;
    };

    /*
    struct MemoSong : public Song {
        explicit MemoSong(const std::unordered_map<std::string, fs::path>& memo_paths);
    private:
        std::unordered_map<std::string, fs::path> memo_paths;
    };
    */

    // Class holding all the necessary song data to run the Music Select screen
    class SongList {
    public:
        SongList(const fs::path& jujube_path);
        std::list<std::shared_ptr<Song>> songs;
    };

    // Returns the folders conscidered to contain a valid song
    // classic memo files should have the .memo extension
    std::list<std::shared_ptr<Song>> recursiveSongSearch(fs::path song_or_pack);
}
