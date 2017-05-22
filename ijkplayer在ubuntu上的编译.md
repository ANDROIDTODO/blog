纯净Ubuntu 16.04.2

### 1.安装git
#>sudo apt-get install git 回车之后输入密码

### 2.下载ijkplayer源代码  [ijkplayer github地址](https://github.com/Bilibili/ijkplayer)
#>git clone https://github.com/Bilibili/ijkplayer.git ijkplayer-android

### 3.下载Android ndk，Android sdk
* ndk [下载地址](https://dl.google.com/android/repository/android-ndk-r14b-linux-x86_64.zip)
https://dl.google.com/android/repository/android-ndk-r14b-linux-x86_64.zip
* sdk [下载地址](https://dl.google.com/android/repository/sdk-tools-linux-3859397.zip)
https://dl.google.com/android/repository/sdk-tools-linux-3859397.zip

在terminal中通过wget下载sdk和ndk，下载地址默认为当前路径（wget -P 目录地址 网址 eg：wget -P /root https://xxx.xxx.xxx）

### 4.配置Android ndk，Android sdk环境，一般我们需要vim （sudo apt-get install vim）
``
# add these lines to your ~/.bash_profile or ~/.profile
# export ANDROID_SDK=< your sdk path >
# export ANDROID_NDK=< your ndk path >
``



