
/*uniform sampler2D tex;

void main (void) {
    
    gl_FragColor = texture2D(tex, gl_TexCoord[0].st) * gl_Color;
    
}*/
uniform sampler2D tex;
uniform float zPosition;

varying vec4 position;
//uniform float blurSize
//varying vec2 gl_TexCoord[0];
float blurSize = ((abs(zPosition-position.z))/600.0)/200.0*8.0; // I've chosen this size because this will result in that every step will be one pixel wide if the RTScene texture is of size 512x512
 
void main(void)
{
 vec4 sum;
 
   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them

   sum += texture2D(tex, vec2(gl_TexCoord[0].x - 4.0*blurSize, gl_TexCoord[0].y)) * 0.05;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x - 3.0*blurSize, gl_TexCoord[0].y)) * 0.09;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x - 2.0*blurSize, gl_TexCoord[0].y)) * 0.12;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x - blurSize, gl_TexCoord[0].y)) * 0.15;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y)) * 0.16;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x + blurSize, gl_TexCoord[0].y)) * 0.15;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x + 2.0*blurSize, gl_TexCoord[0].y)) * 0.12;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x + 3.0*blurSize, gl_TexCoord[0].y)) * 0.09;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x + 4.0*blurSize, gl_TexCoord[0].y)) * 0.05;
 
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - 4.0*blurSize)) * 0.05;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - 3.0*blurSize)) * 0.09;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - 2.0*blurSize)) * 0.12;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - blurSize)) * 0.15;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y)) * 0.16;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + blurSize)) * 0.15;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + 2.0*blurSize)) * 0.12;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + 3.0*blurSize)) * 0.09;
   sum += texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + 4.0*blurSize)) * 0.05;



   gl_FragColor = sum/2.0;

   }