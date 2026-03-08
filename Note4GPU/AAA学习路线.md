# 路线
学习CUDA C++ 编程，学习CUDA算子开发流程，[CUDA编程--编程模型简介&算子开发流程](https://blog.csdn.net/xzpdxz/article/details/134336869)需要跟着从头到尾做一遍。从算子到pytorch使用需要摸一遍。


手势识别软件端到端实现：
1. pytorch训练一个模型（或者找一个已有的模型）跑起来，复现HRNet吧，能跑能训，能测。
2. 自定义CUDA算子，编成so。在pytroch里用起来。
3. 模型打包部署，在自己的电脑上能跑起来，重点是能给面试官演示。


# 总结的经验贴
手撕LRU缓存、最小栈
CUDA Graphs是啥？

【CUDA编程--编程模型简介&算子开发流程】https://blog.csdn.net/xzpdxz/article/details/134336869
CUDA算子手撕与面试指南: https://zhuanlan.zhihu.com/p/12661298743


集锦：https://xinsheng.huawei.com/next/detail/#/index?uuid=1217914605093912576


Agent相关：https://xinsheng.huawei.com/next/detail/#/index?uuid=1196973863647133696


还没开始：
一些ai infra知识整理：https://xinsheng.huawei.com/next/detail/#/index?uuid=1184940239305371648
一些ai infra QA整理：https://xinsheng.huawei.com/next/detail/#/index?uuid=1186094292076953600
每月第一周发布团队成员推荐的优质知识-202510：https://3ms.huawei.com/km/blogs/details/21860033?l=zh-cn
大模型面试问题准备：https://xinsheng.huawei.com/next/detail/#/index?uuid=1211478696500727808
发点面筋：https://xinsheng.huawei.com/next/detail/#/index?uuid=1224049854211432448
AI Infra/大模型算法工程 学习笔记 （第一篇）：https://xinsheng.huawei.com/next/detail/#/index?uuid=1169069801454620672
最全大模型学习路径：https://3ms.huawei.com/km/blogs/details/21482007
算子面经：https://xinsheng.huawei.com/next/detail/#/index?uuid=1209278224175779840
算子面经：https://xinsheng.huawei.com/next/detail/#/index?uuid=1207026957357527040
面经：https://xinsheng.huawei.com/next/detail/#/index?uuid=1196320390718959616
王总面经：https://xinsheng.huawei.com/next/detail/#/index?uuid=1227357704853708800
SIMD向量化实战：Embedding算子性能提升1.5倍：https://xinsheng.huawei.com/next/detail/#/index?uuid=1229281965583400960



太难了：
AI infra学习材料总结：https://xinsheng.huawei.com/next/detail/#/index?uuid=1169724997839052800
昇腾集合通信HCCL基本软硬件原理：https://xinsheng.huawei.com/next/detail/#/index?uuid=1201615192939347968



已完成：

GPU架构：https://blog.csdn.net/asasasaababab/article/details/80447254
GPU=n*SM+Cache
SM=n*GPUCore + m*DPU    DPU是双精度运算单元，GPUCore是单精度的

# 八股总结

1. 你说你用到了Milvus，请讲一下Milvus的底层实现
2. RAG搜索的索引是怎么构建的， 索引构建原理， 向量检索时间复杂度，向量检索怎么加速，
3. 幻觉是什么，幻觉原因
4. 你的Agent有哪些组件， Agent怎么实现的，为什么不用多Agent
5.  你的Agent各个组件是怎么设计的
6. 你的决策部分用到了ReAct，说一下怎么实现的
7. ReAct 耗时问题，怎么终止退出思考， 怎么避免思考死循环
8. Agent记忆的消息结构是怎么设计的
9. 那你的提示词怎么放下所有项目需要的内容。（因为确实东西很少，直说了因为选择模型的问题上下文窗口本来是够的，感觉他不是很满意，确实应该说一些优化的点。）
10. 怎么保证你生成的东西（项目相关）的生成正确性，生成可执行，链路完整性，。 
11. 你觉得你们项目后续还可以做哪些内容
12. llm边界怎么处理
13. 为什么选择这个embedding模型
14. 用户量多少，badcase怎么优化
15. 知识库怎么更新除了定时更新你觉得还有什么好方法
16. 说说你觉得最有效果的prompt优化方法。 
17. 了解workflow编排的平台如Coze的底层引擎吗


https://zhuanlan.zhihu.com/p/678602674
https://www.nowcoder.com/feed/main/detail/57033fe8898b4e85b97c48d8b2dcfb28
https://zhuanlan.zhihu.com/p/12661298743
