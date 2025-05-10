#ifndef PLUGIN_EXPORT_H
#define PLUGIN_EXPORT_H

#ifdef _WIN32
    #ifdef EXPORTS
        #define DLLEXPORT __declspec(dllexport)
    #else
        #define DLLEXPORT __declspec(dllimport)
    #endif
#else
    #define DLLEXPORT __attribute__((visibility("default")))
#endif

#endif // PLUGIN_EXPORT_H