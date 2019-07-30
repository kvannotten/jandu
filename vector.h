#include <stdbool.h>

#define VECTOR_INITIAL_CAPACITY 16

typedef struct{
  int x;
  int y;
}Coord;

typedef struct {
    int size;
    int capacity;
    Coord *data;
} Vector;

void init(Vector *vector);
void append(Vector *vector, Coord value);
void prepend(Vector *vector, Coord value);
// void delete(Vector *vector, int index);
void delete_value(Vector *vector, Coord value);
void set(Vector *vector, int index, Coord value);
void resize(Vector *vector);
void free_memory(Vector *vector);

Coord get(Vector *vector, int value);
Coord pop(Vector *vector);
int find_value(Vector *vector, Coord value);
int size(Vector *vector);
int capacity(Vector *vector);

bool is_empty(Vector *vector);
