#pragma once
#include "VotingMechanisms.hpp"

class ExtendedVotingMechanisms {
public:
    enum class AdvancedVotingSystem {
        APPROVAL_VOTING,         // Vote for all acceptable options
        SCORE_VOTING,           // Rate each option (0-5)
        CONDORCET_METHOD,       // Pairwise comparisons
        PARTICIPATORY,          // Direct involvement in proposal creation
        DEMARCHY,               // Random selection with deliberation
        SOCIOCRACY,            // Consent-based circular organization
        HOLACRACY,             // Dynamic roles and circles
        FUTARCHY               // Prediction markets for decisions
    };

    struct ExtendedVote {
        std::string voter_id;
        std::unordered_map<std::string, float> scores;  // For score voting
        std::vector<std::string> approved_options;      // For approval voting
        std::vector<std::pair<std::string, std::string>> preferences;  // For Condorcet
        float expertise_level{0.0f};
        bool has_participated_in_deliberation{false};
    };

    static VotingResult process_approval_voting(
        const std::vector<ExtendedVote>& votes) {
        
        std::unordered_map<std::string, float> approval_counts;
        float total_voters = static_cast<float>(votes.size());

        // Count approvals for each option
        for (const auto& vote : votes) {
            for (const auto& option : vote.approved_options) {
                approval_counts[option] += 1.0f;
            }
        }

        // Find most approved option
        auto max_it = std::max_element(
            approval_counts.begin(),
            approval_counts.end(),
            [](const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            }
        );

        return VotingResult{
            max_it->first,
            max_it->second / total_voters,
            max_it->second > (total_voters * 0.5f),
            {}
        };
    }

    static VotingResult process_score_voting(
        const std::vector<ExtendedVote>& votes) {
        
        std::unordered_map<std::string, float> total_scores;
        std::unordered_map<std::string, size_t> vote_counts;

        // Sum scores for each option
        for (const auto& vote : votes) {
            for (const auto& [option, score] : vote.scores) {
                total_scores[option] += score;
                vote_counts[option]++;
            }
        }

        // Calculate average scores
        std::unordered_map<std::string, float> average_scores;
        for (const auto& [option, total] : total_scores) {
            average_scores[option] = total / vote_counts[option];
        }

        // Find highest average score
        auto max_it = std::max_element(
            average_scores.begin(),
            average_scores.end(),
            [](const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            }
        );

        return VotingResult{
            max_it->first,
            max_it->second / 5.0f,  // Normalize to 0-1
            max_it->second > 3.0f,  // Threshold for strong consensus
            {}
        };
    }

    static VotingResult process_condorcet(
        const std::vector<ExtendedVote>& votes) {
        
        std::set<std::string> all_options;
        // Gather all options
        for (const auto& vote : votes) {
            for (const auto& [opt1, opt2] : vote.preferences) {
                all_options.insert(opt1);
                all_options.insert(opt2);
            }
        }

        // Create pairwise preference matrix
        std::unordered_map<std::string, 
            std::unordered_map<std::string, int>> preferences;

        // Count preferences
        for (const auto& vote : votes) {
            for (const auto& [preferred, less_preferred] : vote.preferences) {
                preferences[preferred][less_preferred]++;
            }
        }

        // Find Condorcet winner (beats all others in pairwise comparisons)
        for (const auto& candidate : all_options) {
            bool is_condorcet_winner = true;
            for (const auto& other : all_options) {
                if (candidate == other) continue;

                int wins_against_other = preferences[candidate][other];
                int loses_to_other = preferences[other][candidate];

                if (wins_against_other <= loses_to_other) {
                    is_condorcet_winner = false;
                    break;
                }
            }

            if (is_condorcet_winner) {
                return VotingResult{
                    candidate,
                    1.0f,  // Condorcet winner has full legitimacy
                    true,
                    {}
                };
            }
        }

        // No Condorcet winner found, fall back to ranked choice
        return process_ranked_choice_fallback(votes);
    }

    static VotingResult process_participatory(
        const std::vector<ExtendedVote>& votes) {
        
        // Weight votes by participation in deliberation
        float total_weight = 0.0f;
        std::unordered_map<std::string, float> weighted_votes;

        for (const auto& vote : votes) {
            float weight = 1.0f;
            if (vote.has_participated_in_deliberation) {
                weight *= 1.5f;  // Participation bonus
            }
            weight *= (1.0f + vote.expertise_level);  // Expertise bonus

            for (const auto& [option, score] : vote.scores) {
                weighted_votes[option] += score * weight;
            }
            total_weight += weight;
        }

        // Find option with highest weighted score
        auto max_it = std::max_element(
            weighted_votes.begin(),
            weighted_votes.end(),
            [](const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            }
        );

        return VotingResult{
            max_it->first,
            max_it->second / total_weight,
            max_it->second > (total_weight * 0.6f),  // Higher threshold
            {}
        };
    }
}; 