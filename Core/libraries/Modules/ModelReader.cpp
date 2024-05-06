
#include "ModelReader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Zero {
    ModelReader::ModelReader(const char* _modelPath) {
        



        std::string modelPath; // Cadena para almacenar el contenido del archivo.
        std::ifstream modelFile; // Objeto ifstream para leer el archivo.

        modelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // Configurar excepciones para el ifstream.
        try {
            modelFile.open(_modelPath);
            float x = 0, y = 0, z = 0;
            int v1, v2, v3;
            std::string line;
            while (std::getline(modelFile, line)) {
                if (line[0] == 'v') {
                    std::istringstream iss(line.substr(2)); // Ignorar el primer carácter (que es 'v') y leer el resto de la línea.
                    iss >> x >> y >> z;
                    // Aquí puedes hacer algo con las coordenadas del vértice, como almacenarlas en algún lugar.
                    // Por ejemplo, puedes almacenarlas en un contenedor como un vector.
                    vertices.push_back({glm::vec3(x,y,z),glm::vec2(x*-1,y*-1)});
                }else if (line[0] == 'f')
                {
                    std::istringstream iss(line.substr(2)); // Ignorar el primer carácter (que es 'v') y leer el resto de la línea.
                    iss >> v1 >> v2 >> v3;
                    indices.push_back(v1-1);
                    indices.push_back(v2-1);
                    indices.push_back(v3-1);
                }
            }

            modelFile.close(); // Cerrar el archivo después de leer todas las líneas.
        }
        catch (std::ifstream::failure& e) {
            // Si ocurre un error al abrir o leer el archivo, registrar un mensaje de error utilizando el objeto Logger.
            std::cerr << "Error al abrir/leer el archivo: " << e.what() << std::endl;
        }
    }


    
}