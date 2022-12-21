# Simple Render
It is a very simple rendering engine that uses **Ray Marching Techinique**. This project is to explore the field of rendering engines and ray marching techniques

### ToDo
- [x] Make different shapes
- [x] Add shadows
- [ ] Add boolean operators
- [ ] Add color to light

### Engine Details

#### Shapes that the engine supports
- [x] Sphere
- [x] Infinite plane
- [x] Capsule
- [x] Torus

#### Lighting that the engine supports
- [x] Simple Sun Lamp Lighting

### Documentation

#### Structs
*Vector*: A vector is a simple struct that contains three floats `(x,y,z)`

*Shape struct*: are simple structs that contain information about the shape

*Sphere*
```
  .position (Vector)                            // position of the sphere
  .parameter1 (float)                           // radius of the sphere
  .distance ( (struct Shape, Vector) => float)  // a function that gives distance between point and sphere surface 
  .distance = sphere_distance;                  // inbuilt function which can be used for spheres
  .normal ( (struct Shape, Vector) => Vector)   // a function that gives Normal vector at the given surface point
  .normal = sphere_normal;                      // inbuilt function which can be used for spheres
```

*Plane*
```
  .position (Vector)                            // position of the plane
  .vec_parameter (Vector)                       // normal of the plane
  .distance ( (struct Shape, Vector) => float)  // a function that gives distance between point and plane 
  .distance = infinite_plane_distance;          // inbuilt function which can be used for planes
  .normal ( (struct Shape, Vector) => Vector)   // a function that gives Normal vector at the given surface point
  .normal = infinite_plane_normal;              // inbuilt function which can be used for planes
```

*Capsule*
```
  .position (Vector)                            // position of one of the spheres
  .vec_parameter (Vector)                       // relative position of second sphere
  .parameter1 (float)                           // radius of the spheres
  .distance ( (struct Shape, Vector) => float)  // a function that gives distance between point and capsule surface 
  .distance = capsule_distance;                 // inbuilt function which can be used for capsule
  .normal ( (struct Shape, Vector) => Vector)   // a function that gives Normal vector at the given surface point
  .normal = capsule_normal;                     // inbuilt function which can be used for capsule
```

*Torus*
```
  .position (Vector)                            // position of the torus
  .vec_parameter (Vector)                       // normal of the torus
  .parameter1 (float)                           // radius of the torus
  .parameter2 (float)                           // thickness of the torus
  .distance ( (struct Shape, Vector) => float)  // a function that gives distance between point and torus 
  .distance = torus_distance;                   // inbuilt function which can be used for torus
  .normal ( (struct Shape, Vector) => Vector)   // a function that gives Normal vector at the given surface point
  .normal = torus_normal;                       // inbuilt function which can be used for torus
```

*Scene*: Scene struct is sturct that contains the entire scene 
```
  .shapes (struct Shape[])  // a list of all shapes in the scene
  .shape_count (int)        // number of shapes
  .light (Vector)           // position of the light source
  .camera (Vector)          // position of the camera
  .background (char[3])     // RGB values of background
```

#### Get started 

1. To get started, add the file `#include "shape.c"`
2. Then create `Shape struct`, which are simple structs that contain the shapes in the scene
3. Add all shapes into a scene
4. Set the light and camera position. camera always looks toward `(0,0,1)`
5. Use `render(char[height*width*3] img_buffer, height, width, scene)` to render the image in buffer
6. Use Simper image format to view the image [Simper image viewer](https://github.com/bhaskar0120/Simper)


### How to view the output image
To view the output image use the [Simper image viewer](https://github.com/bhaskar0120/Simper) - (Python version Recommended ✅)

### How to compile the engine
Compile using `make.sh` file. (_Careful: it also runs the file_)

Although you can also compile it using your favourite compiler as it has no dependencies on external library
