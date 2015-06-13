precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D u_textureUnit;
uniform vec4 u_color;

void main(void) {
    gl_FragColor = vec4(1, 1, 1, texture2D(u_textureUnit, v_texcoord).a) * u_color;
}