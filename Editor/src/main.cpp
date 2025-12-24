#include "Enjin/Core/Application.h"
#include "Enjin/Logging/Log.h"
#include <iostream>
#if !defined(_WIN32)
    #include <unistd.h>
    #include <cstdio>
#endif

// Editor application
class EditorApplication : public Enjin::Application {
public:
    void Initialize() override {
        ENJIN_LOG_INFO(Editor, "Enjin Editor starting...");
    }

    void Shutdown() override {
        ENJIN_LOG_INFO(Editor, "Enjin Editor shutting down...");
    }

    void Update(Enjin::f32 deltaTime) override {
        (void)deltaTime;
        // Editor update logic
    }

    void Render() override {
        // Editor rendering logic
    }
};

Enjin::Application* CreateApplication() {
    return new EditorApplication();
}

// Entry point - Engine owns this
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    Enjin::Application* app = CreateApplication();
    int result = app->Run();
    delete app;

    std::cout << "Application exited with code " << result << "." << std::endl;
#if !defined(_WIN32)
    // Only pause when launched from an interactive terminal.
    if (isatty(fileno(stdin))) {
        std::cout << "Press Enter to close..." << std::endl;
        std::cin.get();
    }
#endif

    return result;
}
