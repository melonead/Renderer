#include "objloader.h"
#include <algorithm>
#include "string_funcs.h"
#include <map>

Model_Loader::Model_Loader() {

}


void Model_Loader::load_model(std::string path) {
    // read file
    std::ifstream file_stream{path, std::ios_base::in};

    std::string line;

    std::vector<std::string> position;
    std::vector<std::string> normal;
    std::vector<std::string> t_coord;

    std::vector<glm::vec3> unsortedPositions;
    std::vector<glm::vec3> unsortedNormals;
    std::vector<glm::vec2> unsortedTexCoords;

    // This map is used to avoid duplication of vertices that are
    // similar, two vertices are similar if they are the same attributes.
    // For example, same position, tex coord and normal values (indices).
    std::map<std::string, unsigned int> verticesMap;
    // This is the index for the whole vertex, as opposed to an index to
    // an attribute value.
    unsigned int vertexIndex = 0;

    std::string space = " ";
    std::string slash = "/";
    std::string vertexPositionDelimeter = "v ";
    std::string vertexNormalDelimeter = "vn";
    std::string vertexTexCoordDelimeter = "vt";
    std::string vertexFaceDelimeter = "f ";


    while(std::getline(file_stream, line)) {
        if (line.length() < 2)
            continue;

        

        // positions
        if (line.substr(0, 2) == vertexPositionDelimeter) {
            position = std::move(split_string(line, space));
            unsortedPositions.push_back(glm::vec3(std::stof(position[0]), std::stof(position[1]), std::stof(position[2])));
        }
        
        // normals
        else if (line.substr(0, 2) == vertexNormalDelimeter) {
            normal = std::move(split_string(line, space));
            unsortedNormals.push_back(glm::vec3(std::stof(normal[0]), std::stof(normal[1]), std::stof(normal[2])));
        }
        // texture coordinates
        else if (line.substr(0, 2) == vertexTexCoordDelimeter) {
            t_coord = std::move(split_string(line, space));
            unsortedTexCoords.push_back(glm::vec2(std::stof(t_coord[0]), std::stof(t_coord[1])));
        }

        // indices
        else if (line.substr(0, 2) == vertexFaceDelimeter) {

            unsigned int firstVertexIndexOfFaces = vertexIndex;

            // Put face key into the map with value of vertex index.
            std::vector<std::string> faceKeys = std::move(getFaceKeys(line));
            
            unsigned int numberOfTriangles = faceKeys.size() - 2;
            // For each face key test if it already exists in the map.
            for (auto& key : faceKeys)
            {

                if (verticesMap.find(key) != verticesMap.end())
                {
                    continue;
                }
                else
                {
                    verticesMap[key] = vertexIndex;
                    vertexIndex += 1;
                }
            }

            //verticesCount += numberOfTriangles * 3;

            // Generate indices for groups of triangles.
            // The number of triangles will be numberOfFaces - 2.
            // We are generating the list of indices of a single triangle from current value of
            // vertexIndex.

            // Arrangement of the triangles.
            unsigned int k = firstVertexIndexOfFaces;
            for (unsigned int i = 0; i < (numberOfTriangles); i++)
            {   
                // Indices to the triangle vertices in faceKeys array.
                unsigned int firstIndex = 0;
                unsigned int secondIndex = firstIndex + i + 1;
                unsigned int thirdIndex = secondIndex + 1;
                // The first vertex is shared by all the triangles.

                sortedIndices.push_back(verticesMap[faceKeys[firstIndex]]);
                sortedIndices.push_back(verticesMap[faceKeys[secondIndex]]);
                sortedIndices.push_back(verticesMap[faceKeys[thirdIndex]]);
            }
            
        }
        

    }

    // Reserve enough space for these containers. So that
    // we do not get errors when we index and save on the 
    // amount of resizing that will be needed if we do not 
    // do this.
    verticesCount = verticesMap.size();
    
    sortedPositions.resize(verticesCount * 3);
    sortedNormals.resize(verticesCount);
    sortedTexCoords.resize(verticesCount * 2);

    // Process the map of vertices.
    for (auto& vertexKeyValue : verticesMap)
    {   
        // This key is the string format of the attribute indices for this vertex.
        auto key = vertexKeyValue.first;
        // This is the index of the vertex associated with the key above.
        unsigned int value = vertexKeyValue.second;
        assert(value >= 0 && "value is less than zero");

        // 0th attribute -> position index
        // 1st attribute -> vertex normal index
        // 2nd attribute -> texture coord index
        std::vector<unsigned int> vertexAttributeIndices = std::move(getVertexAttributeIndices(key));

        // Fill in the sorted attribute containers
        // Minus 1 from attribute indices because obj format starts 
        // count from 1 not 0.
        glm::vec3 pos = unsortedPositions[vertexAttributeIndices[0] - 1];
        sortedPositions[value * 3] = pos.x;
        sortedPositions[value * 3 + 1] = pos.y;
        sortedPositions[value * 3 + 2] = pos.z;
        glm::vec2 texCoord = unsortedTexCoords[vertexAttributeIndices[1] - 1];
        sortedTexCoords[value * 2] = texCoord.x;
        sortedTexCoords[value * 2 + 1] = texCoord.y;
        sortedNormals[value] = unsortedNormals[vertexAttributeIndices[2] - 1];
    }

    
    file_stream.close();
}

std::vector<float> Model_Loader::getPositions()
{
    return sortedPositions;
}

unsigned int Model_Loader::getVerticesCount()
{
    return verticesCount;
}

std::vector<unsigned int> Model_Loader::getVertexAttributeIndices(std::string& faceKey)
{
    std::string slash = "/";
    std::vector<unsigned int> attributeIndices;
    std::vector<std::string> stringIndices = split_string(faceKey, slash);

    for (auto& strIndex : stringIndices)
    {
        unsigned int ind = (unsigned int)std::stoi(strIndex);
        assert(ind >= 0 && "Index value is less than zero");
        attributeIndices.push_back(ind);
    }
    return attributeIndices;
}

std::vector<std::string> Model_Loader::getFaceKeys(std::string& unProcessedFaceValues)
{
    std::string space = " ";
    std::vector<std::string> splitFaceKeys = split_string(unProcessedFaceValues, space);
    return splitFaceKeys;
}

std::vector<unsigned int>& Model_Loader::get_indices() {
    return sortedIndices;
}

std::vector<float> Model_Loader::getTexCoords() {
    return sortedTexCoords;
}

/*
    Debugger: variable is optimized away and not available. Why?
*/