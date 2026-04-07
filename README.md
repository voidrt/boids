3D Boids simulation. Runs on a compute shader and has small optimization patterns in the GLSL and C++ throughout. 
Can render up to 32k boids on screen, possibly more (Intel ARC B570), performance hindered by an O(n^2) for-loop. 
The rendering is also done on the GPU with the vertex and frag shaders, following arceryz's raylib-gpu-particles repo.

boids :D

https://github.com/user-attachments/assets/49b2ed2d-4e66-4b64-a5be-ec6187ae4616


