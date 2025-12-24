#include "Enjin/Core/Application.h"
#include "Enjin/Logging/Log.h"

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
    return result;
}
