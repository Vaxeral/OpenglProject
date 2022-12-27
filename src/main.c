/*
    Visualize dot product in opengl.
*/

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include <cglm/cglm.h>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <assert.h>

float *generate_vertices(int *chunk, int width, int height, int depth)
{
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; ++j)
        {
            for(int k = 0; k < height; ++k)
            {
                int index = i + j * width + k * width * height;
                if(chunk[index])
                {
                    
                }
            }
        }
    }
}

#define SDL_ASSERT(assertion) \
(assertion) ? \
    (void)0 : \
    (fprintf(stderr, "%s:%d: assertion failed: %s\n", \
        __FILE__, __LINE__, SDL_GetError()), abort())

/*https://stackoverflow.com/questions/32294913/getting-continuous-window-resize-event-in-sdl-2*/
static int on_window_resize(void *data, SDL_Event *event);

int main(int argc, char const *argv[])
{
    int error = SDL_Init(SDL_INIT_VIDEO);
    SDL_ASSERT(!error);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, 
        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window *window = SDL_CreateWindow(
        "", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        640, 480, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_ASSERT(window);
    SDL_AddEventWatch(on_window_resize, window);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_ASSERT(gl_context);
    int success = gladLoadGLLoader(
        (GLADloadproc)SDL_GL_GetProcAddress);
    SDL_ASSERT(success);
    SDL_GL_SetSwapInterval(1);
    int nattribute;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nattribute);
    fprintf(stderr, "max attributes: %d\n", nattribute);
    glViewport(0, 0, 640, 480);
    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    unsigned char *data = stbi_load(
        "res/container.jpg", &width, &height, &channels, 0);
    assert(data);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, 
        GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, 
        width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    data = stbi_load(
        "res/awesomeface.png", &width, &height, &channels, 0);
    assert(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
        GL_TEXTURE_2D, 
        GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, 
        GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, 
        width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    mat4 projection;
    glm_mat4_copy(GLM_MAT4_IDENTITY, projection);

    glm_perspective(
        M_PI / 4.0f, 640.0f / 480.0f, 0.1f, 100.0f, projection);

    mat4 model;
    glm_mat4_copy(GLM_MAT4_IDENTITY, model);

    mat4 view;
    glm_mat4_copy(GLM_MAT4_IDENTITY, view);
    glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(indices), 
        indices, 
        GL_STATIC_DRAW);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(vertices), 
        vertices, 
        GL_STATIC_DRAW);
    
    ShaderProgram program = shader_create(
        "res/vertex.vert", "res/fragment.frag");

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        5 * sizeof(float), 
        (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
/*    glVertexAttribPointer(
        2, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        8 * sizeof(float), 
        (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);*/

    shader_use(program);
    shader_set_int(program, "texture1", 0);
    shader_set_int(program, "texture2", 1);
    int location = glGetUniformLocation(program.id, "texture_coords");
    float texture_coords[4 * 4] = {
        0.0, 0.0, 0.5, 0.5,
        0.5, 0.0, 0.5, 0.5,
        0.5, 0.5, 0.5, 0.5,
        0.0, 0.5, 0.5, 0.5
    };
    glUniform4fv(program.id, 4, texture_coords);


    float offsetx, offsety;
    offsetx = offsety = 0;

    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
    int quit = 0;
    while(!quit)
    {
        Uint32 start = SDL_GetTicks();

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_WINDOWEVENT:
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE:
                    quit = 1;
                    break;
                }
                break;
                case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_w) {
                    static GLenum mode = GL_FILL;
                    if(mode == GL_FILL)
                        mode = GL_LINE;
                    else
                        mode = GL_FILL;
                    glPolygonMode(
                        GL_FRONT_AND_BACK, mode);
                }
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                    offsety -= 1.0f / 480.0f;
                    break;
                    case SDLK_UP:
                    offsety += 1.0f / 480.0f;
                    break;
                    case SDLK_LEFT:
                    offsetx -= 1.0f / 640.0f;
                    break;
                    case SDLK_RIGHT:
                    offsetx += 1.0f / 640.0f;
                    break;
                }
                break;
            }
        }
        if(keyboard_state[SDL_SCANCODE_ESCAPE])
        {
            quit = 1;
        }
        glClearColor(1.0f, 0.3f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int ticks = SDL_GetTicks();
        float green = (sin(ticks / 1000.0f) / 2.0f) + 0.5f;
/*        int vertex_color_location = glGetUniformLocation(
            shader_program, "ourColor");*/
        shader_use(program);

        int model_location = glGetUniformLocation(
            program.id, "model");


        glm_mat4_copy(GLM_MAT4_IDENTITY, model);

        glm_rotate(
            model, 
            (SDL_GetTicks() / 1000.0f) * M_PI / 180.0f * 50.0f, 
            (vec3){0.5f, 1.0f, 0.0f});

        glUniformMatrix4fv(
            model_location, 1, GL_FALSE, (float *)model);

        int view_location = glGetUniformLocation(
            program.id, "view");
        glUniformMatrix4fv(
            view_location, 1, GL_FALSE, (float *)view);

        int projection_location = glGetUniformLocation(
            program.id, "projection");
        glUniformMatrix4fv(
            projection_location, 1, GL_FALSE, (float *)projection);

/*        glUniform4f(
            vertex_color_location, 0.0f, green, 0.0f, 1.0f);*/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(vao);
        /*glDrawArrays(GL_TRIANGLES, 0, 3);*/
        glDrawArrays(GL_TRIANGLES, 0, 36);

        SDL_GL_SwapWindow(window);
    }
    return 0;
}

static int on_window_resize(void *data, SDL_Event *event)
{
    if(event->type == SDL_WINDOWEVENT 
        && event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
        if(window == (SDL_Window *)data)
        {
            int width, height;
            SDL_GetWindowSize(window, &width, &height);
            glViewport(0, 0, width, height);
        }
    }
    return 0;
}