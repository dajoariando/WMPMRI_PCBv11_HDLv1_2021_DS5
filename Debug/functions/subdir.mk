################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../functions/bstream.c \
../functions/common_functions.c \
../functions/dac_ad5722r_driver.c \
../functions/pll_calculator.c \
../functions/pll_param_generator.c \
../functions/reconfig_functions.c 

OBJS += \
./functions/bstream.o \
./functions/common_functions.o \
./functions/dac_ad5722r_driver.o \
./functions/pll_calculator.o \
./functions/pll_param_generator.o \
./functions/reconfig_functions.o 

C_DEPS += \
./functions/bstream.d \
./functions/common_functions.d \
./functions/dac_ad5722r_driver.d \
./functions/pll_calculator.d \
./functions/pll_param_generator.d \
./functions/reconfig_functions.d 


# Each subdirectory must supply rules for building sources it contributes
functions/%.o: ../functions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -Dsoc_cv_av -I"C:\intelFPGA\17.1\embedded\ip\altera\hps\altera_hps\hwlib\include" -I"C:\intelFPGA\17.1\embedded\ip\altera\hps\altera_hps\hwlib\include\soc_cv_av" -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


