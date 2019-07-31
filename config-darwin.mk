# ==============================================================
# Darwin (macOS) specific overrides go in here
# this file is included AFTER config.mk
# setting a variable in here will override it completely on macOS
# ===============================================================

CC=clang
CFLAGS=-std=c99 -Wall -pedantic
