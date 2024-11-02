#pragma once
#include "SpeciesEvolutionSystem.hpp"
#include <random>

class GeneticMutationSystem {
private:
    struct MutationEffect {
        alignas(16) float magnitude;
        alignas(16) float stability;
        std::string affected_trait;
        std::function<void(Species*)> mutation_effect;
    };

    // SIMD-aligned mutation data
    alignas(32) std::vector<float> mutation_chances;
    alignas(32) std::vector<float> mutation_strengths;
    
    static constexpr float BASE_MUTATION_RATE = 0.001f;  // 0.1% per generation
    static constexpr float STRESS_MUTATION_MULTIPLIER = 2.0f;
    
    std::mt19937 rng{std::random_device{}()};

public:
    void process_mutations_simd(Species* species, float environmental_stress) {
        // Process gene mutations in SIMD batches
        const size_t gene_count = species->genome.size();
        
        for (size_t i = 0; i < gene_count; i += 8) {
            __m256 mutation_rates = calculate_mutation_rates_simd(species, i, environmental_stress);
            apply_mutations_simd(species, i, mutation_rates);
        }
        
        // Check for emergent traits
        check_trait_emergence(species);
    }

private:
    __m256 calculate_mutation_rates_simd(Species* species, size_t start_idx, float stress) {
        alignas(32) float base_rates[8];
        for (size_t i = 0; i < 8; ++i) {
            if (start_idx + i < species->genome.size()) {
                base_rates[i] = species->genome[start_idx + i].mutation_chance;
            }
        }
        
        __m256 rates = _mm256_load_ps(base_rates);
        __m256 stress_vec = _mm256_set1_ps(stress * STRESS_MUTATION_MULTIPLIER);
        
        return _mm256_mul_ps(rates, stress_vec);
    }

    void apply_mutations_simd(Species* species, size_t start_idx, __m256 mutation_rates) {
        alignas(32) float chances[8];
        _mm256_store_ps(chances, mutation_rates);
        
        for (size_t i = 0; i < 8 && start_idx + i < species->genome.size(); ++i) {
            if (std::uniform_real_distribution<float>(0.0f, 1.0f)(rng) < chances[i]) {
                apply_single_mutation(species, start_idx + i);
            }
        }
    }

    void apply_single_mutation(Species* species, size_t gene_idx) {
        auto& gene = species->genome[gene_idx];
        
        // Different types of mutations
        enum class MutationType {
            POINT_MUTATION,    // Small change in expression
            DUPLICATION,       // Copy gene
            DELETION,         // Remove gene
            REGULATION,       // Change activation conditions
            FUSION           // Combine with another gene
        };

        switch (get_random_mutation_type()) {
            case MutationType::POINT_MUTATION:
                apply_point_mutation(gene);
                break;
            case MutationType::DUPLICATION:
                duplicate_gene(species, gene_idx);
                break;
            case MutationType::DELETION:
                if (species->genome.size() > 1) {  // Prevent deletion of last gene
                    delete_gene(species, gene_idx);
                }
                break;
            case MutationType::REGULATION:
                modify_gene_regulation(gene);
                break;
            case MutationType::FUSION:
                if (species->genome.size() > 1) {
                    fuse_genes(species, gene_idx);
                }
                break;
        }
    }

    void check_trait_emergence(Species* species) {
        // Check for new trait combinations
        std::vector<std::string> active_traits;
        for (const auto& gene : species->genome) {
            if (gene.expression_level > 0.5f) {
                active_traits.push_back(gene.trait_id);
            }
        }

        // Example emergent traits
        if (has_trait_combination(active_traits, {"HighTemperatureTolerance", "WaterConservation"})) {
            add_emergent_trait(species, "DesertAdaptation");
        }

        if (has_trait_combination(active_traits, {"NightVision", "PackHunting"})) {
            add_emergent_trait(species, "NocturnalPredator");
        }

        if (has_trait_combination(active_traits, {"Photosynthesis", "WaterFiltering"})) {
            add_emergent_trait(species, "SymbioticAlgae");
        }
    }

    void add_emergent_trait(Species* species, const std::string& trait_id) {
        Gene new_gene{
            trait_id,
            1.0f,  // Full expression
            BASE_MUTATION_RATE,
            true   // Dominant
        };
        species->genome.push_back(new_gene);
        species->genetic_diversity += 0.1f;  // Increase diversity
    }

    // Helper functions for different mutation types
    void apply_point_mutation(Gene& gene) {
        std::normal_distribution<float> dist(0.0f, 0.1f);
        gene.expression_level = std::clamp(
            gene.expression_level + dist(rng),
            0.0f, 1.0f
        );
    }

    void duplicate_gene(Species* species, size_t gene_idx) {
        Gene new_gene = species->genome[gene_idx];
        new_gene.mutation_chance *= 1.5f;  // Increased mutation chance for duplicated genes
        species->genome.push_back(new_gene);
    }

    void delete_gene(Species* species, size_t gene_idx) {
        species->genome.erase(species->genome.begin() + gene_idx);
    }

    void modify_gene_regulation(Gene& gene) {
        gene.mutation_chance = std::clamp(
            gene.mutation_chance * (1.0f + std::normal_distribution<float>(0.0f, 0.2f)(rng)),
            0.0001f, 0.1f
        );
    }

    void fuse_genes(Species* species, size_t gene_idx) {
        size_t other_idx = (gene_idx + 1) % species->genome.size();
        Gene& gene1 = species->genome[gene_idx];
        Gene& gene2 = species->genome[other_idx];
        
        // Create new fused gene
        Gene fused_gene{
            gene1.trait_id + "_" + gene2.trait_id,
            (gene1.expression_level + gene2.expression_level) * 0.5f,
            std::max(gene1.mutation_chance, gene2.mutation_chance) * 1.2f,
            gene1.is_dominant || gene2.is_dominant
        };
        
        // Replace original genes with fused gene
        species->genome[gene_idx] = fused_gene;
        species->genome.erase(species->genome.begin() + other_idx);
    }
}; 