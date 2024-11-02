#pragma once
#include <string>
#include <vector>

class SpeciesNetworkTypes {
public:
    enum class NetworkType {
        WOOD_WIDE_WEB,          // Tree-fungal internet
        CORAL_SOCIAL,           // Reef community network
        PRAIRIE_GOSSIP,         // Grassland chemical messaging
        URBAN_JUNGLE_NET,       // City-adapted species network
        MUSHROOM_MARKETPLACE,   // Underground resource trading
        POLLINATOR_TWITTERSPHERE, // Bee/bird communication web
        BACTERIAL_BLOCKCHAIN,   // Microbe consensus network
        SLIME_MOLD_COMPUTING,  // Natural distributed computing
        WHALE_SONG_SOCIAL,     // Ocean acoustic network
        ANT_COLONY_CLOUD       // Distributed ant intelligence
    };

    struct NetworkSignal {
        enum class Type {
            DANGER_ALERT,          // "Watch out fam!"
            RESOURCE_SHARING,      // "Free nutrients here!"
            WEATHER_WARNING,       // "Storm's coming!"
            MATING_CALL,          // "Hey there... ;)"
            TERRITORY_MARKING,     // "My turf, bro"
            FRIENDSHIP_REQUEST,    // "Let's be symbionts?"
            COMMUNITY_INVITE,      // "Join our biome!"
            STRESS_SUPPORT,       // "Hang in there buddy"
            KNOWLEDGE_SHARE,      // "Check out this cool adaptation"
            PARTY_ANNOUNCEMENT    // "Blooming season starts now!"
        };

        Type type;
        float intensity;
        std::string content;
        bool is_viral{false};  // Can spread rapidly through network
    };

    struct NetworkProtocol {
        enum class Type {
            CHEMICAL_MESSAGING,    // Traditional pheromone-based
            BIOLUMINESCENT_CHAT,  // Light-based communication
            SONIC_BROADCAST,      // Sound/vibration based
            ELECTRIC_PULSE,       // Bioelectric signaling
            QUANTUM_ENTANGLED,    // Spooky plant action at a distance
            INTERPRETIVE_DANCE,   // Movement-based signals
            PSYCHIC_LINK,         // Unexplained connections
            MEME_TRANSFER,        // Cultural information sharing
            SPORE_MAIL,          // Message-carrying spores
            CRYSTAL_RESONANCE    // Mineral-based communication
        };

        Type protocol_type;
        float bandwidth;
        float reliability;
        bool needs_translator;  // Different species might need help understanding
    };

    struct NetworkMeme {
        std::string trend;
        float viral_coefficient;
        std::vector<std::string> hashtags;
        bool is_wholesome{true};
    };

    // Fun network status messages
    static std::string get_network_status(NetworkType type) {
        switch (type) {
            case NetworkType::WOOD_WIDE_WEB:
                return "🌳 Forest WiFi: Connected";
            case NetworkType::CORAL_SOCIAL:
                return "🐠 CoralBook: Online";
            case NetworkType::PRAIRIE_GOSSIP:
                return "🌾 GrassChat: Buzzing";
            case NetworkType::MUSHROOM_MARKETPLACE:
                return "🍄 MyceliumMarket: Trading";
            case NetworkType::POLLINATOR_TWITTERSPHERE:
                return "🐝 BeeBook: Buzzing";
            case NetworkType::BACTERIAL_BLOCKCHAIN:
                return "🦠 MicrobeChain: Mining";
            case NetworkType::SLIME_MOLD_COMPUTING:
                return "🟡 SlimeNet: Processing";
            case NetworkType::WHALE_SONG_SOCIAL:
                return "🐋 DeepChat: Broadcasting";
            case NetworkType::ANT_COLONY_CLOUD:
                return "🐜 AntCloud: Syncing";
            default:
                return "🌿 BioCom: Status Unknown";
        }
    }

    // Fun network error messages
    static std::string get_network_error(NetworkType type) {
        switch (type) {
            case NetworkType::WOOD_WIDE_WEB:
                return "Error: Too many branches in recursion";
            case NetworkType::CORAL_SOCIAL:
                return "Error: Reef connection timed out";
            case NetworkType::PRAIRIE_GOSSIP:
                return "Error: Message lost in the wind";
            case NetworkType::MUSHROOM_MARKETPLACE:
                return "Error: Insufficient spores";
            case NetworkType::POLLINATOR_TWITTERSPHERE:
                return "Error: Too many flowers to follow";
            case NetworkType::BACTERIAL_BLOCKCHAIN:
                return "Error: Colony consensus failed";
            case NetworkType::SLIME_MOLD_COMPUTING:
                return "Error: Path optimization overflow";
            case NetworkType::WHALE_SONG_SOCIAL:
                return "Error: Signal lost in deep water";
            case NetworkType::ANT_COLONY_CLOUD:
                return "Error: Pheromone buffer overflow";
            default:
                return "Error: Bio-protocol not found";
        }
    }

    // Network trending topics
    static std::vector<NetworkMeme> get_trending_memes(NetworkType type) {
        switch (type) {
            case NetworkType::WOOD_WIDE_WEB:
                return {
                    {"New photosynthesis technique just dropped", 0.8f, {"#SunlightHack", "#ChlorophyllGang"}},
                    {"This one weird trick to survive drought", 0.9f, {"#WaterSaving", "#RootLife"}}
                };
            case NetworkType::POLLINATOR_TWITTERSPHERE:
                return {
                    {"You won't believe this new flower", 0.95f, {"#NectarCheck", "#PollenParty"}},
                    {"Top 10 garden spots (number 7 will shock you)", 0.7f, {"#BeeLife", "#FlowerPower"}}
                };
            // Add more fun memes for other networks...
            default:
                return {{"404 Meme Not Found", 0.1f, {"#BiologyHumor"}}};
        }
    }
}; 