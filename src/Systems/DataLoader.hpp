#ifndef DATALOADER_HPP
#define DATALOADER_HPP

#include <Godot.hpp>
#include <Reference.hpp>
#include <File.hpp>
#include <JSON.hpp>
#include <ProjectSettings.hpp>
#include <vector>
#include "Election.hpp"
#include "Candidate.hpp"

namespace Systems {

class DataLoader : public godot::Reference {
    GODOT_CLASS(DataLoader, godot::Reference)

public:
    static void _register_methods();
    
    DataLoader();
    ~DataLoader();

    void _init();

    std::vector<Election*> load_election_definitions(const godot::String& path);
    std::vector<Candidate*> load_candidate_definitions(const godot::String& path);

private:
    godot::Error load_json_file(const godot::String& path, godot::String& json_content);
};

} // namespace Systems

#endif // DATALOADER_HPP 