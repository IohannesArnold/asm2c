SHELL = /bin/sh

CP    = $(shell which cp)
MAKE  = $(shell which make)

# The build directory
BUILD_DIR = build

AS    = $(BUILD_DIR)/bin/as
LD    = $(BUILD_DIR)/bin/ld

# The directory to install to
INSTALL_DIR = /opt/btk
