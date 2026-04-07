3D Boids simulation. Runs on a compute shader and has small optimization patterns in the GLSL and C++ throughout.

Can render up to 32k boids on screen, possibly more (Intel ARC B570), performance hindered by an O(n^2) for-loop.
The rendering is also done on the GPU with the vertex and frag shaders, following arceryz's raylib-gpu-particles repo.

boids :D

https://github.com/user-attachments/assets/6c2019c7-8a9e-4228-8e7f-099b64a5a397


