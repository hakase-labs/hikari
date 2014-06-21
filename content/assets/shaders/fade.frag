uniform sampler2D texture;

void main()
{
    vec4 sourcePixel = texture2D(texture, gl_TexCoord[0].xy);

    // Do tinting/fading based on elapsed time..
    vec4 tintedPixel = sourcePixel;
    tintedPixel.r = 1.0;

    gl_FragColor = tintedPixel;
}
