# ts侧字符串多一个\0
**表现**
在游戏客户端Cs call TS的时候，如果TS侧返回字符串会发现字符串末尾多了个\0。
**原因分析**
native侧使用`char* buf`接收TS返回的字符串。返回的时候应该返回bufsize-1，但是native侧错误地返回了bufsize。
**解决方法**
OpenHarmonyJSBindingsHelpers.h 的 GetValueStringUTF8函数 第337行返回值那里改成  ret = scripting_string_new(buff, retSize)。把actualSize改为retSize

# InputField UI不更新问题
**表现**
在输入框中输入的时候，光标位置存在异常。

**原因分析**
inputField输入框光标位置有误导致数组下标越界，导致InputField UI不更新。表现为每次输入删除键之后UI就不更新，但是Text是对的。

**解决方法**
首先要参考团结的，把SetInputSelection事件给注册进去。其次就是EntryPoint.cpp的nativeSetInputSelection(napi_env env, napi_callback_info info)函数写的有问题，debug发现status不为napi_ok。
改为如下形式
exten "C" napi_value nativeSetInputSelection(napi_env, env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {nullptr, nullptr};
    int32_t start = 0;
    int32_t length = 0;
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    napi_status status = napi_get_value_int32(env, args[0], &start);
    napi_status status2 = napi_get_value_int32(env, args[1], &length);
    if (status != napi_ok || status2 != napi_ok)
    {
        return nullptr;
    }
    SetInputSelection(start, length);
    return nullptr;
}

# UnityJSRegister.ets注册不成功
**解决方法**
CopyHapResources.cs里，filesToNotOverwrite里需要加上UnityJSScriptRegister.ets。否则对UnityJSRegister.ets的修改会被覆盖。
