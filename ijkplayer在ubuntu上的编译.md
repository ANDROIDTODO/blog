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
下载完成之后 我们需要解压缩文件
#>unzip <文件名>
### 4.配置Android ndk，Android sdk环境，一般我们需要vim （sudo apt-get install vim）
```
# add these lines to your ~/.bash_profile or ~/.profile
# export ANDROID_SDK=< your sdk path >
# export ANDROID_NDK=< your ndk path >
```
官方给出 /.bash_profile or ~/.profile中设置环境，我一般为了简单就直接用profile

用vim打开profile 文件
#>vim ~./profile
在最后面添加：
```
export ANDROID_HOME=/home/ijk/Desktop/ijkSupport
export ANDROID_SDK=$ANDROID_HOME/tools
export ANDROID_NDK=$ANDROID_HOME/android-ndk-r14b
```
sdk和ndk都在桌面的ijkSupport文件夹中








