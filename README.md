# PendingNameEngine2

This 3D Engine is created by two students from Barcelona for the subject of 3D Engines. 
The github repository page is: https://github.com/GuillemD/PendingNameEngine2

Version 2.0 allows the user to drag and drop FBX files to load geometry and also drop textures (.png,.dds) to use on those models. Everyone of those 
will be saved in our own file format ".caca" if  they are meshes or ".dds" if they are textures and then they will load from there which will be faster.
And talk about faster, Octree enables our mousepicking to be faster so select GameObjects clicking in the screen without lag. Same goes for frustum culling
it enables the scene to run more smoothly even with street.fbx loaded. You can use the gizmo to move GameObjects around the scene.


Camera has Unity-like camera controls:
- RMB + 'WASD' to move around the space
- Mouse wheel to Zoom IN/OUT
- ALT + LMB to orbit an object
- 'F' to focus camera to selected game object.
- HOLD SHIFT increases camera speed

In the Main menu, you have several features:
- Saving configuration
- Show/Hide configuration panel  ( information about performance, debug functionalities)
- Inspector panel to show geometry properties of a selected game object. You can edit properties such as escale position and rotation from there.
- Hierarchy Panel displaying all the game objects in a tree (also able to delete all scene game objects)
- Show/hide console
- Create primitives using par_shapes
- Random number generator

INNOVATION: Octree


In this version we are using SDL/OpenGL and several external libraries:

- Assimp: http://www.assimp.org/
- DevIL: http://openil.sourceforge.net/
- GLEW: http://glew.sourceforge.net/
- ImGui: https://github.com/ocornut/imgui
- MathGeoLib: https://github.com/juj/MathGeoLib
- PCG RNG: http://www.pcg-random.org/
- Par_Shapes: https://prideout.net/shapes
- RapidJson: https://rapidjson.org/
- gpudetect: https://github.com/GameTechDev/gpudetect
- mmgr: http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml
- PhysFS: https://icculus.org/physfs/
- ImGuizmo: https://github.com/CedricGuillemet/ImGuizmo
- WWISE: https://www.audiokinetic.com/products/wwise/
### MIT LICENSE

Copyright (c) 2019 Asier Arellano Moreno and Guillem Dominguez Ruiz de Villa

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


### Report a bug

https://github.com/GuillemD/PendingNameEngine2/issues

### Creators github pages

- Asier Arellano: www.github.com/axiermo

- Guillem Domínguez: www.github.com/GuillemD
