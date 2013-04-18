
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

   gl_FragColor = gl_Color *vec4(position.z/600.0,position.z/600.0,position.z/600.0,1.0);

   }