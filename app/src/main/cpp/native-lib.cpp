#include <jni.h>
#include <string>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

//sockaddr_un
#include <sys/un.h>

//htons,sockaddr_in
#include <netinet/in.h>
//inet_ntop
#include <arpa/inet.h>
//close,unlink
//offsetof
#include <stddef.h>

#include <android/log.h>
#include <errno.h>

#define LOG_TAG "show infomation"

#define LOGW(a)  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)


void child_do_work();

int child_create_channel();

void child_listen_msg();


//---------------------
int m_child;

//定义一个文件 套接字
const char *PATH = "data/data/com.yifei.ndktest/my.sock";


void child_do_work() {
    //开启socket 服务端
    if (child_create_channel()) {
        child_listen_msg();
    }
}


//创建socktet 函数 服务端读取信息
int child_create_channel() {
    //创建一个socket
    int lisentd = socket(AF_LOCAL, SOCK_STREAM,
                         0);  // int  socket(int protofamily, int type, int protocol);//返回sockfd

    // addr 指向的内存区
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(sockaddr_un)); //清空内存区域
    addr.sun_family = AF_LOCAL;
    stpcpy(addr.sun_path, PATH);
    int confd = 0;
    //绑定一个socket
    int bindId = bind(lisentd, reinterpret_cast<const sockaddr *>(&addr), sizeof(sockaddr_un));
    if (bindId < 0) {
        LOGW("绑定错误");
        return 0;
    }
    listen(lisentd, 5); //最大监听 多少客户端
    //保证 宿主进程连接成功
    while (1) {
        //返回客户端的地址 阻塞 函数
        if ((confd = accept(lisentd, NULL, NULL)) < 0) {
            //连接失败
            if (errno == EINTR) {
                continue;
            } else {
                LOGW("读取错误");
                return 0;
            }
        }

        //连接成功
        m_child = confd;
        break;

    }
    return 1;
}

// 创建服务端的socket socket()函数

//读
void child_listen_msg() {
    fd_set rfds;
    struct timeval timeout ={3,0};
    while (1) {

        // 清空内容
        FD_ZERO(&rfds);
        FD_SET(m_child, &rfds);
        //选择监听
       int r= select(m_child+1,&rfds,NULL,NULL,&timeout);
        LOGW("读取消息 %d");
        if(r>0){
            //缓冲区
            char pkg[256]={0};
            //保证所读到的信息是客户端的
            if(FD_ISSET(m_child,&rfds)){
                //阻塞式函数 什么都不读
                int result=read(m_child,pkg, sizeof(pkg));
                execlp("am","am","startservice","");
            }
        }

    }


}


extern "C"
JNIEXPORT void JNICALL
Java_com_yifei_ndktest_Watcher_createWetcher(JNIEnv *env, jobject instance, jstring userId_) {
    const char *userId = env->GetStringUTFChars(userId_, 0);
    //开启双进程 fork后一个函数会有两个返回值
    pid_t pid = fork();

    if (pid < 0) { //fork失败

    } else if (pid == 0) {//子进程
        //守护进程 守护服务
        child_do_work();

    } else if (pid > 0) { //父进程

    }


    env->ReleaseStringUTFChars(userId_, userId);
}

