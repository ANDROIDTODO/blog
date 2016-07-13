#Android开发中的问题总结

###1.Fragment切换时导致的页面不流畅问题

###2. android手机录不了音的情况：
1、android判断是否禁用了录音权限

      通过判断分贝值是否大于0，在代码中搜索 volume > 0

2、部分手机在调用 audioRecord.startRecording(); 和audioRecord.stop();方法时会抛出异常，例如联想手机

3、部分手机（例如联想）会抛出java.lang.IllegalStateException: startRecording() called on an uninitialized AudioRecord.

       原因：出现上面问题的原因是录音的硬件资源被申请了，但是没有释放，然后你再次申请资源，导致初始化失败。这里需要注意的是不仅仅需要调用Release()方法。还需要把AudioRecord对象置为null，否则还是释放失败。下面是Android 开发网上面的一个对于AudioRecord的释放说明。

Releases the native AudioRecord resources. The object can no longer be used and the reference should be set to null after a call to release()。

###3.android中应该用线程池来管理线程
###4. RxJava架构
###5. EventBus
###6. Retrofit services
###7. HotPatch
###8. 动态改变横竖屏


[我的博客](http://blog.csdn.net/guodongxiaren "悬停显示")
