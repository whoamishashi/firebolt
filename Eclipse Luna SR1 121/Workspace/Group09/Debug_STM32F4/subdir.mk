################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ActuatorRW.cpp \
../Template.cpp \
../actuatorServo.cpp \
../actuatorThruster.cpp \
../globals.cpp \
../sensorIMU.cpp \
../telecommand.cpp \
../telemetry.cpp \
../topics.cpp 

OBJS += \
./ActuatorRW.o \
./Template.o \
./actuatorServo.o \
./actuatorThruster.o \
./globals.o \
./sensorIMU.o \
./telecommand.o \
./telemetry.o \
./topics.o 

CPP_DEPS += \
./ActuatorRW.d \
./Template.d \
./actuatorServo.d \
./actuatorThruster.d \
./globals.d \
./sensorIMU.d \
./telecommand.d \
./telemetry.d \
./topics.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DUSE_STM32_DISCOVERY -DSTM32F40_41xxx -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\STM32F4xx_StdPeriph_Driver\inc" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\CMSIS\Include" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\hal" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal-generic" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\independent\gateway" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\independent" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\api" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\src\bare-metal\stm32f4\sdCard" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\api\hal" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\rodos\default_usr_configs" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\support_libs" -I"D:\spacemaster\firebolt\branches\firebolt_branch01\Eclipse Luna SR1 121\Workspace\support_libs\flash\spiFlash_AT45DBxxx" -fabi-version=0 -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


