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
DirectedLight MainLight(Eigen :: Vector3f(10.0f, 10.0f, 10.0f), Eigen :: Vector3f(1.0f, 1.0f, 1.0f), 0.5, Eigen :: Vector3f(0.0, 0.0, -1.0));

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

void updateDepthMap(Shader &shader, DirectedLight &light, Eigen :: Vector3f Scale, Eigen :: Vector3f DirectionVec){
    shader.activate();
    glClearColor(0.5, 0.5, 0.5, 1.0f);

    Eigen :: Matrix4f Model = get_model_matrix(0.0, Scale, DirectionVec);
    Eigen :: Matrix4f View = light.GetViewMatrix();
    Eigen :: Matrix4f Projection = light.GetProjectionMatrix();

    shader.SetMat4("Model", Model);
    shader.SetMat4("View", View);
    shader.SetMat4("Projection", Projection);

    shader.deactivate();
}

void update(Shader &shader, Camera &camera, DirectedLight light, Eigen :: Vector3f Scale, Eigen :: Vector3f DirectionVec){
    shader.activate();
    glClearColor(0.5, 0.5, 0.5, 1.0f);

    Eigen :: Matrix4f Model = get_model_matrix(0.0, Scale, DirectionVec);
    Eigen :: Matrix4f View = camera.GetViewMatrix();
    Eigen :: Matrix4f Projection = camera.GetProjectionMatrix();

    Eigen :: Matrix4f LightView = light.GetViewMatrix();
    Eigen :: Matrix4f LightProjection = light.GetProjectionMatrix();

    shader.SetMat4("Model", Model);
    shader.SetMat4("View", View);
    shader.SetMat4("Projection", Projection);
    shader.SetMat4("LightSpaceMatrix", LightProjection * LightView);

    shader.SetVec3("LightColor", light.GetColor()); // 设置光源颜色
    shader.SetVec3("LightPos", light.GetPosition()); // 设置光源位置
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

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
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

    Shader BasicShader("../src/shader/basic.vert", "../src/shader/basic.frag"); // you can name your shader files however you like
    Shader PhongShader("../src/shader/Phong.vert", "../src/shader/Phong.frag"); // you can name your shader files however you like
    Shader DepthShader("../src/shader/depth.vert", "../src/shader/depth.frag"); // you can name your shader files however you like
    Shader DebugShader("../src/shader/debug.vert", "../src/shader/debug.frag"); // you can name your shader files however you like
    /*std :: vector <Vertex> vert;
    for(int i = 0; i < vertices[0].size(); i += 6){
        vert.push_back({Eigen :: Vector3f(vertices[0][i],vertices[0][i + 1], vertices[0][i + 2]), Eigen :: Vector3f(vertices[0][i + 3],vertices[0][i + 4], vertices[0][i + 5]), Eigen :: Vector2f(0.0f, 0.0f)});
    }
    Mesh Test(vert);*/

    Model Lumine("../models/lumine/Lumine.obj", &PhongShader);
    //Model Lumine("../models/CornellBox/cornell_box.obj", &Shader); //CornellBox,叫lumine是为了调试方便
    Model LightCube("../models/Cube/Cube.obj", &BasicShader); //标记光源用的cube
    //Model Lumine("../models/box_stack/box_stack.obj", &PhongShader); //BoxStack
    //Model StanfordBunny("../models/stanford-bunny/stanford-bunny.obj", &Shader); // 加载模型必须放在主函数内,否则OpenGL还没初始化,显然会崩溃

    std :: unordered_map <std :: string, float> BoundingBox = Lumine.GetBoundingBox(); /*放相机 TODO: 把不同大小的模型缩放到同样大小*/

    std :: vector <Vertex> FloorVert; // 设置地板, 目前对地板用了模型的scale因此要用缩放前的包围盒算
    float deltaX = BoundingBox["x_max"] - BoundingBox["x_min"], deltaZ = BoundingBox["z_max"] - BoundingBox["z_min"];
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_min"] - deltaX, BoundingBox["y_min"], BoundingBox["z_min"] - deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_max"] + deltaX, BoundingBox["y_min"], BoundingBox["z_min"] - deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_min"] - deltaX, BoundingBox["y_min"], BoundingBox["z_max"] + deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_max"] + deltaX, BoundingBox["y_min"], BoundingBox["z_min"] - deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_min"] - deltaX, BoundingBox["y_min"], BoundingBox["z_max"] + deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});
    FloorVert.push_back({Eigen :: Vector3f(BoundingBox["x_max"] + deltaX, BoundingBox["y_min"], BoundingBox["z_max"] + deltaZ),Eigen :: Vector3f(0.0f, 1.0f, 0.0f),Eigen :: Vector2f(0.0, 0.0)});

    Mesh Floor(FloorVert);

    float scale = std :: max({BoundingBox["x_max"] - BoundingBox["x_min"], BoundingBox["y_max"] - BoundingBox["y_min"], BoundingBox["z_max"] - BoundingBox["z_min"]});
    Lumine.SetScale(Eigen :: Vector3f(1.0 / scale, 1.0 / scale, 1.0 / scale)); // 把最长的边压到(0,1)

    Lumine.UpdateBoundingBox(); //更新当前Scale下的包围盒
    BoundingBox = Lumine.GetBoundingBox();


    float ylim = std :: max((BoundingBox["y_max"] - BoundingBox["y_min"]) / 2.0, MainCam.GetAspectRatio() * (BoundingBox["x_max"] - BoundingBox["x_min"]) / 2.0); // 计算 y 方向需要的最小空间(max(y方向本身需要的, x方向受aspect ratio影响导致y方向需要的))
    float d = ylim / tan((MainCam.GetEyeFov() / 2.0) * MY_PI / 180.0); // 用fov计算需要向 -z 移动的距离, 记得先转弧度制
    MainCam.SetPosition(Eigen :: Vector3f((BoundingBox["x_min"] + BoundingBox["x_max"]) / 2.0, (BoundingBox["y_min"] + BoundingBox["y_max"]) / 2.0, BoundingBox["z_max"] + d)); // 把相机移动到正面中心, 用几何关系算出完全显示物体所需要使用的最短距离d, 把相机向 z 方向(看向-z方向)拉 d
    MainLight.SetPosition(Eigen :: Vector3f(-1.0,1.0, 1.0)); // 设置光源
    MainLight.SetDirection(-MainLight.GetPosition());

    LightCube.SetScale(Eigen :: Vector3f(0.05, 0.05, 0.05));
    LightCube.SetCenterPos(MainLight.GetPosition());
    LightCube.UpdateBoundingBox();

    std :: cout << MainCam.GetPosition() << std :: endl;
    std :: cout << MainLight.GetPosition() << std :: endl;

    printf("Load Finish!\n");

    //初始化Shadow Mapping 用的深度图
    const unsigned int SHADOW_WIDTH = 1024;
    const unsigned int SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    DebugShader.activate();
    DebugShader.SetInt("depthMap", 0);

    while(!glfwWindowShouldClose(window)){ //"游戏循环"
        float nowTime = glfwGetTime();
        float deltaTime = nowTime - lastTime;
        lastTime = nowTime;
        processInput(window, deltaTime);

        //TODO: Shadow Mapping
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

        glClear(GL_DEPTH_BUFFER_BIT);
        updateDepthMap(DepthShader, MainLight, Lumine.GetScale(), Lumine.GetDirectionVec());
        Lumine.SetShader(&DepthShader);
        Lumine.draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //正式绘制当前画面
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        PhongShader.activate();
        glActiveTexture(GL_TEXTURE0 + 16);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        PhongShader.SetInt("shadowTexture.texture",16);
        PhongShader.SetBool("shadowTexture.activate", true);


        update(PhongShader, MainCam, MainLight, Lumine.GetScale(), Lumine.GetDirectionVec());
        Lumine.SetShader(&PhongShader);
        Lumine.draw();

        PhongShader.activate(); // 地板是mesh,自身没有shader, 需要手动激活
        Floor.draw();
        PhongShader.deactivate();

        update(BasicShader, MainCam, MainLight, LightCube.GetScale(), LightCube.GetDirectionVec());
        LightCube.draw();

        /*DebugShader.activate();
        DebugShader.SetFloat("near_plane", 1.0f);
        DebugShader.SetFloat("far_plane",  7.5f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderQuad();
        DebugShader.deactivate();*/

        // 检测事件,交换屏幕缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
