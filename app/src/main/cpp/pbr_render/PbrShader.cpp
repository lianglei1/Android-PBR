#include <algorithm>
#include "PbrShader.h"
#include "../util/LogUtil.h"

namespace {
    void readFile(const GLchar *path, std::string &content) {
        std::ifstream shaderFile(path);
        try {
            // open files
            shaderFile.open(path);
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();
            // close file handlers
            shaderFile.close();
            // convert stream into string
            content = shaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ file: " << path << std::endl;
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ file: " << e.what() << std::endl;
        }

    }

    void checkShaderCompileErrors(GLuint shader, GLenum type) {
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;

            LOGCATI("ERROR::SHADER_COMPILATION_ERROR of type%s", infoLog);
        }
    }

    void checkProgramCompileErrors(GLuint program) {
        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(program, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
            LOGCATI("ERROR::PROGRAM_LINKING_ERROR%s", infoLog);

        }
    }

    void compileShader(unsigned int &shaderId, const char *code, GLenum type) {
        // vertex Shader
        shaderId = glCreateShader(type);
        glShaderSource(shaderId, 1, &code, NULL);
        glCompileShader(shaderId);
        checkShaderCompileErrors(shaderId, type);
    }
}

PbrShader::PbrShader(const std::string vertexPath, const std::string fragmentPath) {
    // shader Program
    mID = glCreateProgram();

    unsigned int vertex, fragment;

    std::string vertexCode;
    //readFile(vertexPath.c_str(), vertexCode);
    vertexCode = vertexPath;
    compileShader(vertex, vertexCode.c_str(), GL_VERTEX_SHADER);
    glAttachShader(mID, vertex);

    std::string fragmentCode;
    //readFile(fragmentPath.c_str(), fragmentCode);
    fragmentCode = fragmentPath;
    compileShader(fragment, fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    glAttachShader(mID, fragment);

    glLinkProgram(mID);
    checkProgramCompileErrors(mID);
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

PbrShader::~PbrShader() {
    release();
}

GLuint PbrShader::getId() const {
    return mID;
}

void PbrShader::use() const {
    glUseProgram(mID);
}

void PbrShader::release() {
    glDeleteProgram(mID);
    mID = 0;
}

void PbrShader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(mID, name.c_str()), (int) value);
}

void PbrShader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}

void PbrShader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
}

void PbrShader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(mID, name.c_str()), 1, &value[0]);
}

void PbrShader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(mID, name.c_str()), x, y);
}

void PbrShader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(mID, name.c_str()), 1, &value[0]);
}

void PbrShader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(mID, name.c_str()), x, y, z);
}

void PbrShader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(mID, name.c_str()), 1, &value[0]);
}

void PbrShader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(mID, name.c_str()), x, y, z, w);
}

void PbrShader::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void PbrShader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void PbrShader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
