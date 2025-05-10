#include <string>
#include <vector>

#define PLUGIN_DIRECTORY "./plugins"

class PluginManager {
public:
    PluginManager();
    ~PluginManager();
    
private:
    struct PluginHandle;
    std::vector<std::unique_ptr<PluginHandle>> _loaded_plugins;
    
    void* loadPluginLibrary(const std::string& path);
    void unloadPluginLibrary(void* handle);
    void* getSymbolAddress(void* handle, const char* symbol_name);
};