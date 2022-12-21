#include <stdio.h>
#include <stdlib.h>
#include "shape.c"

int convert_to_big_endian(int num) {
    int result = 0;
    result |= (num & 0x000000FF) << 24;
    result |= (num & 0x0000FF00) << 8;
    result |= (num & 0x00FF0000) >> 8;
    result |= (num & 0xFF000000) >> 24;
    return result;
}

int main(){
  // Create shapes
  struct Shape sphere = {
    .position = {
      .x = -2,
      .y = -.5,
      .z = 6
    },
    .parameter1 = .5,
    .distance = sphere_distance,
    .normal = sphere_normal
  };

  struct Shape plane = {
    .position = {
      .x = 0,
      .y = -1,
      .z = 0,
    },
    .vec_parameter = {
      .x = 0,
      .y = 1,
      .z = 0
    },
    .distance = infinite_plane_distance,
    .normal = infinite_plane_normal
  };

  struct Shape capsule= {
    .position = {
      .x = .5,
      .y = -.75,
      .z = 3
    },
    .vec_parameter = {
      .x = .8,
      .y = 0,
      .z = .3,
    },
    .parameter1 = 0.25,
    .distance = capsule_distance,
    .normal = capsule_normal
  };

  struct Shape torus = {
    .position = {
      .x = 1,
      .y = -0.2,
      .z = 5
    },
    .vec_parameter = {
      .x = 0,
      .y = 0,
      .z = -1,
    },
    .parameter1 = 0.5,
    .parameter2 = 0.3,
    .distance = torus_distance,
    .normal = torus_normal
  };


  // Create a scene

  struct Scene scene = {
    .shapes = {sphere,plane,capsule , torus},
    .shape_count = 4,
    .light = {
      .x = -5,
      .y = 5,
      .z = -1
    },
    .camera = {
      .x = 0,
      .y = 0,
      .z = 0
    },
    .background = {0, 0, 0}
  };


  int width = 800;
  int height = 600;

  // create an image buffer
  char* image = (char*)malloc(height * width * 3);

  // Render the scene
  render(image, height, width, scene);


  // Convert height and width to big endian
  int width_big_endian = convert_to_big_endian(width);
  int height_big_endian = convert_to_big_endian(height);

  // Write image height, width in a file as 4 byte bigendian 
  // The image is in RGB format with 8 bits per channel

  FILE *f = fopen("image.simp", "wb");
  fwrite(&height_big_endian, sizeof(int), 1, f);
  fwrite(&width_big_endian, sizeof(int), 1, f);
  fwrite(image, sizeof(char), width * height * 3, f);
  fclose(f);

  return 0;
}
