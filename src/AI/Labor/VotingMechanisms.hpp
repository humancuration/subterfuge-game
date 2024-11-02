#pragma once
#include "LaborOrganizationSystem.hpp"
#include <algorithm>
#include <numeric>

class VotingMechanisms {
public:
    enum class VotingSystem {
        MAJORITY,               // Simple majority
        CONSENSUS,             // Full group agreement
        RANKED_CHOICE,         // Preferential voting
        LIQUID_DEMOCRACY,      // Delegated voting
        QUADRATIC_VOTING,      // Cost-weighted voting
        CONVICTION_VOTING      // Time-weighted voting
    };

    struct Vote {
        std::string voter_id;
        std::string proposal_id;
        float weight{1.0f};
        std::vector<std::string> ranked_choices;
        float conviction_time{0.0f};
        bool is_delegated{false};
    };

    struct VotingResult {
        std::string winning_proposal;
        float approval_rate{0.0f};
        bool consensus_reached{false};
        std::vector<std::string> minority_concerns;
    };

private:
    struct VotingSession {
        VotingSystem system;
        std::vector<Vote> votes;
        float quorum_requirement{0.5f};
        float consensus_threshold{0.8f};
        bool requires_discussion{true};
    };

public:
    static VotingResult process_votes(
        const std::vector<Vote>& votes,
        VotingSystem system,
        float consensus_threshold = 0.8f) {
        
        switch (system) {
            case VotingSystem::MAJORITY:
                return process_majority_vote(votes);
            case VotingSystem::CONSENSUS:
                return process_consensus(votes, consensus_threshold);
            case VotingSystem::RANKED_CHOICE:
                return process_ranked_choice(votes);
            case VotingSystem::LIQUID_DEMOCRACY:
                return process_liquid_democracy(votes);
            case VotingSystem::QUADRATIC_VOTING:
                return process_quadratic_voting(votes);
            case VotingSystem::CONVICTION_VOTING:
                return process_conviction_voting(votes);
            default:
                return VotingResult{};
        }
    }

private:
    static VotingResult process_majority_vote(
        const std::vector<Vote>& votes) {
        
        // Count votes for each proposal
        std::unordered_map<std::string, float> vote_counts;
        float total_votes = 0.0f;
        
        for (const auto& vote : votes) {
            vote_counts[vote.proposal_id] += vote.weight;
            total_votes += vote.weight;
        }
        
        // Find proposal with most votes
        auto max_it = std::max_element(
            vote_counts.begin(),
            vote_counts.end(),
            [](const auto& p1, const auto& p2) {
                return p1.second < p2.second;
            }
        );
        
        return VotingResult{
            max_it->first,
            max_it->second / total_votes,
            max_it->second > (total_votes * 0.5f),
            get_minority_concerns(votes, max_it->first)
        };
    }

    static VotingResult process_consensus(
        const std::vector<Vote>& votes,
        float threshold) {
        
        // Check support level for each proposal
        std::unordered_map<std::string, float> support_levels;
        float total_weight = 0.0f;
        
        for (const auto& vote : votes) {
            support_levels[vote.proposal_id] += vote.weight;
            total_weight += vote.weight;
        }
        
        // Find proposals meeting consensus threshold
        std::vector<std::string> consensus_proposals;
        for (const auto& [proposal, support] : support_levels) {
            if ((support / total_weight) >= threshold) {
                consensus_proposals.push_back(proposal);
            }
        }
        
        if (!consensus_proposals.empty()) {
            return VotingResult{
                consensus_proposals[0],
                support_levels[consensus_proposals[0]] / total_weight,
                true,
                {}  // No minority concerns in consensus
            };
        }
        
        return VotingResult{};
    }

    static VotingResult process_ranked_choice(
        const std::vector<Vote>& votes) {
        
        std::unordered_map<std::string, float> round_counts;
        std::vector<std::string> eliminated;
        
        // Continue until we have a winner
        while (true) {
            round_counts.clear();
            
            // Count first choices that haven't been eliminated
            for (const auto& vote : votes) {
                for (const auto& choice : vote.ranked_choices) {
                    if (std::find(eliminated.begin(), eliminated.end(), 
                                choice) == eliminated.end()) {
                        round_counts[choice] += vote.weight;
                        break;
                    }
                }
            }
            
            // Check for majority
            float total = std::accumulate(
                round_counts.begin(),
                round_counts.end(),
                0.0f,
                [](float sum, const auto& pair) {
                    return sum + pair.second;
                }
            );
            
            for (const auto& [proposal, count] : round_counts) {
                if (count > total * 0.5f) {
                    return VotingResult{
                        proposal,
                        count / total,
                        true,
                        get_minority_concerns(votes, proposal)
                    };
                }
            }
            
            // Eliminate lowest scoring option
            auto min_it = std::min_element(
                round_counts.begin(),
                round_counts.end(),
                [](const auto& p1, const auto& p2) {
                    return p1.second < p2.second;
                }
            );
            
            eliminated.push_back(min_it->first);
            
            if (eliminated.size() >= round_counts.size() - 1) {
                // Only one option remains
                for (const auto& [proposal, _] : round_counts) {
                    if (std::find(eliminated.begin(), eliminated.end(), 
                                proposal) == eliminated.end()) {
                        return VotingResult{
                            proposal,
                            round_counts[proposal] / total,
                            false,
                            get_minority_concerns(votes, proposal)
                        };
                    }
                }
            }
        }
    }

    static std::vector<std::string> get_minority_concerns(
        const std::vector<Vote>& votes,
        const std::string& winning_proposal) {
        
        std::vector<std::string> concerns;
        std::unordered_set<std::string> seen_voters;
        
        for (const auto& vote : votes) {
            if (vote.proposal_id != winning_proposal && 
                !seen_voters.contains(vote.voter_id)) {
                // Record unique concerns from minority voters
                concerns.push_back(
                    get_voter_concern(vote.voter_id, winning_proposal)
                );
                seen_voters.insert(vote.voter_id);
            }
        }
        
        return concerns;
    }
}; 