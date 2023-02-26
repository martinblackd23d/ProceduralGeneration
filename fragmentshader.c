#version 460 core

out vec4 FragColor;

//layout(pixel_center_integer) in vec4 gl_FragCoord;
in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;

//uniform int data[];

//uniform int xImage;
//uniform int yImage;

//uniform int width;
//uniform int height;

void main() {
	//float xNormalized = gl_FragCoord.x / width;
	//float yNormalized = gl_FragCoord.y / height;
	FragColor = texture(texture1, TexCoord);
	//FragColor = vec4(xNormalized, yNormalized, 1-(xNormalized+yNormalized)/2, 1.0f);
}