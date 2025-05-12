# 属性
* playbackSpeed：控制视频播放速度。可以在视频播放过程中修改。`playbackSpeed += 0.1;`
* time：当前视频播放的位置，单位是秒。可以在视频播放过程中直接修改。`time += 2.0`
* isPlaying：获取是否正在播放。
* length：当前正在播放的视频的长度。
* .clip.length：clip视频的长度。
* loopPointReached：

# 方法
* SetDirectAudioVolume()
* Pause()
* Play()
* Pause()
* Stop()

# loopPointReached
需要在UE的组件侧调用`InvokeLoopPointReachedCallback_Internal`
```cs
// VideoPlayer.cs
namespace UnityEngine.Video
{
  public sealed class VideoPlayer : Behaviour
  {
    public event VideoPlayer.EventHandler loopPointReached;
    public delegate void EventHandler(VideoPlayer source);

    [RequiredByNativeCode]
    private static void InvokeLoopPointReachedCallback_Internal(VideoPlayer source)
    {
      if (source.loopPointReached == null)
        return;
      source.loopPointReached(source);
    }
  }
}
```
