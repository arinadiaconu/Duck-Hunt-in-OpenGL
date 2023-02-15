#include "lab_m1/Tema1/object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <cmath>


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, length, 0), color),
        VertexFormat(corner + glm::vec3(length * 2, length / 2, 0), color)
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 circleCenter,
    glm::vec3 color,
    bool fill) {
    glm::vec3 center = circleCenter;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    vertices.push_back(VertexFormat(center, color));                // the origin of the circle
    vertices.push_back(VertexFormat(glm::vec3(1, 0, 0), color));    // the first point on the circle
    indices.push_back(0);
    indices.push_back(1);

    int number_of_triangles = 100;

    for (int i = 2; i <= number_of_triangles; i++) {
        //0.0174532925 radians == 1 degree
        vertices.push_back(VertexFormat(glm::vec3(cos(i * (0.0174532925 * (360.0 / number_of_triangles))),
                            sin(i * (0.0174532925 * (360.0 / number_of_triangles))), 0), color));
        indices.push_back(i);
    }
    indices.push_back(1);

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);

    circle->InitFromData(vertices, indices);
    return circle;
}

