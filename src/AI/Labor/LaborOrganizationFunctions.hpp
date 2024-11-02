#pragma once
#include "LaborOrganizationSystem.hpp"
#include "../Core/VotingSystem.hpp"

// Helper functions for group decision making
inline std::vector<std::string> get_member_proposals(const std::string& member_id) {
    std::vector<std::string> proposals;
    
    // Get member's current capabilities and needs
    auto capabilities = get_member_capabilities(member_id);
    auto needs = get_member_needs(member_id);
    
    // Generate proposals based on capabilities and needs
    for (const auto& capability : capabilities) {
        if (matches_group_need(capability, needs)) {
            proposals.push_back(
                create_proposal(capability, needs)
            );
        }
    }
    
    return proposals;
}

inline float measure_group_support(const WorkGroup& group, 
                                 const std::string& action) {
    float total_support = 0.0f;
    
    for (const auto& member_id : group.member_ids) {
        // Consider member's voting power
        float voting_power = get_member_voting_power(member_id);
        
        // Consider member's expertise in this area
        float expertise_factor = get_member_expertise(member_id, action);
        
        // Consider member's past success with similar actions
        float success_factor = get_member_success_rate(member_id, action);
        
        // Combine factors for final vote
        float member_support = calculate_member_support(
            voting_power,
            expertise_factor,
            success_factor
        );
        
        total_support += member_support;
    }
    
    return total_support / group.member_ids.size();
}

inline std::vector<std::string> get_member_skills(const std::string& member_id) {
    std::vector<std::string> skills;
    
    // Get member's learned skills
    auto learned_skills = get_learned_skills(member_id);
    
    // Get member's potential skills (based on capabilities)
    auto potential_skills = get_potential_skills(member_id);
    
    // Combine both sets
    skills.insert(skills.end(), learned_skills.begin(), learned_skills.end());
    skills.insert(skills.end(), potential_skills.begin(), potential_skills.end());
    
    return skills;
}

inline bool can_teach_skill(const std::string& member_id, 
                          const std::string& skill) {
    // Check skill mastery level
    float mastery = get_skill_mastery(member_id, skill);
    
    // Check teaching ability
    float teaching_ability = get_teaching_ability(member_id);
    
    // Check previous teaching success
    float teaching_success = get_teaching_success_rate(member_id);
    
    return mastery > 0.7f && teaching_ability > 0.5f && 
           teaching_success > 0.6f;
}

inline void arrange_skill_transfer(const std::string& teacher_id,
                                 const std::string& student_id,
                                 const std::string& skill) {
    // Create teaching arrangement
    SkillTransfer transfer{
        teacher_id,
        student_id,
        skill,
        calculate_transfer_efficiency(teacher_id, student_id, skill)
    };
    
    // Schedule teaching sessions
    schedule_skill_transfer(transfer);
    
    // Set up progress tracking
    track_skill_transfer_progress(transfer);
}

inline float calculate_collective_skill(const WorkGroup& group) {
    float total_skill = 0.0f;
    
    // Consider individual skills
    for (const auto& member_id : group.member_ids) {
        total_skill += calculate_member_skill_contribution(member_id);
    }
    
    // Consider synergy bonuses
    float synergy_bonus = calculate_group_synergy(group);
    
    // Consider skill complementarity
    float complementarity_bonus = calculate_skill_complementarity(group);
    
    return (total_skill / group.member_ids.size()) * 
           (1.0f + synergy_bonus) * 
           (1.0f + complementarity_bonus);
}

inline ResourcePool gather_group_resources(const WorkGroup& group) {
    ResourcePool pool;
    
    // Gather individual contributions
    for (const auto& member_id : group.member_ids) {
        auto member_resources = get_member_resources(member_id);
        pool.merge(member_resources);
    }
    
    // Apply group bonuses
    if (group.organization_type == OrganizationType::WORKER_COOPERATIVE) {
        pool.apply_cooperative_bonus();
    }
    
    return pool;
}

inline void distribute_resources_to_member(
    const std::string& member_id,
    const ResourcePool& resources,
    float share) {
    
    // Calculate member's portion
    ResourcePool member_portion = resources.calculate_portion(share);
    
    // Apply any member-specific bonuses
    apply_member_resource_bonuses(member_portion, member_id);
    
    // Transfer resources to member
    transfer_resources_to_member(member_id, member_portion);
    
    // Record distribution for transparency
    record_resource_distribution(member_id, member_portion);
}

inline float calculate_need_factor(const std::string& member_id) {
    // Base need on current resources
    float base_need = 1.0f - get_current_resources(member_id);
    
    // Consider dependents
    float dependent_factor = get_dependent_count(member_id) * 0.2f;
    
    // Consider special circumstances
    float special_needs = calculate_special_needs(member_id);
    
    return std::clamp(base_need + dependent_factor + special_needs, 
                     0.0f, 1.0f);
}

inline float calculate_contribution_factor(const std::string& member_id) {
    // Consider work hours
    float work_factor = get_work_hours(member_id) / 40.0f;
    
    // Consider skill level
    float skill_factor = get_average_skill_level(member_id);
    
    // Consider past contributions
    float historical_factor = get_historical_contribution(member_id);
    
    return std::clamp(
        (work_factor + skill_factor + historical_factor) / 3.0f,
        0.0f, 1.0f
    );
} 