#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>

namespace Data {

class EventData : public godot::Resource {
    GDCLASS(EventData, Resource)
private:
    String id;
    String title_key;
    String description;
    Dictionary conditions;
    Array choices;

protected:
    static void _bind_methods();

public:
    EventData();
    
    void set_id(const String& p_id) { id = p_id; }
    String get_id() const { return id; }
    
    void set_title_key(const String& p_key) { title_key = p_key; }
    String get_title_key() const { return title_key; }
    
    void set_description(const String& p_desc) { description = p_desc; }
    String get_description() const { return description; }
    
    void set_conditions(const Dictionary& p_conditions) { conditions = p_conditions; }
    Dictionary get_conditions() const { return conditions; }
    
    void set_choices(const Array& p_choices) { choices = p_choices; }
    Array get_choices() const { return choices; }
};

class CandidateData : public godot::Resource {
    GDCLASS(CandidateData, Resource)
private:
    String name;
    String election_type;
    Dictionary platform;

protected:
    static void _bind_methods();

public:
    CandidateData();
    
    void set_name(const String& p_name) { name = p_name; }
    String get_name() const { return name; }
    
    void set_election_type(const String& p_type) { election_type = p_type; }
    String get_election_type() const { return election_type; }
    
    void set_platform(const Dictionary& p_platform) { platform = p_platform; }
    Dictionary get_platform() const { return platform; }
};

} 