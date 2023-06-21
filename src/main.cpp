#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;


float lastTime = 0.0f;

void framebuffer_size_callback(GLFWwindow* window,int width,int height){//用来在用户手动调整窗口大小时回调,重新调整视口大小和窗口大小,保证物体不会变形
    /*更新screen width和screen height*/
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    /*更新视口*/
    glViewport(0,0,width,height);
}

PerspectiveCamera MainCam(Eigen :: Vector3f(0.0f, 0.0f, 0.0f), Eigen :: Vector3f(0.0f, 0.0f, -1.0f), Eigen :: Vector3f(0.0f, 1.0f, 0.0f), 0.5, 45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true; // 鼠标是否第一次进入窗口
bool isMouseCaptured = true; // 记录鼠标是否被捕获,默认是捕获

DirectedLight MainLight(Eigen :: Vector3f(10.0f, 10.0f, 10.0f), Eigen :: Vector3f(1.0f, 1.0f, 1.0f), 0.5, Eigen :: Vector3f(0.0, 0.0, -1.0));

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float sensitivity = 0.10f;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if(isMouseCaptured) MainCam.handleMouseInput(xoffset * sensitivity, yoffset * sensitivity); // 只有在鼠标被捕获的状态下才调整相机
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if ((key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) && action == GLFW_PRESS){
        if (isMouseCaptured){
            isMouseCaptured = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    if ((key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) && action == GLFW_RELEASE){
        if (!isMouseCaptured){
            isMouseCaptured = true;
            firstMouse = true; // 重新捕获鼠标要被当作鼠标第一次进入窗口处理
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void processInput(GLFWwindow* window, float deltaTime){
    if(!isMouseCaptured) return; // 鼠标没被捕获的情况下也不处理键盘操作
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

static void HelpMarker(const char* desc){ // ImGui 帮助标记,会绘制一个(?), 鼠标移动到上面就显示desc作为帮助信息
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


void InitConfig(){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);//GLFW大版本号为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);//GLFW小版本号3
    //因此GLFW版本号为3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//告诉GLFW我们使用OpenGL的核心模式
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);//禁止窗口大小被调整
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std :: cout << "Failed to initialize GLAD" << std :: endl;
        return -1;
    }

    //ImGui界面库初始化
    ImGui :: CreateContext();//创建ImGui的上下文
    ImGui_ImplGlfw_InitForOpenGL(window,true);//用GLFW创建的的窗口初始化ImGui
    ImGui :: StyleColorsDark();//窗口配色风格设置为深色
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);//ImGui需要以字符串的形式传入glsl的版本号

    //注意,一切gl开头(opengl本身)的函数都需要用glad管理其函数指针,因此必须先执行gladLoadGLLoader
    //glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);//设置视口左下角位置以及渲染窗口的宽度和高度,但openGL中的坐标范围是[-1,1]

    Shader BasicShader("../src/shader/basic.vert", "../src/shader/basic.frag"); // you can name your shader files however you like
    Shader PhongShader("../src/shader/Phong.vert", "../src/shader/Phong.frag"); // you can name your shader files however you like
    Shader DepthShader("../src/shader/depth.vert", "../src/shader/depth.frag"); // you can name your shader files however you like
    Shader DebugShader("../src/shader/debug.vert", "../src/shader/debug.frag"); // you can name your shader files however you like
    Shader LightCubeShader("../src/shader/LightCube.vert", "../src/shader/LightCube.frag"); // you can name your shader files however you like
    /*std :: vector <Vertex> vert;
    for(int i = 0; i < vertices[0].size(); i += 6){
        vert.push_back({Eigen :: Vector3f(vertices[0][i],vertices[0][i + 1], vertices[0][i + 2]), Eigen :: Vector3f(vertices[0][i + 3],vertices[0][i + 4], vertices[0][i + 5]), Eigen :: Vector2f(0.0f, 0.0f)});
    }
    Mesh Test(vert);*/

    Model Lumine("../models/lumine/Lumine.obj", &PhongShader);
    Model LightCube("../models/Cube/Cube.obj", &LightCubeShader); //标记光源用的cube
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
    MainLight.SetPosition(Eigen :: Vector3f(0.0, 0.0, 1.0)); // 设置光源
    MainLight.SetDirection(-MainLight.GetPosition());

    LightCube.SetScale(Eigen :: Vector3f(0.05, 0.05, 0.05));
    LightCube.SetCenterPos(MainLight.GetPosition());
    LightCube.UpdateBoundingBox();

    //std :: cout << MainCam.GetPosition() << std :: endl;
    //std :: cout << MainLight.GetPosition() << std :: endl;

    printf("Load Finish!\n");

    //初始化Shadow Mapping 用的深度图
    const unsigned int SHADOW_WIDTH = 4096;
    const unsigned int SHADOW_HEIGHT = 4096;
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

    //DebugShader.activate();
    //DebugShader.SetInt("depthMap", 0);

    //ImGui中调节的设定值
    bool EnableShadowMapping = true;
    unsigned int MyFlag = 1; // CheckBoxFlags复选框用的标记

    while(!glfwWindowShouldClose(window)){ //"游戏循环"
        float nowTime = glfwGetTime();
        float deltaTime = nowTime - lastTime;
        lastTime = nowTime;
        processInput(window, deltaTime);

        /*用ImGui绘制UI*/
        ImGui_ImplOpenGL3_NewFrame();//OpenGL的每一帧绘制循环时均要通知ImGui进入了新的一帧
        ImGui_ImplGlfw_NewFrame();
        ImGui :: NewFrame();

        /*操作窗口*/
        ImGui :: SetNextWindowPos(ImVec2(0,SCR_HEIGHT * 2 / 8));//设置窗体位置
        ImGui :: SetNextWindowSize(ImVec2(SCR_WIDTH / 4,SCR_HEIGHT / 4));//设置窗体大小

        ImGui::Begin("Settings");
        ImGui::CheckboxFlags("Shadow Mapping", &MyFlag, (1 << 0)); // 把复选框与位掩码1关联(第一个二进制位),从而用一个Flag管理一组复选框
        EnableShadowMapping = (MyFlag & 1) ? 1 : 0;
        ImGui::SameLine(); HelpMarker("Enable Shadow Mapping");
        if(ImGui::CollapsingHeader("Camera Configuration")){
            ImGuiIO& io = ImGui::GetIO();
            if(ImGui :: TreeNode("Camera #1")){
                float pos1[3] = {MainCam.GetPosition().x(), MainCam.GetPosition().y(), MainCam.GetPosition().z()};
                ImGui :: InputFloat3("Position", pos1);
                MainCam.SetPosition(Eigen :: Vector3f(pos1[0], pos1[1], pos1[2]));

                float dir1[3] = {MainCam.GetDirection().x(), MainCam.GetDirection().y(), MainCam.GetDirection().z()};
                ImGui :: InputFloat3("Direction", dir1);
                MainCam.SetDirection(Eigen :: Vector3f(dir1[0], dir1[1], dir1[2]));

                ImGui :: TreePop(); // 不Pop会assertion fail
            }
        }
        if(ImGui :: CollapsingHeader("Light Configuration")) {
            if(ImGui :: TreeNode("Light #1")){
                float pos1[3] = {MainLight.GetPosition().x(), MainLight.GetPosition().y(), MainLight.GetPosition().z()};
                ImGui :: InputFloat3("Position", pos1);
                MainLight.SetPosition(Eigen :: Vector3f(pos1[0], pos1[1], pos1[2]));
                LightCube.SetCenterPos(Eigen :: Vector3f(pos1[0], pos1[1], pos1[2]));
                LightCube.UpdateBoundingBox();

                float dir1[3] = {MainLight.GetDirection().x(), MainLight.GetDirection().y(), MainLight.GetDirection().z()};
                ImGui :: InputFloat3("Direction", dir1);
                MainLight.SetDirection(Eigen :: Vector3f(dir1[0], dir1[1], dir1[2]));
                ImGui :: SameLine();
                if(ImGui ::Button("Autoset")){
                    MainLight.SetDirection(-MainLight.GetPosition());
                }

                float col1[3] = {MainLight.GetColor().x(), MainLight.GetColor().y(), MainLight.GetColor().z()};
                ImGui::ColorEdit3("Color", col1);
                MainLight.SetColor(Eigen :: Vector3f(col1[0], col1[1], col1[2]));

                ImGui :: TreePop();
            }
        }
        ImGui::End();

        /*数据显示窗口*/
        ImGui :: SetNextWindowPos(ImVec2(0,SCR_HEIGHT * 6 / 8));//设置窗体位置
        ImGui :: SetNextWindowSize(ImVec2(SCR_WIDTH / 4,SCR_HEIGHT / 8));//设置窗体大小
        ImGui :: Begin("Scene Info");
        ImGui :: Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui :: End();

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

        PhongShader.activate(); //手动绑定阴影贴图
        glActiveTexture(GL_TEXTURE0 + 16);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        PhongShader.SetInt("shadowTexture.texture",16);
        //PhongShader.SetBool("shadowTexture.activate", true);
        PhongShader.SetBool("shadowTexture.activate", EnableShadowMapping); // 在ImGui中配置是否启用Shadow Mapping


        update(PhongShader, MainCam, MainLight, Lumine.GetScale(), Lumine.GetDirectionVec());
        Lumine.SetShader(&PhongShader);
        Lumine.draw();

        PhongShader.activate(); // 地板是mesh,自身没有shader, 需要手动激活
        Floor.draw();
        PhongShader.deactivate();

        update(LightCubeShader, MainCam, MainLight, LightCube.GetScale(), LightCube.GetDirectionVec());
        LightCubeShader.activate();
        LightCubeShader.SetVec3("aColor", MainLight.GetColor());
        LightCubeShader.deactivate();
        LightCube.draw();

        /*DebugShader.activate(); // 这一段是调试Shadow Mapping用的, 就是把生成的阴影贴图贴到了一个矩形上面
        DebugShader.SetFloat("near_plane", 1.0f);
        DebugShader.SetFloat("far_plane",  7.5f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderQuad();
        DebugShader.deactivate();*/


        ImGui :: Render(); // 渲染ImGui, 不能放在绘制场景之前, 而应该和场景一起绘制, 否则绘制场景时glClear会把窗口清掉
        ImGui_ImplOpenGL3_RenderDrawData(ImGui :: GetDrawData());

        // 检测事件,交换屏幕缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();//调用imgui的析构函数,终止imgui
    ImGui_ImplGlfw_Shutdown();

    glfwTerminate();
    return 0;
}
