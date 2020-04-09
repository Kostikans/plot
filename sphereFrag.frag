
void main(void)
{
    vec3 color1 = vec3(1.9f,0.55f,0.0f);
    vec3 color2 = vec3(0.226f,0.0f,0.615f);
    float mixValue = distance(gl_FragCoord.xy,vec2(0,1));
    vec3 color = mix(color1,color2,mixValue);
    gl_FragColor = vec4(gl_FragCoord.xy/vec2(1000.0f,1000.0f),0.3f,1.0f);
}
