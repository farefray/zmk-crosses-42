# Local Crosses 42 firmware extensions.
target_sources_ifdef(CONFIG_CROSSES_MOTION_GATED_TEMP_LAYER app PRIVATE
                     src/input_processor_motion_gated_temp_layer.c)
