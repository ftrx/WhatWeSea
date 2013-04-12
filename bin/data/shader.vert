varying vec4 position;

void main() {

    gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec4 eyeCoord = gl_ModelViewProjectionMatrix * gl_Vertex;
    float dist = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z);
	float att	 = 600.0 / dist;
	float size    = gl_Normal.x * att;
    gl_PointSize  = size;
    gl_FrontColor = gl_Color;
    position = gl_Vertex;
}