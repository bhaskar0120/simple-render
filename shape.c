#include <math.h>

#define SHAPE_LIMIT 256

typedef struct {
  float x, y, z;
} Vector;

struct Shape {
  Vector position;
  Vector vec_parameter;
  float parameter1;
  float parameter2;
  float parameter3;

  float (*distance)(struct Shape shape, Vector point);
  Vector (*normal)(struct Shape shape, Vector point);

} ;

struct Scene {
  struct Shape shapes[SHAPE_LIMIT];
  size_t shape_count;
  Vector light;
  Vector camera;
} ;


// Vector functions
float dot(Vector a, Vector b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

float length(Vector a) {
  return sqrt(dot(a, a));
}

Vector normalize(Vector a) {
  float l = length(a);
  return (Vector){a.x / l, a.y / l, a.z / l};
}

Vector add(Vector a, Vector b) {
  return (Vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector sub(Vector a, Vector b) {
  return (Vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector mul(Vector a, float b) {
  return (Vector){a.x * b, a.y * b, a.z * b};
}

// Distance function of shapes

float sphere_distance(struct Shape sphere, Vector p) {
  // Parameter 1 is radius
  return length(sub(p, sphere.position)) - sphere.parameter1;
}

float infinite_plane_distance(struct Shape plane, Vector p) {
  // Vector parameter is normal
  return dot(sub(p, plane.position), normalize(plane.vec_parameter));
}



// Normal function of shapes

Vector sphere_normal(struct Shape sphere, Vector p) {
  return normalize(sub(p, sphere.position));
}

Vector infinite_plane_normal(struct Shape plane, Vector _p) {
  return plane.vec_parameter;
}


// Render function

#define MIN_DISTANCE 0.001
#define MAX_DISTANCE 1000
#define MAX_ITERATION 50

Vector get_intersection(struct Scene scene, Vector p, Vector* normal) {
  float distance = 0;
  size_t idx = 0;
  // Ray marching
  for(int iter = 0; iter < MAX_ITERATION; ++iter){
    // find closest shape from camera+p*distance
    float ds = MAX_DISTANCE;
    for(size_t i = 0; i < scene.shape_count; ++i) {
      struct Shape shape = scene.shapes[i];
      float d = shape.distance(shape, add(scene.camera, mul(p, distance)));
      if(ds > d) {
        ds = d;
        idx = i;
      }
      if(ds < MIN_DISTANCE) break;
    }
    distance += ds;
    if(distance > MAX_DISTANCE){
      *normal = (Vector){0, 0, 0};
      return (Vector){MAX_DISTANCE,MAX_DISTANCE, MAX_DISTANCE};
    }
    if(ds < MIN_DISTANCE) break;
  }
  *normal = scene.shapes[idx].normal(scene.shapes[idx], add(scene.camera, mul(p, distance)));
  return add(scene.camera, mul(p, distance));
}

void render(char* img_buffer, int height, int width, struct Scene scene) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      float x = ((float)j / width ) -0.5;
      float y = ((float)i / height) -0.5;
      x *= (float)width / height;

      Vector ray = normalize(sub((Vector){x, y, 1}, scene.camera));
      Vector normal;

      Vector intersection =  get_intersection(scene, ray, &normal);

      //get color from normal
      float r = (normal.x + 1) * 0.5;
      float g = (normal.y + 1) * 0.5;
      float b = (normal.z + 1) * 0.5;


      // get distance from intersection to camera
      float distance = length(sub(intersection, scene.camera));

      // convert distance to color
      float brightness = 1 - fmin(distance / MAX_DISTANCE, 1);
      
      // convert brightness to color
      int color = (int)(brightness * 255);

      // write color to image buffer
      img_buffer[((height-i-1) * width + j)*3 + 0] = (char)(r * 255);
      img_buffer[((height-i-1) * width + j)*3 + 1] = (char)(g * 255);
      img_buffer[((height-i-1) * width + j)*3 + 2] = (char)(b * 255);
    }
  }
}



