背景：安卓侧有这样一个功能，就是可以通过PlayerSettings界面的单选框将配置文件加到Plugins/Android目录下。具体表现就是，勾上单选框，会在Assets/Plugins/Android里加上对应的配置文件，然后手动修改这个配置文件最后出包的时候使用的就是我们修改后的版本。Android侧还提供占位符功能，鸿蒙侧暂不提供这个功能。

# 修改UI
前两步用于构造SerializedProperty，SerializedProperty是序列化的属性。Unity里除了有Editor/Src/EditorOnlyPlayerSettings.h，还有Runtime/Misc/PlayerSettings.h。这里只涉及到Editor，因此只需要修改Editor下面的EditorOnlyPlayerSettings即可。

1. 在EditorOnlyPlayerSettings.h 的 EditorOnlyPlayerSettings结构体中添加字段 bool ohosUseCustomModuleInEntry;
2. 在EditorOnlyPlayerSettings.cpp的Transfer中加上TRANSFER(openHarmonyUseCustomModuleInEntry)。以及在EditorOnlyPlayerSettings::Reset()加上 openHarmonyUseCustemModuleInEntry = false.
3. 在OpenHarmonyPlayerSettingsEditorExtension.cs中绘制UI
```cs
internal class OpenHarmonyPlayerSettingsEditorExtension : Default PayerSettingsEditorExtension
{
    ...
    SerializedProperty m_ohosUseCustomModuleInEntry;
    GUICOntent textCustomModuleInEntry = EditorGUIUtility.TrTextContext("this is lable");
    ...

    public override void OnEnable(PlayerSettingsEditor settingsEditor)
    {
        base.OnEnable(settingsEditor);
        ...
        // 从某个地方读取序列化的Property。
        m_ohosUseCustomModuleInEntry = settingsEditor.FindPropertyAssert("openHarmonyUseCustemModuleInEntry");
        ...
    }

    public override void PublishSectionGUI(flout h, flout kLableFloatMinW, flout kLabelFloatMaxW)
    {
        // 一些API
        EditorGUILayout.PropertyField(m_ohosUseCustomModuleInEntry, textCustomModuleInEntry); // 绘制单选框
        File.Exists(filePath); // 判断文件是否存在
        File.copy(src, dst);
        Directory.Exists(dirPath);
        Directory.CreateDirectory(dirPath);
        AssetDatabase.MoveAsset(from, to); // 用于处理template.json5.DISABLE相关逻辑，复用DISABLE。
        AssetDatabase.Refresh(); // 刷新Asset
    }
}
```
4. 经过前三步，已经可以实现绘制UI、从模板处复制template到Assets/Plugins/OpenHarmony下了。现在添加后处理逻辑，把Plugins中的修改同步到 出包后的包中。
在做的时候还碰到一个问题，就是源文件不能为只读文件，否则复制到Plugins下也无法修改。手动改一下权限即可。
```cs
class CopyCustomPluginTemplate : IPostProcessorTask
{
    public void Execute(PostProcessorContext context)
    {
        var buildDir = context.Get<string>(Contants.BuildDir); // 出包的地址
        var customBasePath = Constants.OpenHarmonyPluginPath; //OpenHrmony的Plugin路径，Assets/Plugins/OpenHarmony
    
        // 准备src和dst，用Paths.Combine(str1, str2, str3 ...)进行拼接就可以了，
        FileUtil.CopyFileIfExists(src, dst, overwrite: true);
    }
}
```
1. 在OpenHarmonyPostProcessor.cs下注册上面的Task
```cs
namespace UnityEditor.OpenHarmony
{
    interface IPostProcessorTask
    {
        string Name {get; }
        void Execute(PostProcessorContext context);
        event ProgreeeHandler OnProgress;
    }

    class PostProcessorContext
    {
        ...
    }

    internal static class PostProcessOpenHarmonyPlayer
    {
        PostProcessorContext _context;
        // 这个函数会被真正的后处理函数调用。
        internal static BuildPropertiesPostProcess(...)
        {
            // 准备context
            ...
            //注册task
            runner.AddNextTask(new CopyCustomPluginTemplates());
            runner.RunAllTasks(_context);
        }
    }
}

```