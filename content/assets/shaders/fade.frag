
uniform sampler2D texture;
uniform float fadePercent;

const vec4 level0 = vec4(0x5F, 0x5F, 0x5F, 0xFF) / 255.0;
const vec4 level1 = vec4(0x2F, 0x2F, 0x2F, 0xFF) / 255.0;
const vec4 level2 = vec4(0x1F, 0x1F, 0x1F, 0xFF) / 255.0;
const vec4 level3 = vec4(0x0F, 0x0F, 0x0F, 0xFF) / 255.0;

void main()
{
    vec4 sourcePixel = texture2D(texture, gl_TexCoord[0].xy);

    // Do tinting/fading based on elapsed time..
    vec4 tintedPixel = sourcePixel;

    if(fadePercent <= 0.0) {
        tintedPixel = sourcePixel;
    } else if(fadePercent < 25.0) {
        tintedPixel = sourcePixel * level0;
    } else if(fadePercent < 50.0) {
        tintedPixel = sourcePixel * level1;
    } else if(fadePercent < 75.0) {
        tintedPixel = sourcePixel * level2;
    } else {
        tintedPixel = sourcePixel * level3;
    }

    gl_FragColor = tintedPixel;
}
