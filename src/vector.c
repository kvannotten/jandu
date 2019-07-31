#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

void init(Vector *vector){
    vector->size = 0;
    vector->capacity = VECTOR_INITIAL_CAPACITY;
    vector->data = malloc(sizeof(Coord) * vector->capacity);
}

Coord get(Vector *vector, int index){
    if(index > vector->capacity || index < 0){
        printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
        exit(1);
    }

    return vector->data[index];
}

void set(Vector *vector, int index, Coord value){
    while(index >= vector->size){
        append(vector, (Coord){0});
    }

    vector->data[index] = value;
}

void append(Vector *vector, Coord value){
    resize(vector);

    vector->data[vector->size++] = value;
}

void prepend(Vector *vector, Coord value){
    set(vector, 0, value);
    vector->size++;
}

Coord pop(Vector *vector){
    Coord data = vector->data[vector->size - 2];
    set(vector, vector->size - 1, (Coord){0});
    vector->size = vector->size - 1;
    return data;
}

void delete(Vector *vector, int index){
    for(int i = 0; i < index; i++){
        vector->data[index + i] = vector->data[index + i + 1];
    }
    vector->size = vector->size - 1;
}

void delete_value(Vector *vector, Coord value){
    for(int i = 0; i < vector->size; i++){
        if(vector->data[i].x == value.x && vector->data[i].y == value.y){
            delete(vector, i);
        }
    }
}

int find_value(Vector *vector, Coord value){
    for(int i = 0; i < vector->size; i++){
        if(vector->data[i].x == value.x && vector->data[i].y == value.y){
            return i;
        }
    }

    return -1;
}

void resize(Vector *vector){
    if(vector->size >= vector->capacity){
        vector->capacity *= 2;
        vector->data = realloc(vector->data, sizeof(Coord) * vector->capacity);
    }
}

int size(Vector *vector){
    return vector->size;
}

int capacity(Vector *vector){
    return vector->capacity;
}

bool is_empty(Vector *vector){
    return vector->size == 0;
}

void free_memory(Vector *vector){
    free(vector->data);
}
