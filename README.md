# XGL
Interesting

OpenGL Step By Step

学的很用心但好像开始要忘了..........

1. 立即模式 glbegin(traget); glvertex3f()...glcolor3f()... glend();
2. 矩阵栈 当前矩阵入栈 glpushmatrix(); 变换矩阵入栈RST ; glpopmatrix()退栈，恢复；
	 显示列表glgenlist glnewlist draw... glendlist while{ glcalllist;}
	 绘制方式：
   非VBO ： glEnableClientState glVertexPointer glColorPointer  draw ... glDisableClientState;
   VBO glEnableClientState glBindBuffer glVertexPointer glColorPointer draw.. glDisableClientState
   VBO 	
   	glEnableVertexAttribArray(0);
		glBindBuffer( GL_ARRAY_BUFFER,vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(vertexAttribs), 0);
		glDrawArrays(GL_POINTS, 0, 1);
		glDisableVertexAttribArray(0);
3. 	glMatrixMode(GL_PROJECTION);	glMatrixMode(GL_MODELVIEW);
		OPENGL 数组是列主序  c++数组是行主序。 从c++传递至OPENGL需要转置
		
4. 固定管线光栅化颜色插值
5. 简单shader和相机camera矩阵计算
	//-------------------------
	XGL::Vec3f Target(0.0f, 0.0f, 0.0f);
	XGL::Vec3f Eposition(2.0f, 1.0f, 0.0f);
	XGL::Vec3f Direction = Eposition - Target;

	XGL::Vec3f right, up, forward;
	forward = Direction;
	up = Vec3f(0.0f, 1.0f, 0.0f);

	right = up ^ forward;
	right.normalize();

	up = forward ^ right;

	//c++ 数组行主序， 是矩阵的转置形式
	float lightView[4][4] = {
		right.x(), up.x(), forward.x(),0,
		right.y(), up.y(), forward.y(),0,
		right.z(), up.z(), forward.z(),0,
		-Eposition * right , -Eposition * up, -Eposition * forward,1,
	};
6. 2d纹理贴图 xyz绑定uv 左下角uv(0,0) ,右上角uv(1,1) 逆时针
7. 全局环境光
8. 平行光源散射光 dot(-L ,N)
9. 平行光源镜面光 dot(reflect(L,N), E)
10. 点光源 位置+ 无方向+ 衰减 L = eye - lightposition
11.	聚光灯 光椎体+ 方向
12. Assimp
13. 绑定GL_DRAW_FRAMEBUFFER渲染深度至纹理，激活默认帧缓存，绑定目标类型为GL_FRAMEBUFFER
14. 构建光源camera矩阵,渲染模型深度至纹理，激活默认帧缓存，光源为相机手动计算深度，阴影着色
			a. 光源camera计算clip，透视除法，ndc转纹理坐标，采样屏幕大小的深度纹理，比较深度，若纹理深度小，则该像素位于阴影中
			
			
15. 法线着色渲染球体 tnormal = transpose(inverse(g_mv)) * vec4(normal, 0.0);

16. 

17. 天空盒texture_cube_map, 开启深度测试，修改gl_position.xyzw = gl_positon.xyww; z = 1
		始终位于远裁剪面
		
18. 法线贴图，切线空间计算TBN 法线纹理的RGB值存放的是TBN切线空间的向量RGB
		世界坐标系下的NORMAL = TBN *RGB
		推导： https://blog.csdn.net/jxw167/article/details/58671953

19. 布告板技术
		几何shader
		vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(g_eye - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up) 

20. 粒子系统
		feedback
		前一帧渲染输出，作为下一帧渲染的输入
		
21. Color picking
			层次结构： 
			场景模型ID[
				模型EntityID[
					几何片元ID[]		
				]
			]
						vindex = uvec3(g_objectindex, g_drawcallindex, gl_PrimitiveID + 1);
						
			离屏渲染：
				glGenRenderbuffers(1, &m_colorbuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_colorbuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB32UI, windowWith, windowHeight);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorbuffer);

22. Triangles曲面细分
		顶点到视点的距离计算细分因子
		
23. quads/isolines 曲面细分

24. N-Patches曲面细分

25. VAO
		保存vbo/ibo状态，渲染回调只用激活VAO
		
26. 实例化渲染

    glVertexAttribDivisor(WORLD_LOCATION + i, 1);
		从属性数组中每隔divisor个实例都会读取一个新值
		glVertexAttribDivisor(WORLD_LOCATION + i, 3);
		
		layout(location = 3) in mat4 world;
		GLSL顶点属性最大只支持16个元素: mat4
		
27. GBUFFER
		渲染顶点属性至纹理
		
28. 延迟渲染
				点光源球渲染、平行光屏幕四边形渲染，片段shader计算光照，融合


29. 延迟渲染
		解决位于点光源球体后的模型也被光照的错误/ 进入到点光源球后光照效果丢失的错误
		1. 几何阶段GBUFFER
		2. 模板测试阶段
		3. 光照阶段
		4. 平行光阶段
		5. 融混
		
30. 边界检测
		共边的两个三角形的法向量是否一致
		
31. 阴影纹理

32. PCF
		采样据此优化

33. 点光源阴影

34. cubemap

35. SSAO与深度重构
		深度纹理与透视矩阵，应用屏幕渲染技术，计算View坐标
		位于角落处接受的光照更少: 随机点的真实z值和顶点的z比较
		
						
