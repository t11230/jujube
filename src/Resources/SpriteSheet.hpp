#pragma once

#include <optional>

#include <ghc/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../Toolkit/DurationInFrames.hpp"

namespace fs = ghc::filesystem;

namespace Resources {
    struct SpriteSheet {
        sf::Texture tex;
        fs::path tex_path;
        std::size_t count;
        std::size_t columns;
        std::size_t rows;

        void load_and_check(
            const fs::path& folder,
            std::size_t size,
            std::size_t fps,
            const Toolkit::DurationInFrames& max_duration
        );

        std::optional<sf::Sprite> get_sprite(std::size_t frame, std::size_t size) const;
    };

    void from_json(const nlohmann::json& j, SpriteSheet& s);
}
