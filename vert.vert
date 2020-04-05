attribute vec2 a_Vertex;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main(void)
{
    gl_Position = vec4(a_Vertex,0.0f,1.0f);
}
