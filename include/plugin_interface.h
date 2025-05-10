#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#if defined(_WIN32) || defined(__CYGWIN__)
    #define PLUGIN_API extern "C" __declspec(dllexport)
#else
    #define PLUGIN_API extern "C" __attribute__((visibility("default")))
#endif

typedef void (*PluginInitFunc)();
typedef void (*PluginCleanupFunc)();

PLUGIN_API void initialize();
PLUGIN_API void cleanup();

#endif // PLUGIN_INTERFACE_H