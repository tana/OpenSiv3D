#version 300 es
		
precision mediump float;

//layout(location = 0) in vec4 Color;
in vec4 Color;
		
layout(location = 0) out vec4 FragColor;
		
void main()
{
	FragColor = Color;
}
