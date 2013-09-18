# List of all the ChibiOS/RT HAL files, there is no need to remove the files
# from this list, you can disable parts of the kernel by editing halconf.h.
QUADSRC = ${CHIBIOS}/Quad/GCS.cpp \
         ${CHIBIOS}/Quad/Datalogger.cpp \
         ${CHIBIOS}/Quad/ExternalInterrupt.cpp \
         ${CHIBIOS}/Quad/Fernsteuerung.cpp \
         ${CHIBIOS}/Quad/Lage.cpp \
         ${CHIBIOS}/Quad/Motoren.cpp \
         ${CHIBIOS}/Quad/Regelung.cpp \
         ${CHIBIOS}/Quad/MPU6050/MPU6050.cpp \
         ${CHIBIOS}/Quad/I2Cdev/I2Cdev.cpp \
         ${CHIBIOS}/Quad/Flash_driver/eeprom.cpp \
         ${CHIBIOS}/Quad/Flash_driver/Flash.cpp \
         ${CHIBIOS}/Quad/Flash_driver/stm32f4xx_flash.cpp



# Required include directories
QUADINC = ${CHIBIOS}/Quad \
${CHIBIOS}/Quad/mavlink \
${CHIBIOS}/Quad/mavlink/common \
${CHIBIOS}/Quad/MPU6050 \
${CHIBIOS}/Quad/I2Cdev \
${CHIBIOS}/Quad/Flash_driver

