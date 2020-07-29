
uniform sampler2D diffuse;
varying vec2 texCoordVar;

void main() {
    vec4 color = texture2D(diffuse, texCoordVar);
    //gl_FragColor = vec4(1.0-color.r, 1.0-color.g, 1.0- color.b, color.a);//invertcolor
    
//    float c = (color.r + color.g + color.b)/3.0;
//    gl_FragColor = vec4(c,c,c, color.a);
    
//    vec3 luminance = vec3(dot(vec3(0.2126,0.7152,0.0722), color.rgb));//more realistic grayscale
//    gl_FragColor = vec4(luminance, color.a);
    
    //change saturation
    vec3 luminance = vec3(dot(vec3(0.2126,0.7152,0.0722), color.rgb));//more realistic grayscale
    vec3 m = mix(luminance, color.rgb, 1.0);
    gl_FragColor = vec4(m, color.a);
    
}
