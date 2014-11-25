uniform mat4 u_MVPMatrix;

attribute vec4 a_texcoord;

varying vec2 v_texcoord;

void main(void) {
    gl_Position = u_MVPMatrix * vec4(a_texcoord.xy, 0, 1);
    v_texcoord = a_texcoord.zw;
}