
uniform sampler2D texture;
uniform float fadePercent;

const vec4 level0 = vec4(0x5F, 0x5F, 0x5F, 0xFF) / 255.0;
const vec4 level1 = vec4(0x2F, 0x2F, 0x2F, 0xFF) / 255.0;
const vec4 level2 = vec4(0x1F, 0x1F, 0x1F, 0xFF) / 255.0;
const vec4 level3 = vec4(0x0F, 0x0F, 0x0F, 0xFF) / 255.0;

int bitwiseAND(int a, int b) {
    // AND (&)
    int c = 0;

    for (int x = 0; x < 32; ++x) {
        c += c;

        if (a < 0) {
            if (b < 0) {
                c += 1;
            }
        }

        a += a;
        b += b;
    }

    return c;
}

float interpolateHEX(float inputValue, float hexValue) {
    return float(
        bitwiseAND(
            int(inputValue * 255.0),
            int(hexValue)
        )
    ) / 255.0;
}

void main()
{
    vec4 sourcePixel = texture2D(texture, gl_TexCoord[0].xy);

    // Do tinting/fading based on elapsed time..
    vec4 tintedPixel = sourcePixel;

    if(fadePercent < 25.0) {
        tintedPixel = sourcePixel * level0;
        // tintedPixel.r = interpolateHEX(tintedPixel.r, level0.r); // (float(int(tintedPixel.r * 255.0) & int(level0.r)) / 255.0);
        // tintedPixel.g = interpolateHEX(tintedPixel.g, level0.g); // (float(int(tintedPixel.g * 255.0) & int(level0.g)) / 255.0);
        // tintedPixel.b = interpolateHEX(tintedPixel.b, level0.b); // (float(int(tintedPixel.b * 255.0) & int(level0.b)) / 255.0);
    } else if(fadePercent < 50.0) {
        tintedPixel = sourcePixel * level1;
        // tintedPixel.r = interpolateHEX(tintedPixel.r, level1.r); // (float(int(tintedPixel.r * 255.0) & int(level1.r)) / 255.0);
        // tintedPixel.g = interpolateHEX(tintedPixel.g, level1.g); // (float(int(tintedPixel.g * 255.0) & int(level1.g)) / 255.0);
        // tintedPixel.b = interpolateHEX(tintedPixel.b, level1.b); // (float(int(tintedPixel.b * 255.0) & int(level1.b)) / 255.0);
    } else if(fadePercent < 75.0) {
        tintedPixel = sourcePixel * level2;
        // tintedPixel.r = interpolateHEX(tintedPixel.r, level2.r); // (float(int(tintedPixel.r * 255.0) & int(level2.r)) / 255.0);
        // tintedPixel.g = interpolateHEX(tintedPixel.g, level2.g); // (float(int(tintedPixel.g * 255.0) & int(level2.g)) / 255.0);
        // tintedPixel.b = interpolateHEX(tintedPixel.b, level2.b); // (float(int(tintedPixel.b * 255.0) & int(level2.b)) / 255.0);
    } else if(fadePercent < 100.0) {
        tintedPixel = sourcePixel * level3;
        // tintedPixel.r = interpolateHEX(tintedPixel.r, level3.r); // (float(int(tintedPixel.r * 255.0) & int(level3.r)) / 255.0);
        // tintedPixel.g = interpolateHEX(tintedPixel.g, level3.g); // (float(int(tintedPixel.g * 255.0) & int(level3.g)) / 255.0);
        // tintedPixel.b = interpolateHEX(tintedPixel.b, level3.b); // (float(int(tintedPixel.b * 255.0) & int(level3.b)) / 255.0);
    }

    // tintedPixel.a = fadePercent / 100.0;

    gl_FragColor = tintedPixel;
}
