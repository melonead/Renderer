/*
    v  - vertex
    vt - texture coordinates
    vn - vertex normals
    vp - parameter spaces vertices
    f  - polygonal face element
*/
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


struct mesh_vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};

static float findMin(float a, float b)
{
    return a < b ? a : b;
}

static float findMax(float a, float b)
{
    return a > b ? a : b;
}


class Model_Loader {
public:
    Model_Loader();
    std::vector<unsigned int>& get_indices();
    void load_model(std::string path);
    std::vector<float> getPositions();
    std::vector<float> getTexCoords();
    glm::vec3 getMin() {return min;}
    glm::vec3 getMax() {return max;}
    unsigned int getVerticesCount();
private:

    unsigned int verticesCount{0};

    std::vector<float> sortedPositions;
    std::vector<glm::vec3> sortedNormals;
    std::vector<float> sortedTexCoords;
    std::vector<unsigned int> sortedIndices;

    std::vector<unsigned int> getVertexAttributeIndices(std::string& faceKey);
    std::vector<std::string> getFaceKeys(std::string& unProcessedFaceValues);
    glm::vec3 min{99999, 99999, 99999};
    glm::vec3 max{-min};
};
