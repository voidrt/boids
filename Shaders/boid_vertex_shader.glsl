#version 430

layout(location = 0) in vec3 vertexPosition;

layout (location=0) uniform mat4 projectionMatrix;
layout (location=1) uniform mat3 viewMatrix;

layout(std430, binding=0) readonly buffer inPosition {
    vec2 positions[];
};
layout(std430, binding=1) readonly buffer inVelocity {
    vec2 velocities[];
};


out vec4 BoidColor;

void main(){
    vec3 myPosition = vec3(positions[gl_InstanceID].xy, 0.0);
    vec3 myVelocity = vec3(velocities[gl_InstanceID].xy,0.0);
    vec3 vertex = vertexPosition * 1;

    BoidColor.rgb = abs(normalize(vec3(myVelocity.x, myVelocity.y, 0.0))) + 0.25;
    BoidColor.a = 1.0;

//    vec2 targetDirection = (viewMatrix * myVelocity).xy;
//    float facingAngle = atan(targetDirection.y, targetDirection.x);
//    float speed = length(targetDirection);

    vertex.xy = vertex.x*xvec + vertex.y*yvec;

    vertex += (viewMatrix * (myPosition)).xyz;

    gl_Position = projectionMatrix * vec4(vertex, 1.0);
}