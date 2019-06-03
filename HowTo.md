# 简易指导
进入/path/to/GOSP/code目录，能找到configure这个可执行文件。  
export GIVEDA_DIR=/path/to/GOSP/code   #设置环境变量  
./configure  #执行configure  
make           #执行make  
#编译成功后，进入example目录，执行runExample.sh  
cd example   #进入example目录  
./runExample.sh   #在example目录下执行runExample.sh  
使用按键进行操作（常用的按键包括：上下左右方向键（用于移动焦点）、回车用于选中、Esc用于返回和退出）  


# FAQ
#### 编译过程中常见问题的解决：   
如果提示找不到头文件或库文件，请安装依赖。本软件依赖libjpeg/libpng/freetype等。  
    sudo apt-get install libjpeg-dev libpng-dev   

执行qmake时，如果提示Project ERROR: Package Giveda not found，就将lib/Giveda.pc复制到/usr/lib/pkgconfig/下。  
