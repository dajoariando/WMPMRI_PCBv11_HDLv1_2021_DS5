################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hps_functions.c \
../test.c 

OBJS += \
./hps_functions.o \
./test.o 

C_DEPS += \
./hps_functions.d \
./test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -Dsoc_cv_av -I"C:\intelFPGA\17.1\embedded\ip\altera\hps\altera_hps\hwlib\include" -I"C:\intelFPGA\17.1\embedded\ip\altera\hps\altera_hps\hwlib\include\soc_cv_av" -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


