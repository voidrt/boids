#version 430

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;
layout (location = 3) uniform float scale;

layout (std430, binding = 0) readonly buffer PositionBuffer { vec4 positions[];};
layout (std430, binding = 1) readonly buffer VelocityBuffer { vec4 velocities[];};

out vec4 boidColor;

void main() {
    vec3 boidPosition = positions[gl_InstanceID].xyz;
    vec3 boidVelocity = velocities[gl_InstanceID].xyz;

    vec3 vertexView = scale * vertexPosition;

    vec2 velocityInView = (view * vec4(boidVelocity, 0.0)).xy;
    float speed = length(velocityInView);

    float facingAngle = atan(velocityInView.y, velocityInView.x);
    float boidRotation = facingAngle - radians(90.0);

    vec2 xvec = vec2(cos(boidRotation), sin(boidRotation));
    vec2 yvec = vec2(-sin(boidRotation), cos(boidRotation));
    vertexView.xy = vertexView.x * xvec + vertexView.y * yvec;

    vertexView += (view * vec4(boidPosition, 1.0)).xyz;

    gl_Position = projection * vec4(vertexView, 1);

    float isBoidEven = float(gl_InstanceID % 2 == 0);
    float isBoidThird = float(gl_InstanceID % 3 == 0);

    boidColor.rgb = vec3(1.0 - isBoidEven, isBoidThird * isBoidThird, 1./0- isBoidThird);
    boidColor.a = 1.0;
}