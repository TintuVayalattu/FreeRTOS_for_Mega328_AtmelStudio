serial.o serial.d : serial/serial.c ../../Source/include/FreeRTOS.h \
  FreeRTOSConfig.h ../../Source/include/projdefs.h \
  ../../Source/include/portable.h \
  ../../Source/include/deprecated_definitions.h \
  ../../Source/include/../portable/GCC/ATMega323/portmacro.h \
  ../../Source/include/mpu_wrappers.h ../../Source/include/queue.h \
  ../../Source/include/task.h ../../Source/include/list.h \
  ../Common/include/serial.h
