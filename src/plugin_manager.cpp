#include "plugin_manager.h"

#include "../include/plugin_interface.h"
#include "../include/platform.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

struct PluginManager::PluginHandle {
    void* library_handle = nullptr;
    PluginInitFunc initialize = nullptr;
    PluginCleanupFunc cleanup = nullptr;
    
    PluginHandle() = default;
    ~PluginHandle() = default;
};

PluginManager::PluginManager()
{
    std::cout << "Initializing plugin manager in: " << PLUGIN_DIRECTORY << std::endl;

    if (!fs::exists(PLUGIN_DIRECTORY)) {
		std::cout << "PLUGIN_DIRECTORY not found" << std::endl;
		return;
	}

    for (const auto& entry : fs::directory_iterator(PLUGIN_DIRECTORY)) {
        if (!entry.is_regular_file()) continue;
        
		std::string extension = entry.path().extension().string();
        if (extension != ("." + std::string(LIBRARY_EXTENSION))) continue;
		
        std::string plugin_path = entry.path().string();
        std::cout << "Found plugin: " << plugin_path << std::endl;
		
        void* handle = loadPluginLibrary(plugin_path);
        if (!handle) {
            std::cerr << "Failed to load plugin: " << plugin_path << std::endl;
            continue;
        }
        
        auto plugin = std::make_unique<PluginHandle>();
        plugin->library_handle = handle;
        
        plugin->initialize = reinterpret_cast<PluginInitFunc>(getSymbolAddress(handle, "initialize"));
        plugin->cleanup = reinterpret_cast<PluginCleanupFunc>(getSymbolAddress(handle, "cleanup"));
        
        if (!plugin->initialize || !plugin->cleanup) {
            std::cerr << "Plugin missing required functions: " << plugin_path << std::endl;
            unloadPluginLibrary(handle);
            continue;
        }
        
        plugin->initialize();
		
        _loaded_plugins.push_back(std::move(plugin));
    }
    
    std::cout << "Loaded " << _loaded_plugins.size() << " plugins" << std::endl;
}

PluginManager::~PluginManager() {
    for (auto& plugin : _loaded_plugins) {
        if (plugin && plugin->cleanup) {
            plugin->cleanup();
        }
        
        if (plugin && plugin->library_handle) {
            unloadPluginLibrary(plugin->library_handle);
        }
    }
    
    _loaded_plugins.clear();
}

void* PluginManager::loadPluginLibrary(const std::string& path) {
#ifdef _WIN32
	return LoadLibraryA(path.c_str());
#else
	return dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void PluginManager::unloadPluginLibrary(void* handle) {
#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(handle));
#else
    dlclose(handle);
#endif
}

void* PluginManager::getSymbolAddress(void* handle, const char* symbol_name) {
#ifdef _WIN32
    return GetProcAddress(static_cast<HMODULE>(handle), symbol_name);
#else
    return dlsym(handle, symbol_name);
#endif
}