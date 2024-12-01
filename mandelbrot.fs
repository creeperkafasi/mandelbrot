#version 330 core

uniform vec2 position;
uniform float zoom;
uniform int maxIter;
uniform vec2 size;

vec2 cMul(vec2 z1, vec2 z2) {
	return vec2(
		z1.x*z2.x - z1.y*z2.y,
		z1.x*z2.y + z1.y*z2.x
	);
}

void main() {
	vec2 coords = gl_FragCoord.xy / size;
	coords *= zoom;
	coords.x += position.x;
	coords.y -= position.y;

	int i;
	vec2 z = vec2(0,0);
	for (i = 0; i < maxIter; i++) {
		z = cMul(z, z) + coords;

		if ( dot(z, z) > 4 ) break;
	}

	gl_FragColor = vec4(float(i) / float(maxIter), 0.0, 0.0, 1.0);
}

