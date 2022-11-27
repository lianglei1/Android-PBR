#version 300 es

in vec2 TexCoords;
out float saoBlurOutput;

uniform sampler2D saoInput;
uniform int saoBlurSize;


void main()
{
   vec2 texelSize = 1.0 / vec2(textureSize(saoInput, 0));
   float result = 0.0;

   for (int x = 0; x < saoBlurSize; ++x)
   {
      for (int y = 0; y < saoBlurSize; ++y)
      {
         vec2 offset = (vec2(-2.0) + vec2(float(x), float(y))) * texelSize;
         result += texture(saoInput, TexCoords + offset).r;
      }
   }

   saoBlurOutput = result / float(saoBlurSize * saoBlurSize);
}
