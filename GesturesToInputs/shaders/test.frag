#version 430
out vec4 FragColor;
in vec2 texCoord;
uniform layout(binding=2, rgba32f) image2D outputImage;

void main()
{
    uvec2 size = imageSize(outputImage);
    ivec2 coord = ivec2(texCoord.x * size.x, size.y - texCoord.y * size.y);

    uint oneFifth = size.x / 5;
    uint oneThird = size.y / 3;

    bool shouldBeBlue = coord.y == oneThird || coord.y == oneThird * 2 ||
        coord.x == oneFifth || coord.x == oneFifth * 2 || coord.x == oneFifth * 3 || coord.x == oneFifth * 4;

    FragColor = shouldBeBlue ? vec4(0,0,1,1) : imageLoad(outputImage, coord);
}
