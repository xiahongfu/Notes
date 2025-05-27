UUtuPlugin::Import
    FUtuPluginCurrentImport::Import
        FUtuPluginCurrentImport::BeginImport   做一些前置工作（保存信息等）
        while
            FUtuPluginCurrentImport::ContinueImport
            ---ContinueImport完成后会调用FUtuPluginCurrentImport::CompleteImport，这里才是真正的exodus导入。




FUtuPluginCurrentImport::CompleteImport
    JsonImporter::importProject
        设置资源路径
        jsonData = 从JsonProject导入信息
        调用JsonImporter::importResources执行资源导入流程。（Texture、CubeMap、Materials、Meshes）
        for scene in scenes: 遍历所有场景
            curSceneData = 从场景json中导入信息
            JsonScene scene(curSceneData);  // 解析curSceneData到Scene
            调用importScene导入场景


JsonImporter::importResources
    JsonImporter::loadMeshes
        for filename in meshes  遍历mesh的json
            JsonObjPtr = loadExternResourceFromFile(filename) // 一个通用的描述JsonObject的指针
            JsonImporter::importMesh(obj, curId);
                JsonMesh jsonMesh(obj);    // 创建JsonMesh描述函数
                JsonImporter::importMesh



适配计划
1. JsonImporter.h需要加一个loadVideoPlayer
2. JsonImporter::importResources需要调用loadVideoPlayer。loadVideoPlayer从JsonsManager取数据。
3. 需要加一个JsonVideoPlayer、JsonVideoClip和JSONRenderTexture。        完成
4. JsonGameObject需要加一个字段components，适配JsonGameObject::load

5. Unity侧AAAIDPathMap.json里需要按tag分类，每个tag都是一个list。写一个JsonsManager。只保存Scene、Component、Assets。不保存静态资源。   完成
6. UE侧写一个对应的JsonsManager，读入AAAIDPathMap.json，按tag写一个`unordered_map<tag, vector<pair<id, path>>>`。    完成
7. 在JsonImporter::importResources调用前先把所有静态资源一把导入进去。