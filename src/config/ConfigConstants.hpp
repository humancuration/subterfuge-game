#pragma once

namespace Config {
    class Constants {
    public:
        // Visualization Constants
        static constexpr const char* BACKGROUND_COLOR = "#FFFFFF";
        static constexpr const char* NODE_COLOR = "#FF5733";
        static constexpr const char* EDGE_COLOR = "#C70039";
        static constexpr int NODE_SIZE = 10;
        static constexpr int EDGE_WIDTH = 1;

        // File Paths
        static constexpr const char* DEFAULT_DATA_FILE = "data_sample_dataset.csv";
        static constexpr const char* LOG_FILE = "app.log";

        // UI Constants
        static constexpr int WINDOW_WIDTH = 800;
        static constexpr int WINDOW_HEIGHT = 600;
    };
} 