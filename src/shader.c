#include "shader.h"

ShaderProgram shader_create(
    const char *vertex_path, const char *fragment_path)
{

    char *source = NULL;
    FILE *file = fopen(vertex_path, "r");
    assert(file);
    fseek(file, 0, SEEK_END);
    int count = ftell(file);
    fseek(file, 0, SEEK_SET);
    source = malloc(count + 1);
    assert(source);
    count = fread(source, 1, count, file);
    fclose(file);
    source[count] = '\0';
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char **)&source, NULL);
    glCompileShader(vertex_shader);
    char info_log[512];
    int success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(
            stderr, 
            "%s:%d: shader error: %s\n", 
            __FILE__, __LINE__, info_log);
        abort();
    }

    file = fopen(fragment_path, "r");
    assert(file);
    fseek(file, 0, SEEK_END);
    count = ftell(file);
    fseek(file, 0, SEEK_SET);
    source = malloc(count + 1);
    assert(source);
    count = fread(source, 1, count, file);
    fclose(file);
    source[count] = '\0';
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char **)&source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(
            stderr, 
            "%s:%d: shader error: %s\n", 
            __FILE__, __LINE__, info_log);
        abort();
    }

    unsigned int id;
    id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, info_log);
        fprintf(
            stderr, 
            "%s:%d: shader error: %s\n", 
            __FILE__, __LINE__, info_log);
        abort();
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    return (ShaderProgram){id};
}
void shader_use(ShaderProgram program)
{
    glUseProgram(program.id);
}
void shader_set_bool(
    ShaderProgram program, const char *name, _Bool value)
{
    glUniform1i(glGetUniformLocation(program.id, name), (int)value);
}
void shader_set_int(
    ShaderProgram program, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(program.id, name), value);
}
void shader_set_float(
    ShaderProgram program, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(program.id, name), value);
}
void shader_set_vec3f(
    ShaderProgram program, const char *name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(program.id, name), x, y, z);
}