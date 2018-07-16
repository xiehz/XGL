#version 410 core

layout(vertices = 4) out;

uniform vec3 g_eye;

float getTessLevel(float d1, float d2)
{
	float avgd = (d1+ d2)/2.0f;
	if(avgd <= 3.5f)
		return 20;
	else if(avgd <= 4)
		return 5;
	else if(avgd <= 6.0f)
		return 4;
	else if(avgd <= 8.0f)
		return 3;
	else
		return 1;
}
void main(){
	
	//ÿ�δ���ʱϸ����ɫ�����Է��ʶ�����ɫ����������ж���
	float d0 = distance(g_eye,  (gl_in[0].gl_Position).xyz);
	float d1 = distance(g_eye,  (gl_in[1].gl_Position).xyz);
	float d2 = distance(g_eye,  (gl_in[2].gl_Position).xyz);
	float d3 = distance(g_eye,  (gl_in[3].gl_Position).xyz);

	d0 = (d0+ d3 + d1 + d2)/4.0f;
	d1 = (d0 + d1 + d2 +d3)/4.0f;

	float level0 = getTessLevel(d0, d0);
	float level1 = getTessLevel(d1, d1);

	gl_TessLevelOuter[0] = level0;
	gl_TessLevelOuter[1] = level1;

	//ϸ�ֿ�����ɫ����Patch���Ƶ㲢�д���������gl_InvocationID��Ϊ��������ϸ�ּ��������
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

}

