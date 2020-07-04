#version 430
out vec4 FragColor;
in vec2 texCoord;
uniform layout(binding=2, rgba32f) image2D outputImage;

void main()
{
    ivec2 size = imageSize(outputImage);
    ivec2 image2DCoord = ivec2(texCoord.x * size.x, size.y - texCoord.y * size.y);
    FragColor = imageLoad(outputImage, image2DCoord);
}
