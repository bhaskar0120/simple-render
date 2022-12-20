#include <math.h>
#include <stdbool.h>

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
  char background[3];
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

float clamp(float x, float min, float max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
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

#define MIN_DISTANCE 0.01
#define MAX_DISTANCE 100
#define MAX_ITERATION 500

bool in_shadow(struct Scene scene, Vector point){
  // Ray march from point to light
  // if collision before reaching light, return true
  // else return false
  
  Vector light = scene.light;
  Vector direction = normalize(sub(light, point));
  for (int i = 0; i < MAX_ITERATION; i++) {
    float distance;
    for(size_t i = 0; i < scene.shape_count; ++i) {
      struct Shape shape = scene.shapes[i];
      float d = shape.distance(shape, add(point, mul(direction, distance)));
      distance = fmin(distance, d);
    }
    if(distance < MIN_DISTANCE) return true;
    float distance_to_light = length(sub(light, point));
    if(distance > distance_to_light) return false;
    point = add(point, mul(direction, distance));
  }
  return false;
}

Vector get_intersection(struct Scene scene, Vector p, Vector* normal) {
  float distance = 0;
  size_t idx = 0;

  // Ray marching
  size_t iter = 0;
  for(iter = 0; iter < MAX_ITERATION; ++iter){
    // find closest shape from camera+p*distance
    float ds = MAX_DISTANCE;
    for(size_t i = 0; i < scene.shape_count; ++i) {
      struct Shape shape = scene.shapes[i];
      float d = shape.distance(shape, add(scene.camera, mul(p, distance)));
      if(ds > d) {
        ds = d;
        idx = i;
      }
      if(ds < MIN_DISTANCE){
        *normal = scene.shapes[idx].normal(scene.shapes[idx], add(scene.camera, mul(p, distance)));
        return add(scene.camera, mul(p, distance));
      }
        
    }
    distance += ds;
    if(distance > MAX_DISTANCE){
      break;
    }
  }
  normal->x = normal->y = normal->z = 0;
  return (Vector){MAX_DISTANCE, MAX_DISTANCE, MAX_DISTANCE};

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

      if(fabs(MAX_DISTANCE-intersection.x) < 0.001 
          || fabs(MAX_DISTANCE-intersection.y) < 0.001
          || fabs(MAX_DISTANCE-intersection.z) < 0.001) {
        img_buffer[((height-i-1) * width + j)*3 + 0] = scene.background[0];
        img_buffer[((height-i-1) * width + j)*3 + 1] = scene.background[1];
        img_buffer[((height-i-1) * width + j)*3 + 2] = scene.background[2];
        continue;
      }

      if(in_shadow(scene, intersection)){
        img_buffer[((height-i-1) * width + j)*3 + 0] = 
        img_buffer[((height-i-1) * width + j)*3 + 1] = 
        img_buffer[((height-i-1) * width + j)*3 + 2] = 
          (char)0;
        continue;
      }

      // brightness = dot(normal, light-intersection)
      float brightness = dot(normal, normalize(sub(scene.light, intersection)));
      brightness = clamp(brightness, 0, 1);

      // convert brightness to color
      int color = (int)(brightness * 255);

      img_buffer[((height-i-1) * width + j)*3 + 0] = 
      img_buffer[((height-i-1) * width + j)*3 + 1] = 
      img_buffer[((height-i-1) * width + j)*3 + 2] = 
        (char)(color);
    }
  }
}



