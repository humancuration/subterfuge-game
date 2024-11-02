#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <immintrin.h>  // For SIMD
#include "../Core/SIMDHelper.hpp"

class PatternRecognitionSystem {
public:
    struct Pattern {
        // Core components of a pattern
        std::vector<std::string> actions;
        std::vector<std::string> environmental_factors;
        std::vector<std::string> cultural_elements;
        
        // Pattern metadata
        float success_rate{0.0f};
        size_t observation_count{0};
        bool is_verified{false};
        
        // What this pattern led to
        std::string discovery_id;
        float discovery_impact{0.0f};
    };

    struct PatternMatch {
        float similarity_score{0.0f};
        float confidence{0.0f};
        std::vector<std::string> missing_elements;
        bool is_partial_match{false};
    };

private:
    // Store observed patterns
    std::unordered_map<std::string, Pattern> verified_patterns;
    std::vector<Pattern> emerging_patterns;
    
    // Track pattern frequencies
    struct PatternFrequency {
        alignas(32) std::vector<float> action_frequencies;
        alignas(32) std::vector<float> env_frequencies;
        alignas(32) std::vector<float> cultural_frequencies;
    } frequencies;

    // SIMD-aligned similarity calculation data
    alignas(32) std::vector<float> similarity_scores;
    alignas(32) std::vector<float> frequency_weights;

    // Additional similarity metrics
    struct SimilarityMetrics {
        float sequence_similarity{0.0f};  // Order of actions matters
        float timing_similarity{0.0f};    // When actions occur
        float context_similarity{0.0f};   // Environmental conditions
        float outcome_similarity{0.0f};   // Results of pattern
    };

    // Helper methods for similarity calculation
    float calculate_sequence_similarity(
        const std::vector<std::string>& seq1,
        const std::vector<std::string>& seq2) {
        // Longest Common Subsequence algorithm
        std::vector<std::vector<int>> dp(
            seq1.size() + 1,
            std::vector<int>(seq2.size() + 1, 0)
        );

        for (size_t i = 1; i <= seq1.size(); ++i) {
            for (size_t j = 1; j <= seq2.size(); ++j) {
                if (seq1[i-1] == seq2[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }

        float lcs_length = static_cast<float>(dp[seq1.size()][seq2.size()]);
        return lcs_length / std::max(seq1.size(), seq2.size());
    }

    float calculate_context_similarity(
        const std::vector<std::string>& context1,
        const std::vector<std::string>& context2) {
        std::unordered_map<std::string, bool> context_map;
        
        // Mark all elements from first context
        for (const auto& ctx : context1) {
            context_map[ctx] = true;
        }
        
        // Count matches in second context
        size_t matches = 0;
        for (const auto& ctx : context2) {
            if (context_map[ctx]) matches++;
        }
        
        return static_cast<float>(matches) / 
               std::max(context1.size(), context2.size());
    }

    float calculate_timing_similarity(
        const Pattern& p1,
        const Pattern& p2) {
        // Compare action sequences considering their timing
        float timing_diff = std::abs(
            p1.observation_count - p2.observation_count
        ) / std::max(p1.observation_count, p2.observation_count);
        
        return 1.0f - timing_diff;
    }

    float calculate_outcome_similarity(
        const Pattern& p1,
        const Pattern& p2) {
        // Compare pattern results
        if (p1.discovery_id == p2.discovery_id) return 1.0f;
        
        return std::abs(p1.discovery_impact - p2.discovery_impact);
    }

    SimilarityMetrics calculate_detailed_similarity(
        const Pattern& p1,
        const Pattern& p2) {
        return {
            calculate_sequence_similarity(p1.actions, p2.actions),
            calculate_timing_similarity(p1, p2),
            calculate_context_similarity(
                p1.environmental_factors,
                p2.environmental_factors
            ),
            calculate_outcome_similarity(p1, p2)
        };
    }

    void update_existing_pattern(Pattern& pattern, const PatternMatch& match) {
        auto it = verified_patterns.find(match.pattern_id);
        if (it != verified_patterns.end()) {
            // Update observation count
            it->second.observation_count++;
            
            // Update success rate with decay
            const float DECAY_RATE = 0.95f;
            it->second.success_rate = 
                it->second.success_rate * DECAY_RATE + 
                pattern.success_rate * (1.0f - DECAY_RATE);
            
            // Update impact if new impact is higher
            if (pattern.discovery_impact > it->second.discovery_impact) {
                it->second.discovery_impact = pattern.discovery_impact;
            }
            
            // Merge new elements
            merge_pattern_elements(it->second, pattern);
        }
    }

    void merge_pattern_elements(Pattern& existing, const Pattern& new_pattern) {
        // Merge actions
        for (const auto& action : new_pattern.actions) {
            if (std::find(existing.actions.begin(),
                         existing.actions.end(),
                         action) == existing.actions.end()) {
                existing.actions.push_back(action);
            }
        }
        
        // Merge environmental factors
        for (const auto& factor : new_pattern.environmental_factors) {
            if (std::find(existing.environmental_factors.begin(),
                         existing.environmental_factors.end(),
                         factor) == existing.environmental_factors.end()) {
                existing.environmental_factors.push_back(factor);
            }
        }
        
        // Merge cultural elements
        for (const auto& element : new_pattern.cultural_elements) {
            if (std::find(existing.cultural_elements.begin(),
                         existing.cultural_elements.end(),
                         element) == existing.cultural_elements.end()) {
                existing.cultural_elements.push_back(element);
            }
        }
    }

public:
    void observe_success(const std::vector<std::string>& actions,
                        const std::vector<std::string>& env_factors,
                        const std::vector<std::string>& cultural_elements,
                        const std::string& result) {
        // Create or update pattern
        Pattern pattern{
            actions,
            env_factors,
            cultural_elements,
            1.0f,  // Initial success rate
            1,     // First observation
            false, // Not verified yet
            result,
            calculate_impact(result)
        };

        // Check if similar pattern exists
        auto match = find_similar_pattern(pattern);
        if (match.similarity_score > 0.8f) {
            update_existing_pattern(pattern, match);
        } else {
            emerging_patterns.push_back(pattern);
        }

        // Update frequency data
        update_frequencies(pattern);
        
        // Check for pattern verification
        verify_emerging_patterns();
    }

    PatternMatch find_similar_pattern(const Pattern& pattern) {
        const size_t pattern_count = verified_patterns.size();
        
        // Prepare SIMD arrays
        prepare_similarity_calculation(pattern);
        
        // Calculate similarities in batches
        for (size_t i = 0; i < pattern_count; i += 8) {
            calculate_similarity_batch(pattern, i);
        }
        
        // Find best match
        return find_best_match(pattern);
    }

private:
    void prepare_similarity_calculation(const Pattern& pattern) {
        similarity_scores.clear();
        similarity_scores.resize(verified_patterns.size(), 0.0f);
        
        // Prepare frequency weights for SIMD
        frequency_weights.clear();
        frequency_weights.resize(8, 0.0f);
        
        for (size_t i = 0; i < 8; ++i) {
            frequency_weights[i] = calculate_frequency_weight(i);
        }
    }

    void calculate_similarity_batch(const Pattern& pattern, size_t start_idx) {
        // Load pattern data into SIMD registers
        __m256 action_sim = calculate_action_similarity_simd(
            pattern.actions, start_idx);
        __m256 env_sim = calculate_environmental_similarity_simd(
            pattern.environmental_factors, start_idx);
        __m256 cultural_sim = calculate_cultural_similarity_simd(
            pattern.cultural_elements, start_idx);
        
        // Weight similarities based on frequencies
        __m256 weights = _mm256_load_ps(frequency_weights.data());
        
        // Combine similarities
        __m256 combined = _mm256_mul_ps(
            _mm256_add_ps(
                _mm256_add_ps(action_sim, env_sim),
                cultural_sim
            ),
            weights
        );
        
        // Store results
        _mm256_store_ps(&similarity_scores[start_idx], combined);
    }

    PatternMatch find_best_match(const Pattern& pattern) {
        float best_score = 0.0f;
        size_t best_idx = 0;
        
        for (size_t i = 0; i < similarity_scores.size(); ++i) {
            if (similarity_scores[i] > best_score) {
                best_score = similarity_scores[i];
                best_idx = i;
            }
        }
        
        if (best_score > 0.0f) {
            return create_pattern_match(pattern, best_idx, best_score);
        }
        
        return PatternMatch{0.0f, 0.0f, {}, false};
    }

    void verify_emerging_patterns() {
        for (auto it = emerging_patterns.begin(); 
             it != emerging_patterns.end();) {
            if (should_verify_pattern(*it)) {
                // Move to verified patterns
                std::string pattern_id = generate_pattern_id(*it);
                verified_patterns[pattern_id] = std::move(*it);
                verified_patterns[pattern_id].is_verified = true;
                
                it = emerging_patterns.erase(it);
            } else {
                ++it;
            }
        }
    }

    bool should_verify_pattern(const Pattern& pattern) {
        return pattern.observation_count >= 3 && 
               pattern.success_rate > 0.7f &&
               pattern.discovery_impact > 0.5f;
    }

    void update_frequencies(const Pattern& pattern) {
        // Update action frequencies
        for (const auto& action : pattern.actions) {
            update_frequency(frequencies.action_frequencies, action);
        }
        
        // Update environmental frequencies
        for (const auto& factor : pattern.environmental_factors) {
            update_frequency(frequencies.env_frequencies, factor);
        }
        
        // Update cultural frequencies
        for (const auto& element : pattern.cultural_elements) {
            update_frequency(frequencies.cultural_frequencies, element);
        }
    }

    float calculate_impact(const std::string& discovery_id) {
        // Calculate how significant this discovery is
        // based on its effects and uniqueness
        return 0.5f; // Placeholder
    }
}; 