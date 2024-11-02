#pragma once
#include "LaborOrganizationSystem.hpp"

// Helper functions for group dynamics
inline void process_group_decisions(WorkGroup& group) {
    // Track voting/consensus process
    float agreement_level = 0.0f;
    std::vector<std::string> proposed_actions;
    
    // Each member contributes ideas
    for (const auto& member_id : group.member_ids) {
        auto proposals = get_member_proposals(member_id);
        proposed_actions.insert(
            proposed_actions.end(),
            proposals.begin(),
            proposals.end()
        );
    }
    
    // Deliberation process
    for (const auto& action : proposed_actions) {
        float support = measure_group_support(group, action);
        if (support > 0.7f) { // Consensus threshold
            group.shared_practices.push_back(action);
            agreement_level += support;
        }
    }
    
    // Update group cohesion based on decision process
    group.group_cohesion *= (0.8f + 0.2f * (agreement_level / proposed_actions.size()));
}

inline void facilitate_skill_sharing(WorkGroup& group) {
    // Track who can teach what
    std::unordered_map<std::string, std::vector<std::string>> teacher_skills;
    
    // Identify teachers
    for (const auto& member_id : group.member_ids) {
        auto skills = get_member_skills(member_id);
        for (const auto& skill : skills) {
            if (can_teach_skill(member_id, skill)) {
                teacher_skills[skill].push_back(member_id);
            }
        }
    }
    
    // Organize learning pairs
    for (const auto& [skill, teachers] : teacher_skills) {
        for (const auto& member_id : group.member_ids) {
            if (needs_skill(member_id, skill)) {
                // Match with best teacher
                std::string teacher_id = find_best_teacher(teachers, member_id);
                arrange_skill_transfer(teacher_id, member_id, skill);
            }
        }
    }
    
    // Update collective skill level
    group.collective_skill = calculate_collective_skill(group);
}

inline void distribute_resources_fairly(WorkGroup& group) {
    // Track available resources
    ResourcePool available_resources = gather_group_resources(group);
    
    // Calculate fair shares based on need and contribution
    std::unordered_map<std::string, float> fair_shares;
    for (const auto& member_id : group.member_ids) {
        float need_factor = calculate_need_factor(member_id);
        float contribution_factor = calculate_contribution_factor(member_id);
        fair_shares[member_id] = (need_factor + contribution_factor) / 2.0f;
    }
    
    // Normalize shares
    float total_shares = std::accumulate(
        fair_shares.begin(), 
        fair_shares.end(), 
        0.0f,
        [](float sum, const auto& pair) { return sum + pair.second; }
    );
    
    // Distribute resources
    for (auto& [member_id, share] : fair_shares) {
        float portion = share / total_shares;
        distribute_resources_to_member(member_id, available_resources, portion);
    }
}

inline void update_group_cohesion(WorkGroup& group, float delta_time) {
    // Factors affecting cohesion
    float success_factor = measure_recent_success(group);
    float cooperation_factor = measure_cooperation_level(group);
    float fairness_factor = measure_resource_distribution_fairness(group);
    
    // Update cohesion
    float target_cohesion = (success_factor + cooperation_factor + fairness_factor) / 3.0f;
    group.group_cohesion = std::lerp(
        group.group_cohesion,
        target_cohesion,
        delta_time * 0.1f  // Gradual change
    );
    
    // Record particularly successful practices
    if (group.group_cohesion > 0.8f) {
        record_successful_practices(group);
    }
}

inline void coordinate_swarm_behavior(WorkGroup& group) {
    // Track local interactions
    std::vector<InteractionRule> active_rules;
    
    // Update interaction rules based on success
    for (const auto& practice : group.shared_practices) {
        if (is_practice_successful(practice)) {
            active_rules.push_back(create_interaction_rule(practice));
        }
    }
    
    // Apply swarm rules
    for (const auto& member_id : group.member_ids) {
        // Get local neighborhood
        auto neighbors = get_nearby_members(member_id);
        
        // Apply rules
        for (const auto& rule : active_rules) {
            apply_interaction_rule(member_id, neighbors, rule);
        }
    }
}

inline void optimize_task_distribution(WorkGroup& group) {
    // Track task preferences and capabilities
    std::unordered_map<std::string, float> task_efficiencies;
    
    // Measure individual performances
    for (const auto& member_id : group.member_ids) {
        auto performed_tasks = get_member_tasks(member_id);
        for (const auto& task : performed_tasks) {
            float efficiency = measure_task_efficiency(member_id, task);
            task_efficiencies[task] = std::max(
                task_efficiencies[task],
                efficiency
            );
        }
    }
    
    // Optimize assignments
    for (const auto& task : get_pending_tasks()) {
        std::string best_member = find_best_member_for_task(
            group.member_ids,
            task,
            task_efficiencies
        );
        assign_task(best_member, task);
    }
} 