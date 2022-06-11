# GraphicEngine

Graphic Engine and Model Generator made in C++. 
The engine uses OpenGl to render 3D models, i.e, models of points that describe triangles.
The generator creates some basic primitives :
- Plane
- Box
- Cone
- Sphere
- Bezier
- Torus

 ## Engine
 
 As refered before, the engine uses OpenGL to render a scene, it also applies transformations, textures, normals and lights. These are passed in a XML file passed by argument to the engine, that follows a syntax that can be found in the *xml_syntax* file.
 
 To run the executable you must provide the xml as an argument and have all the files (from generator) and textures (if using) in the respective folder.
 
 ## Generator
 
 The generator creates 2 files, one containing the points of the triangles and the respective normals, and the other file has the points for the textures.
 To create a different model with bezier it's recommended to read *README.md* in *bezier_info*'s folder. *Teapot*'s patch is already provided as an example.
 
 ### Plane
 `./generator plane size divisions outFile`

 Ex: `./generator plane 1 3 plane.3d`
 
 ### Box
 `./generator box size divisions outFile`
 
 Ex: `./generator box 1 3 box.3d`
 
 ### Cone
 `./generator cone radius height slices stacks outFile`
 
 Ex: `./generator cone 1 2 10 10 cone.3d`
 
 ### Sphere
 `./generator sphere radius slices stacks outFile`
 
 Ex: `./generator sphere 1 10 10 sphere.3d`
 
 ### Bezier
 `./generator bezier inFile level outFile`
 
 Ex: `./generator bezier teapot.patch 64 bezier.3d`
 
  ### Torus
 `./generator torus outterRadius innerRadius slices stacks outFile`
 
 Ex: `./generator torus 2.4 2.2 20 20 torus.3d`
 
 
 
