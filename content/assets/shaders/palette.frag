uniform sampler2D texture;
uniform sampler2D colorTableTexture;
uniform float colorTableWidth;
uniform float colorTableHeight;
uniform float paletteIndex;

void main()
{
    vec4 sourcePixel = texture2D(texture, gl_TexCoord[0].xy);

    // Preserve transparent source pixels
    if(sourcePixel.a == 0.0) {
        discard;
    }

    // Apply color mapping to opaque pixels.
    //
    // The source pixel RGB values are are normalized between 0.0 and 1.0,
    // so in order to get the correct "index" you must multiply the RGB value
    // by 256.0, and then scale it back down to a value normalized by the width
    // of the color table.
    
    vec2 colorIndex = vec2((sourcePixel.r * 256.0) / colorTableWidth, paletteIndex / colorTableHeight);
    vec4 mappedPixel = texture2D(colorTableTexture, colorIndex);
    gl_FragColor = mappedPixel;
}
