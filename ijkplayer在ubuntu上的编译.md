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

```
官方给出 /.bash_profile or ~/.profile中设置环境，我一般为了简单就直接用profile

用vim打开bashrc 文件
#>sudo vim ~./bashrc
在最后面添加：
```
export ANDROID_SDK=/home/ijk/Desktop/ijkSupport/tools
export ANDROID_NDK=/home/ijk/Desktop/ijkSupport/android-ndk-r14b
```

esc保存退出

用source命令让刚才添加的路径立即生效

#>source ~./bashrc

### 5.编译ijkplayer so库
```
cd ijkplayer-android
git checkout -B latest k0.7.9

./init-android.sh

cd android/contrib
./compile-ffmpeg.sh clean
./compile-ffmpeg.sh all

cd ..
./compile-ijk.sh all
```








