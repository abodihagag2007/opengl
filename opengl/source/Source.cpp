#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct shaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static shaderProgramSource parseShader(const std::string& filepath) {  
    enum class shadertype{
    NONE = -1 ,VERTEX = 0 ,FRAGMENT = 1
};  
    std::ifstream stream(filepath);
    std::stringstream ss[2];
    shadertype type = shadertype::NONE;
    std::string line;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                shadertype type = shadertype::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                shadertype type = shadertype::FRAGMENT;
           
        }
        else
            ss[(int)type] << line << '\n';
    }

    return { ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader( unsigned int type ,const std::string& source ) {

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE ){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) _malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment"  ) <<" shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
    }
    else
    {
        std::cout << "succesfuly compiled shader" << std::endl;
    }

    return id;
}


static unsigned int CreateShader(const std::string& vertexshader, const std::string& fragmentshader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER ,vertexshader );
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentshader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1080, 1920, "opengl", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "error" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6]{
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f

    };

    unsigned int buffer;
    glGenBuffers( 1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),positions,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    shaderProgramSource source = parseShader("\res\Shaders\Basic.shader");
    std::cout << "vertex" << std::endl; 
    std::cout << source.VertexSource << std::endl;
    std::cout <<"fragment" << std::endl;
    std::cout << source.FragmentSource << std::endl;
    //unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    //glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

     
       
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
    }
  //glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}