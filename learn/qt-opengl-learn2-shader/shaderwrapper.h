#ifndef SHADERWRAPPER_H
#define SHADERWRAPPER_H

#include <string>
using namespace std;
class ShaderWrapper
{
public:
    ShaderWrapper();

public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    ShaderWrapper(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

};

#endif // SHADERWRAPPER_H
