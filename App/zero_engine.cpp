#include <App.h>
#include <GL_graphics/Shader.h>
#include <filesystem>


int main(int argc, char* argv[])
{
    // Obtener el directorio del ejecutable
    std::filesystem::path exePath = std::filesystem::canonical(std::filesystem::path(argv[0]).parent_path());

    // Navegar hacia arriba en la jerarquía de directorios (dos niveles en este caso)
    std::filesystem::path rootPath = exePath.parent_path().parent_path().parent_path().parent_path();
    std::filesystem::path workingDirectory = rootPath;
    std::filesystem::current_path(workingDirectory);

	zeroengine::App app;
	app.run();
	app.stop();
}