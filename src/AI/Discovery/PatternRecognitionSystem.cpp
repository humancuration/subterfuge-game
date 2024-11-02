#include "PatternRecognitionSystem.hpp"
#include <algorithm>
#include <cmath>

// Helper function to calculate similarity between vectors of strings
float calculate_vector_similarity(const std::vector<std::string>& vec1, 
                                const std::vector<std::string>& vec2) {
    if (vec1.empty() || vec2.empty()) return 0.0f;
    
    size_t matches = 0;
    for (const auto& item1 : vec1) {
        for (const auto& item2 : vec2) {
            if (item1 == item2) matches++;
        }
    }
    
    return static_cast<float>(matches) / 
           std::max(vec1.size(), vec2.size());
}

__m256 PatternRecognitionSystem::calculate_action_similarity_simd(
    const std::vector<std::string>& actions, size_t start_idx) {
    alignas(32) float similarities[8] = {0};
    
    for (size_t i = 0; i < 8 && (start_idx + i) < verified_patterns.size(); ++i) {
        auto it = std::next(verified_patterns.begin(), start_idx + i);
        similarities[i] = calculate_vector_similarity(actions, it->second.actions);
    }
    
    return _mm256_load_ps(similarities);
}

__m256 PatternRecognitionSystem::calculate_environmental_similarity_simd(
    const std::vector<std::string>& factors, size_t start_idx) {
    alignas(32) float similarities[8] = {0};
    
    for (size_t i = 0; i < 8 && (start_idx + i) < verified_patterns.size(); ++i) {
        auto it = std::next(verified_patterns.begin(), start_idx + i);
        similarities[i] = calculate_vector_similarity(
            factors, 
            it->second.environmental_factors
        );
    }
    
    return _mm256_load_ps(similarities);
}

__m256 PatternRecognitionSystem::calculate_cultural_similarity_simd(
    const std::vector<std::string>& elements, size_t start_idx) {
    alignas(32) float similarities[8] = {0};
    
    for (size_t i = 0; i < 8 && (start_idx + i) < verified_patterns.size(); ++i) {
        auto it = std::next(verified_patterns.begin(), start_idx + i);
        similarities[i] = calculate_vector_similarity(
            elements, 
            it->second.cultural_elements
        );
    }
    
    return _mm256_load_ps(similarities);
}

void PatternRecognitionSystem::update_frequency(
    std::vector<float>& frequencies, 
    const std::string& element) {
    // Simple frequency counting with decay
    static const float DECAY_RATE = 0.99f;
    
    // Find or create frequency entry
    auto it = std::find_if(frequencies.begin(), frequencies.end(),
        [&element](float f) { return f > 0.0f; });
    
    if (it != frequencies.end()) {
        *it = (*it * DECAY_RATE) + 1.0f;
    } else {
        frequencies.push_back(1.0f);
    }
}

float PatternRecognitionSystem::calculate_frequency_weight(size_t index) {
    // Weight more frequent patterns higher
    float total_observations = 0.0f;
    for (const auto& pattern : verified_patterns) {
        total_observations += pattern.second.observation_count;
    }
    
    if (total_observations == 0.0f) return 1.0f;
    
    auto it = std::next(verified_patterns.begin(), index);
    return it->second.observation_count / total_observations;
}

PatternMatch PatternRecognitionSystem::create_pattern_match(
    const Pattern& pattern,
    size_t match_idx,
    float similarity_score) {
    
    PatternMatch match;
    match.similarity_score = similarity_score;
    
    // Calculate confidence based on observation count
    auto it = std::next(verified_patterns.begin(), match_idx);
    match.confidence = std::min(1.0f, 
        static_cast<float>(it->second.observation_count) / 10.0f);
    
    // Find missing elements
    for (const auto& action : it->second.actions) {
        if (std::find(pattern.actions.begin(), 
                     pattern.actions.end(), 
                     action) == pattern.actions.end()) {
            match.missing_elements.push_back(action);
        }
    }
    
    match.is_partial_match = !match.missing_elements.empty();
    
    return match;
}

std::string PatternRecognitionSystem::generate_pattern_id(const Pattern& pattern) {
    // Create a unique ID based on pattern components
    std::string id = "";
    for (const auto& action : pattern.actions) {
        id += action.substr(0, 3);
    }
    id += "_" + std::to_string(pattern.observation_count);
    return id;
} 