#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include <iostream>

#include "Geometry.h"

constexpr double MY_PI = 3.141592653589;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;

float deltaTime = 0.0f;
float lastTime = 0.0f;

Eigen :: Vector3f CameraPos(0.0f,0.0f,0.0f); //相机位置
Eigen :: Vector3f CameraDir(0.0f,0.0f,-1.0f); //注视目标
Eigen :: Vector3f Up(0.0f,1.0f,0.0f); //世界坐标的上方

void framebuffer_size_callback(GLFWwindow* window,int width,int height){//用来在用户手动调整窗口大小时回调,重新调整视口大小和窗口大小,保证物体不会变形
    /*更新screen width和screen height*/
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    /*更新视口*/
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){//如果在window窗口检测到esc被按下
        glfwSetWindowShouldClose(window,true);// 设置window应该被关闭
    }
    float CameraSpeed = 2.5f * deltaTime; //利用两帧之间的间隔时间来平衡移动速度
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){ //拉近
        CameraPos += CameraSpeed * CameraDir;
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){ //远离
        CameraPos -= CameraSpeed * CameraDir;
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){ //左移
        CameraPos -= (CameraDir.cross(Up)).normalized() * CameraSpeed;
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){ //右移
        CameraPos += (CameraDir.cross(Up)).normalized() * CameraSpeed;
    }
}

void update(){

}

void render(){
    glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"GLYT",NULL,NULL);//创建一个GLFW窗口对象,它的指针放在 window 中
    if(window == NULL){
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

    while(!glfwWindowShouldClose(window)){ //"游戏循环"
        processInput(window);
        update();
        render();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
