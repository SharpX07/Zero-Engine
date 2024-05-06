#include <App.h>
#include <ECS/EntityManager.h>

int main(int argc, char* argv[]) {
    Zero::EntityManager* M_EntityManager = new Zero::EntityManager();
    Zero::App app;
    app.run();
    app.stop();
}