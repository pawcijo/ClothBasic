#pragma once

#include "Src/Shader/Shader.hpp"
#include "Src/Transform/Transform.hpp"

#include <vector>
class SubDataObject3D
{

    /*
    //Vertex Array buffer
    unsigned VAO; //simply verticies (id)
    // Vertex Buffer object
    unsigned VBO; //simpy object of verticies (id)
    // Element buffer object
    unsigned EBO; // simply object of indicies (id)

*/

    unsigned indiciesSize;
    
public:
    Transform& transform;
    SubDataObject3D(std::vector<float>& verticies,std::vector<unsigned>& indicies,Transform& transform);
    ~SubDataObject3D();
    void Draw(Shader *shaderProgram/*,std::vector<glm::vec3>& verticies*/);
};

//Notes 

/*

To fill data use: 
glBufferSubData

example : 
glBufferSubData(GL_ARRAY_BUFFER, 24, sizeof(data), &data); // Range: [24, 24 + sizeof(data)]

                        OR


copy the data in memory yourself
 glMapBuffer 
 example :

 float data[] = {
  0.5f, 1.0f, -0.35f
  [...]
};
glBindBuffer(GL_ARRAY_BUFFER, buffer);

void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

memcpy(ptr, data, sizeof(data));

// make sure to tell OpenGL we're done with the pointer
glUnmapBuffer(GL_ARRAY_BUFFER);


float positions[] = { ... };
float normals[] = { ... };
float tex[] = { ... };
// fill buffer
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), &normals);
glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(tex), &tex);


glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));  
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));

*/