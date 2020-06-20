#version 430
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D texture0;
uniform int texture0Height;

uniform layout(rgba8) image2D thresholdTexture;

//vec3 rgb2hsv(vec3 rgb)
//{
//   vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
//   vec4 p = rgb.g < rgb.b ? vec4(rgb.bg, K.wz) : vec4(rgb.gb, K.xy);
//   vec4 q = rgb.r < p.x ? vec4(p.xyw, rgb.r) : vec4(rgb.r, p.yzx);
//   float d = q.x - min(q.w, q.y);
//   float e = 1.0e-10;
//   return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
//}
//vec3 hsv2rgb(vec3 hsv)
//{
//   vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//   vec3 p = abs(fract(hsv.xxx + K.xyz) * 6.0 - K.www);
//   return vec3(hsv.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), hsv.y));
//}
void main()
{
//   vec4 bgr = texture(texture0, vec2(texCoord.x, texture0Height - texCoord.y));
//   vec3 rgb = bgr.zyx;
//   vec3 hsv = rgb2hsv(rgb).xyz;
//   FragColor = vec4(hsv2rgb(hsv).xyz, 1.0);
    FragColor = imageLoad(thresholdTexture, ivec2(texCoord.x, texCoord.y));
}
