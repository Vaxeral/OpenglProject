#ifndef SHADER_HEADER_INCLUDED
#define SHADER_HEADER_INCLUDED

#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    unsigned int id;
} ShaderProgram;

ShaderProgram shader_create(
    const char *vertex_path, const char *fragment_path);
void shader_use(ShaderProgram program);
void shader_set_bool(
    ShaderProgram program, const char *name, _Bool value);
void shader_set_int(
    ShaderProgram program, const char *name, int value);
void shader_set_float(
    ShaderProgram program, const char *name, float value);
void shader_set_vec3f(
    ShaderProgram program, const char *name, float x, float y, float z);


#endif