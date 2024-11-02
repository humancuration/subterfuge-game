#include "register_types.hpp"
#include "AI/AIController.hpp"
#include "AI/NPCController.hpp"
#include "Core/Entity.hpp"
#include "Core/GameState.hpp"
#include "Core/NameDatabase.hpp"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>

using namespace godot;

void initialize_ai_module() {
    ClassDB::register_class<AIController>();
    ClassDB::register_class<NPCController>();
}

void initialize_core_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<Entity>();
    ClassDB::register_class<NodeStats>();
    ClassDB::register_class<NodeData>();
    ClassDB::register_class<NPCData>();
    ClassDB::register_class<GameState>();
    ClassDB::register_class<NameDatabase>();
}

void uninitialize_ai_module() {
    // Cleanup if necessary
}

extern "C" {
    GDExtensionBool GDE_EXPORT ai_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
        
        init_obj.register_initializer(initialize_ai_module);
        init_obj.register_initializer(initialize_core_module);
        init_obj.register_terminator(uninitialize_ai_module);
        
        return init_obj.init();
    }
} 