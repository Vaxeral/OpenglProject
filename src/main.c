#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <assert.h>

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
        SDL_WINDOW_OPENGL);
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
    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(indices), 
        indices, 
        GL_STATIC_DRAW);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(vertices), 
        vertices, 
        GL_STATIC_DRAW);
    
    FILE *file = fopen("res/vertex.glsl", "r");
    assert(file);
    char buffer[512] = {0};
    int count = fread(buffer, 1, 512, file);
    fclose(file);
    buffer[count] = '\0';
    const char *str = buffer;
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &str, NULL);
    glCompileShader(vertex_shader);
    char info_log[512];
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
    file = fopen("res/fragment.glsl", "r");
    assert(file);
    count = fread(buffer, 1, 512, file);
    fclose(file);
    buffer[count] = '\0';
    str = buffer;
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &str, NULL);
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

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(
            stderr, 
            "%s:%d: shader error: %s\n", 
            __FILE__, __LINE__, info_log);
        abort();
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
    int quit = 0;
    while(!quit)
    {
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
                break;
            }
        }
        if(keyboard_state[SDL_SCANCODE_ESCAPE])
        {
            quit = 1;
        }
        glClearColor(1.0f, 0.3f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int ticks = SDL_GetTicks();
        float green = (sin(ticks / 1000.0f) / 2.0f) + 0.5f;
        int vertex_color_location = glGetUniformLocation(
            shader_program, "ourColor");
        glUseProgram(shader_program);
        glUniform4f(
            vertex_color_location, 0.0f, green, 0.0f, 1.0f);
        glBindVertexArray(vao);
        /*glDrawArrays(GL_TRIANGLES, 0, 3);*/
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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