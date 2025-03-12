# List of source files
set(SRC_FILES
    src/main.c
)

# List of sources
set(INC_DIRS
    inc/
)

# List of includes
set(REQ_LIBS
    # ESP-IDF components
    driver
    # Other components 
    ring-buffer
)

# Register component
idf_component_register(SRCS ${SRC_FILES} INCLUDE_DIRS ${INC_DIRS} REQUIRES ${REQ_LIBS})

# Create storage partition and flash it to project
# spiffs_create_partition_image(storage ../partitions/flash FLASH_IN_PROJECT)