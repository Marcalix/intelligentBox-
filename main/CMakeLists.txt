# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c led_task.c switch_task.c buzzer_ldr_task.c Sht3x_Driver.c i2c_task.c i2s_task.c CircularBuffer.c filters_task.c Wifi_task.c Mysql_task.c AI_task.c    # list the source files of this component
    INCLUDE_DIRS        # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
