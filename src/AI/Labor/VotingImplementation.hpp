#pragma once
#include "VotingMechanisms.hpp"
#include <cmath>

class VotingImplementation {
public:
    // Liquid Democracy implementation
    static VotingResult process_liquid_democracy(const std::vector<Vote>& votes) {
        std::unordered_map<std::string, float> delegated_power;
        std::unordered_map<std::string, float> final_votes;
        
        // First pass: Calculate delegated voting power
        for (const auto& vote : votes) {
            if (vote.is_delegated) {
                delegated_power[vote.voter_id] += vote.weight;
            }
        }
        
        // Second pass: Apply delegated votes
        for (const auto& vote : votes) {
            float total_power = vote.weight;
            if (delegated_power.contains(vote.voter_id)) {
                total_power += delegated_power[vote.voter_id];
            }
            
            final_votes[vote.proposal_id] += total_power;
        }
        
        // Find winning proposal
        auto max_it = std::max_element(
            final_votes.begin(),
            final_votes.end(),
            [](const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            }
        );
        
        return VotingResult{
            max_it->first,
            max_it->second / std::accumulate(
                final_votes.begin(),
                final_votes.end(),
                0.0f,
                [](float sum, const auto& pair) {
                    return sum + pair.second;
                }
            ),
            false,
            get_minority_concerns(votes, max_it->first)
        };
    }

    // Quadratic Voting implementation
    static VotingResult process_quadratic_voting(const std::vector<Vote>& votes) {
        std::unordered_map<std::string, float> vote_costs;
        std::unordered_map<std::string, float> vote_counts;
        
        // Calculate quadratic costs and apply votes
        for (const auto& vote : votes) {
            float cost = std::pow(vote.weight, 2);
            vote_costs[vote.proposal_id] += cost;
            vote_counts[vote.proposal_id] += std::sqrt(cost);
        }
        
        // Find proposal with most effective votes
        auto max_it = std::max_element(
            vote_counts.begin(),
            vote_counts.end(),
            [](const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            }
        );
        
        return VotingResult{
            max_it->first,
            max_it->second / std::accumulate(
                vote_counts.begin(),
                vote_counts.end(),
                0.0f,
                [](float sum, const auto& pair) {
                    return sum + pair.second;
                }
            ),
            false,
            get_minority_concerns(votes, max_it->first)
        };
    }

    // Conviction Voting implementation
    static VotingResult process_conviction_voting(const std::vector<Vote>& votes) {
        std::unordered_map<std::string, float> conviction_strength;
        
        // Calculate conviction-weighted votes
        for (const auto& vote : votes) {
            float conviction = vote.weight * 
                             (1.0f + std::log(1.0f + vote.conviction_time));
            conviction_strength[vote.proposal_id] += conviction;
        }
        
        // Find proposal with strongest conviction
        auto max_it = std::max_element(
            conviction_strength.begin(),
            conviction_strength.end(),
            [](const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            }
        );
        
        return VotingResult{
            max_it->first,
            max_it->second / std::accumulate(
                conviction_strength.begin(),
                conviction_strength.end(),
                0.0f,
                [](float sum, const auto& pair) {
                    return sum + pair.second;
                }
            ),
            false,
            get_minority_concerns(votes, max_it->first)
        };
    }

private:
    static std::string get_voter_concern(
        const std::string& voter_id,
        const std::string& winning_proposal) {
        
        // In a real implementation, this would query the voter's specific concerns
        return "Concern from voter " + voter_id + 
               " about proposal " + winning_proposal;
    }
}; 