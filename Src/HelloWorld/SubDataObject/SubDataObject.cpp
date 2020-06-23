#include "Src/HelloWorld/SubDataObject/SubDataObject.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Src/Shapes/Shapes.hpp"

SubDataObject::SubDataObject(std::vector<float> &verticies,std::vector<float> &colors, std::vector<unsigned> &indicies, Transform &aTransform) : transform(aTransform)
{
    indiciesSize = indicies.size() * sizeof(unsigned);

    glGenBuffers(1, &objectBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, objectBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 verticies.size() * sizeof(float) + colors.size() * sizeof(float) + indicies.size() * sizeof(unsigned), 0, GL_STATIC_DRAW);

    GLsizeiptr currentoffset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, 0, verticies.size() * sizeof(float), &verticies[0]);
    currentoffset += verticies.size() * sizeof(float);
    glBufferSubData(GL_ARRAY_BUFFER, currentoffset, colors.size() * sizeof(unsigned), &colors[0]);
    currentoffset += colors.size() * sizeof(float);
    objectIndexByteOffset = currentoffset;
    glBufferSubData(GL_ARRAY_BUFFER, currentoffset, indicies.size() * sizeof(unsigned), &indicies[0]);

    glGenVertexArrays(1, &vertexArrayObject);

    glBindVertexArray(vertexArrayObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, objectBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(colors.size() * sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectBuffer);

    /* 
    params:  1 (0) attrib number - layout location
             2 (3) attrib size  [ 3 floats for vec3 x y z]
             3 (GL_FLOAT) type of attrib
             4 (GL_FALSE) if data is normalized
             5 ( 6 * sizeof(float) )  the space between consecutive vertex attributes
             6 ((void *)0) - offset of position data
    
*/
}

void SubDataObject::Draw(Shader *shader, std::vector<float> &verticies,std::vector<float> &colors, std::vector<unsigned> &indicies)
{
    glUseProgram(shader->shaderProgramID);

    unsigned int transformLoc = glGetUniformLocation(shader->shaderProgramID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform.getTransform()));

      glBindBuffer(GL_ARRAY_BUFFER, objectBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 verticies.size() * sizeof(float) + colors.size() * sizeof(float) + indicies.size() * sizeof(unsigned), 0, GL_STATIC_DRAW);


   GLsizeiptr currentoffset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, 0, verticies.size() * sizeof(float), &verticies[0]);
    currentoffset += verticies.size() * sizeof(float);
    glBufferSubData(GL_ARRAY_BUFFER, currentoffset, colors.size() * sizeof(unsigned), &colors[0]);
    currentoffset += colors.size() * sizeof(float);
    objectIndexByteOffset = currentoffset;
    glBufferSubData(GL_ARRAY_BUFFER, currentoffset, indicies.size() * sizeof(unsigned), &indicies[0]);
    
    glBindVertexArray(vertexArrayObject);

    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, reinterpret_cast<void *>(objectIndexByteOffset));
    glBindVertexArray(0);
}

SubDataObject::~SubDataObject()
{
    glDeleteVertexArrays(1, &objectBuffer);
    glDeleteBuffers(1, &vertexArrayObject);

    printf("SubDataObject removed \n");
}
