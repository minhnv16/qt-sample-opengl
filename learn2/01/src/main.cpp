#include <stdio.h>
#include <iostream>
// Nhúng các thư viện cần thiết
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Định nghĩa các hàm callback xử lý sự kiện cửa sổ
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// Cài đặt chiều rộng và chiều cao của cửa sổ
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Mã nguồn cho Vertex Shader (viết bằng GLSL)
const char * vertexShaderSource = R"GLSL(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main(){
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    }
)GLSL";

// Mã nguồn cho Fragment Shader (viết bằng GLSL)
const char *fragmentShaderSource = R"GLSL(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
       FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    };
)GLSL";


int main()
{
    printf("Hello\n");

    // 1. Khởi tạo GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. Tạo một đối tượng cửa sổ
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "test OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    // 3. Khởi tạo glew
    //init glew
    GLenum err = glewInit();

    if (GLEW_OK != err) {
        glfwTerminate();
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 5. Liên kết các shader vào một chương trình shader (Shader Program)
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Xóa các shader vì chúng đã được liên kết vào chương trình
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


        // 6. Định nghĩa dữ liệu đỉnh cho tam giác
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Góc dưới bên trái
         0.5f, -0.5f, 0.0f, // Góc dưới bên phải
         0.0f,  0.5f, 0.0f  // Góc trên cùng
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Liên kết VAO đầu tiên, sau đó liên kết và cấu hình VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Cấu hình thuộc tính đỉnh (Vertex Attributes)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Hủy liên kết VBO và VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // 7. Vòng lặp chính (render loop)
    while (!glfwWindowShouldClose(window))
    {
        // Xử lý input
        processInput(window);

        // Render commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Vẽ tam giác
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Hoán đổi buffer và thăm dò các sự kiện
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 8. Giải phóng tài nguyên
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;

}

// Hàm callback khi kích thước cửa sổ thay đổi
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Hàm xử lý input
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

