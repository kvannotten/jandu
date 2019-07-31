target:=jandu

project_dir:=$(CURDIR)
src_dir:=$(project_dir)/src
build_dir:=$(project_dir)/build

CC:=c99
CFLAGS:=-Wall -pedantic
LDFLAGS:=-lSDL2 -lSDL2_image
