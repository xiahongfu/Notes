# Unity的VideoPlayer
![alt text](images/image-12.png)


# Cocos Creater的VideoPlayer
![alt text](images/image-11.png)
![alt text](images/image-10.png)

# 对比

1. Cocos Creater VideoPlayer组件只能和UI Canvas配合使用。经过试验在非Canvas上搭载VideoPlayer组件只能听到声音，没有视频画面。相对Unity来说没那么灵活。
2. Unity直接支持将视频流渲染到RenderTexture上。Cocos Creater只能通过在UI画布下新建一个Camera组件对着UI画布摄像，然后渲染到RenderTexture上。
3. Unity支持渲染到摄像机近平面/远平面上，可以方便地实现背景视频、前景视频。Cocos Creater默认是前景视频，背景视频只支持Stay On Bottom，需要配合将UICanvas设置成透明的，否则就会只有声音，看不到视频。
4. Unity的RenderMode支持Material Override（视频流渲染到材质上）和API Only（视频流通过API进行操作，实现对视频流的深度控制和编辑等）。Cocos Creater都不支持
5. Unity支持视频的音频输出到Audio Source上实现音视频分离和3D效果等（还支持APIOnly，但是是实验性质的）。CocosCreater都不支持，只支持静音/播放/调节音量
6. Unity VideoPlayer的UpdateMode支持设置时钟源，支持视频与音频同步，视频与游戏时间同步。
7. Unity 的Skip On Drap用来设置：当视频的播放位置和游戏时钟之间产生漂移时，是跳过某些帧而保持和游戏的时钟同步，还是系统的播放所有帧。
8. Unity可以对音轨单独设置Enable。Cocos Creater不支持。