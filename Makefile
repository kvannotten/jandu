# include make configuration
include config.mk

 # include darwin specific overrides
ifeq ($(shell uname -s),Darwin)
include config-darwin.mk
endif

# collect sources and headers
source_files:=$(shell find $(src_dir) -name "*.c")
header_files:=$(shell find $(src_dir) -name "*.h")

all: options $(build_dir)/$(target) 

options:
	@echo "Building $(target)"
	@echo "CC      = $(CC)"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo ""

$(build_dir)/$(target): $(source_files)
	@mkdir -p $(build_dir)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(source_files)

clean:
	rm -f $(build_dir)/$(target)

.PHONY: all options clean
