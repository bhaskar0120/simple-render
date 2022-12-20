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
  // Vector 
  Vector v = {
    .x = 2,
    .y = 0,
    .z = 0
  };

  // Sphere
  struct Shape sphere = {
    .position = {
      .x = 0,
      .y = 0,
      .z = 6
    },
    .parameter1 = 1.0,
    .distance = sphere_distance,
    .normal = sphere_normal
  };

  // Create a scene

  struct Scene scene = {
    .shapes = {sphere},
    .shape_count = 1,
    .light = {
      .x = 0,
      .y = 0,
      .z = 0
    },
    .camera = {
      .x = 0,
      .y = 0,
      .z = 0
    }
  };

  //create an image buffer

  int width = 1080;
  int height = 800;
  char* image = (char*)malloc(height * width * 3);

  // Render the scene
  render(image, height, width, scene);

  // Convert height and width to big endian
  int width_big_endian = convert_to_big_endian(width);
  int height_big_endian = convert_to_big_endian(height);


  // Write image height width a file as 4 byte bigendian to a file name "image.simp"
  // The image is in RGB format with 8 bits per channel
  FILE *f = fopen("image.simp", "wb");
  fwrite(&height_big_endian, sizeof(int), 1, f);
  fwrite(&width_big_endian, sizeof(int), 1, f);
  fwrite(image, sizeof(char), width * height * 3, f);
  fclose(f);


  printf("Sphere distance: %f\n", sphere.distance(sphere, v)); // Expected: 1
  return 0;
}
