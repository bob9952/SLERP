#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "model.h"
#include "camera.h"
#include "Axies.h"
#include "Line.h"
#include "Isometries.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(4.0f, 4.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool animation_on = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
constexpr float pi = glm::pi<float>();

glm::fvec3 c1 = glm::fvec3(0.0f,0.0f,0.0f);
glm::fvec3 c2 = glm::fvec3(2.0f, 1.0f, 0.0f);

float phi1 = pi / 6.0f;
float theta1 = pi / 3.0f;
float psi1 = pi / 3.0f;

float phi2 = pi;
float theta2 = pi / 3.0f;
float psi2 = 2 * pi / 5.0f;

glm::quat q;
glm::quat quat1;
glm::quat quat2;

float t = 0.0f;
float tm = 30.0f;

glm::fmat4 xyzModelMatrix;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SLERP", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("assets/shaders/1.model_loading.vs.glsl", 
        "assets/shaders/1.model_loading.fs.glsl");

    // load models
    // -----------
    Model ourModel("assets/models/backpack/backpack.obj");


    //Line line4(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.5f, 0.0f));
    //line4.setLineColor(glm::vec3(glm::vec3(0.0f)));
    std::vector<glm::fvec3> in = {
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    };
    

    Axies xyz(in);
    xyz.setLineColor(glm::fvec3(1.0f, 0.0f, 0.0f));
    
    Axies modelxyz(in);
    modelxyz.setLineColor(glm::fvec3(0.0f, 1.0f, 0.0f));

    Axies endxyz(in);
    endxyz.setLineColor(glm::fvec3(0.0f, 0.0f, 1.0f));
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::cout << "TEST 1:" << std::endl;
    Isometries::test1();
    std::cout << std::endl;

    std::cout << "MY TEST:" << std::endl;
    Isometries::my_test();
    std::cout << std::endl;

    float angle = 0.0f;


    float a = -1.0f / glm::sqrt(3.0f);
    camera.Front = glm::vec3(a, a, a);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        // MODEL FIRST 
        // BEGIN DRAW
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::translate(model, c1);
        
        auto A = Isometries::euler_2a(phi1, theta1, psi1);
        auto r = Isometries::axis_angle(GLM2E(A));
        auto p = glm::fvec3(r.first(0), r.first(1), r.first(2));
        auto angle = r.second;

        quat1 = Isometries::axis_engle_2q(p, angle);
        model = glm::rotate(model, angle , p);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        
        ourShader.setMat4("model", model);

        ourModel.Draw(ourShader);
        // END DRAW 

        
        // MODEL SECOND
        // BEGIN DRAW
        auto model2 = glm::fmat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model2 = glm::translate(model2, c2);

        auto A2 = Isometries::euler_2a(phi2, theta2, psi2);
        auto r2 = Isometries::axis_angle(GLM2E(A2));
        auto p2 = glm::fvec3(r2.first(0), r2.first(1), r2.first(2));
        auto angle2 = r2.second;

        quat2 = Isometries::axis_engle_2q(p2, angle2);
        model2 = glm::rotate(model2, angle2, p2);
        model2 = glm::scale(model2, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model2);

        ourModel.Draw(ourShader);
        //END DRAW
        
        // BEGIN DRAW ANIMATED
        if (animation_on) {
            t += deltaTime * 3.0f;
            if (t < tm) {
                auto model_anim = glm::fmat4(1.0f);
                model_anim = glm::translate(model_anim, glm::fvec3(0.0f));
                float pos1 = (1.0f - t / tm) * c1[0] + (t / tm) * c2[0];
                float pos2 = (1.0f - t / tm) * c1[1] + (t / tm) * c2[1];
                float pos3 = (1.0f - t / tm) * c1[2] + (t / tm) * c2[2];
                model_anim = glm::translate(model_anim, glm::fvec3(pos1, pos2, pos3));

                auto q = Isometries::slerp(quat1, quat2, tm, t);
                auto res = Isometries::q2_axis_angle(q);

                auto p_new = res.first;
                auto angle_new = res.second;

                model_anim = glm::rotate(model_anim, angle_new, p_new);
                model_anim = glm::scale(model_anim, glm::fvec3(0.2f));
                xyzModelMatrix = model_anim;
                ourShader.setMat4("model", model_anim);

                ourModel.Draw(ourShader);
            }
        }

        // END DRAW ANIMATED


        // MODEL FIRST XYZ
        // BEGIN DRAW
        auto m = model;
        m = glm::scale(m, glm::fvec3(1.8f));
        modelxyz.setMVP(projection * view * m);
        modelxyz.draw();
        // END DRAW

        // MODEL ANIMATED XYZ
        auto m_anim = xyzModelMatrix;
        m_anim = glm::scale(m_anim, glm::fvec3(1.8f));
        modelxyz.setMVP(projection * view * m_anim);
        modelxyz.draw();
        // END DRAW
        
        // MODEL SECOND XYZ
        // BEGIN DRAW
        auto m2 = model2;
        m2 = glm::scale(m2, glm::fvec3(2.8f));
        modelxyz.setMVP(projection * view * m2);
        modelxyz.draw();
        // END DRAW
        
        // XYZ
        // BEGIN DRAW
        model = glm::fmat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        xyz.setMVP(projection * view * model);
        xyz.draw();
        // END DRAW


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        if(!animation_on)
            animation_on = true;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
