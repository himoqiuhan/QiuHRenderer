# QiuHRenderer
撸一个软光栅渲染器



## devLog

### 20230426-成功封装Render Pilpline

把Render Pipeline封装在了Rasterrizer中的ExeRenderPileline中，架构同最基本的渲染流水线架构（没有加Clipping）：

```伪代码
//渲染架构：
	//传入模型、读取光照信息
	//--for(每个面)
	//		定义v2f结构体数组
	//		--for(3个顶点)
	//			GetVertexData获取当前顶点appdate_base顶点信息
	//			VertexShader给v2f赋值
	//		----
	//		使用v2f信息:
	//		GetAABB获取BoudingBox
	//		--for(BoundingBox)
	//			GetBarycentric获取重心坐标
	//			-if(在三角形内部)
	//				FragmentShader输出片元颜色color4
	//				DrawPixel
	//			-else
	//				continue
	//		----
```

> Debug实记：
>
> 在第一遍写出Render Pipeline的时候，模型不能正确显示，使用和原本代码同样的摄像机配置和变换
>
> 然后就开始了暴力Debug（添加各种宏定义，来定义输出各个渲染部分的输出：
>
> - #define DEBUG_SINGLEFRAME ： 只渲染一帧
> - #define LEGACY ： 看看执行原本的代码得到的各个信息长什么样子（后续版本这一段会被我删掉）
> - #define DEBUG_VERT ： 监测顶点着色器执行后顶点的世界空间坐标、屏幕空间坐标和透视系数如何
> - #define DEBUG_DATASTREAMING ： 查看信息流送状态（此时Debug显示为v2f内的信息）
>
> 当我逐个内容和原本代码对比之后，发现空间变换都是没问题的，这个时候想到了问题可能出在屏幕映射（从[-1 ,1]映射到[screen.width, screen.height]）。然后好好看了看，果然，v2f传输给后续阶段的屏幕空间坐标是Candidate Cube中的坐标，没有映射到屏幕（窗口）的大小上，导致传递给**构建BoundingBox**的函数和**获取片元重心坐标**的函数接收到的ScreenTriangle参数不正确。
>
> 后续便在VertexShader之后加入了ScreenMapping函数，让ScreenTriangle的x、y处于屏幕（窗口）的空间中
