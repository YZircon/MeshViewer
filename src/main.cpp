#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include <iostream>

#include "Geometry.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"

constexpr double MY_PI = 3.141592653589;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;

float lastTime = 0.0f;

void framebuffer_size_callback(GLFWwindow* window,int width,int height){//用来在用户手动调整窗口大小时回调,重新调整视口大小和窗口大小,保证物体不会变形
    /*更新screen width和screen height*/
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    /*更新视口*/
    glViewport(0,0,width,height);
}

PerspectiveCamera MainCam(Eigen :: Vector3f(0.0f, 0.0f, 0.0f), Eigen :: Vector3f(0.0f, 0.0f, -1.0f), Eigen :: Vector3f(0.0f, 1.0f, 0.0f), 0.5, 45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
Light MainLight(Eigen :: Vector3f(10.0f, 10.0f, 10.0f), Eigen :: Vector3f(1.0f, 1.0f, 1.0f), 0.5);

void processInput(GLFWwindow* window, float deltaTime){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){//如果在window窗口检测到esc被按下
        glfwSetWindowShouldClose(window,true);// 设置window应该被关闭
    }
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){ //拉近
        MainCam.handleKeyboardInput(GLFW_KEY_W, deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){ //远离
        MainCam.handleKeyboardInput(GLFW_KEY_S, deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){ //左移
        MainCam.handleKeyboardInput(GLFW_KEY_A, deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){ //右移
        MainCam.handleKeyboardInput(GLFW_KEY_D, deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){ //上移
        MainCam.handleKeyboardInput(GLFW_KEY_Q, deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS){ //下移
        MainCam.handleKeyboardInput(GLFW_KEY_E, deltaTime);
    }
}

void update(Shader &shader, Camera &camera, Eigen :: Vector3f Scale){
    shader.activate();
    glClearColor(0.5, 0.5, 0.5, 1.0f);

    Eigen :: Matrix4f Model = get_model_matrix(0.0, Scale);
    Eigen :: Matrix4f View = camera.GetViewMatrix();
    Eigen :: Matrix4f Projection = camera.GetProjectionMatrix();

    shader.SetMat4("Model", Model);
    shader.SetMat4("View", View);
    shader.SetMat4("Projection", Projection);

    shader.SetVec3("LightColor", MainLight.GetColor()); // 设置光源颜色
    shader.SetVec3("LightPos", MainLight.GetPosition()); // 设置光源位置
    shader.SetVec3("EyePos", MainCam.GetPos());
    shader.deactivate();
}

void InitConfig(){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);//GLFW大版本号为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);//GLFW小版本号3
    //因此GLFW版本号为3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//告诉GLFW我们使用OpenGL的核心模式
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);//禁止窗口大小被调整
}

int main(){

    glfwInit();//初始化GLFW

    InitConfig();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"MeshViewer", nullptr,nullptr);//创建一个GLFW窗口对象,它的指针放在 window 中
    if(window == nullptr){
        std :: cout << "Failed to create GLFW window" << std :: endl;
        glfwTerminate();//终止 GLFW
        return -1;
    }

    glfwMakeContextCurrent(window);//创建窗口
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);//如果窗口大小发生改变,调用framebuffer_size_callback函数重新设置视口大小为新值
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std :: cout << "Failed to initialize GLAD" << std :: endl;
        return -1;
    }

    //注意,一切gl开头(opengl本身)的函数都需要用glad管理其函数指针,因此必须先执行gladLoadGLLoader
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);//设置视口左下角位置以及渲染窗口的宽度和高度,但openGL中的坐标范围是[-1,1]

    //Shader Shader("../src/shader/basic.vert", "../src/shader/basic.frag"); // you can name your shader files however you like
    Shader Shader("../src/shader/Phong.vert", "../src/shader/Phong.frag"); // you can name your shader files however you like
    /*std :: vector <Vertex> vert;
    for(int i = 0; i < vertices[0].size(); i += 6){
        vert.push_back({Eigen :: Vector3f(vertices[0][i],vertices[0][i + 1], vertices[0][i + 2]), Eigen :: Vector3f(vertices[0][i + 3],vertices[0][i + 4], vertices[0][i + 5]), Eigen :: Vector2f(0.0f, 0.0f)});
    }
    Mesh Test(vert);*/

    Model Lumine("../models/lumine/Lumine.obj", &Shader);
    //Model CornellBox("../models/CornellBox/cornell_box.obj", &Shader);
    //Model BoxStack("../models/box_stack/box_stack.obj", &Shader);
    //Model StanfordBunny("../models/stanford-bunny/stanford-bunny.obj", &Shader); // 加载模型必须放在主函数内,否则OpenGL还没初始化,显然会崩溃

    std :: unordered_map <std :: string, float> BoundingBox = Lumine.GetBoundingBox(); /*放相机 TODO: 把不同大小的模型缩放到同样大小*/
    std :: cout << std :: fixed << BoundingBox["x_max"] - BoundingBox["x_min"] << std :: endl;
    float ylim = std :: max((BoundingBox["y_max"] - BoundingBox["y_min"]) / 2.0, MainCam.GetAspectRatio() * (BoundingBox["x_max"] - BoundingBox["x_min"]) / 2.0); // 计算 y 方向需要的最小空间(max(y方向本身需要的, x方向受aspect ratio影响导致y方向需要的))
    float d = ylim / tan((MainCam.GetEyeFov() / 2.0) * MY_PI / 180.0); // 用fov计算需要向 -z 移动的距离, 记得先转弧度制
    MainCam.SetPosition(Eigen :: Vector3f((BoundingBox["x_min"] + BoundingBox["x_max"]) / 2.0, (BoundingBox["y_min"] + BoundingBox["y_max"]) / 2.0, BoundingBox["z_max"] + d)); // 把相机移动到正面中心, 用几何关系算出完全显示物体所需要使用的最短距离d, 把相机向 z 方向(看向-z方向)拉 d
    MainLight.SetPosition(Eigen :: Vector3f(BoundingBox["x_max"], BoundingBox["y_max"], (BoundingBox["z_max"] + BoundingBox["z_min"]) / 2.0 + d)); // 设置光源
    std :: cout << MainCam.GetPosition() << std :: endl;

    std :: vector <Vertex> FloorVert; // 设置地板
    float deltaX = BoundingBox["x_max"] - BoundingBox["x_min"], deltaZ = BoundingBox["z_max"] - BoundingBox["z_min"];
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_min"] - deltaX, BoundingBox["y_min"], BoundingBox["z_min"] - deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_max"] + deltaX, BoundingBox["y_min"], BoundingBox["z_min"] - deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_min"] - deltaX, BoundingBox["y_min"], BoundingBox["z_max"] + deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_max"] + deltaX, BoundingBox["y_min"], BoundingBox["z_min"] - deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_min"] - deltaX, BoundingBox["y_min"], BoundingBox["z_max"] + deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_max"] + deltaX, BoundingBox["y_min"], BoundingBox["z_max"] + deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});

    Mesh Floor(FloorVert);

    printf("Load Finish!\n");

    while(!glfwWindowShouldClose(window)){ //"游戏循环"
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float nowTime = glfwGetTime();
        float deltaTime = nowTime - lastTime;
        lastTime = nowTime;
        processInput(window, deltaTime);
        update(Shader, MainCam, Lumine.GetScale());
        Lumine.draw();
        Floor.draw();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
