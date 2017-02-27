################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../embedded/with_ACK/embedded_with_ACK_proc.cpp 

OBJS += \
./embedded/with_ACK/embedded_with_ACK_proc.o 

CPP_DEPS += \
./embedded/with_ACK/embedded_with_ACK_proc.d 


# Each subdirectory must supply rules for building sources it contributes
embedded/with_ACK/%.o: ../embedded/with_ACK/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -DSC_INCLUDE_DYNAMIC_PROCESSES -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -I"C:\Users\amr\workspace\Rechannel" -I"E:\simulators\boost_1_34_0" -I"C:\systemc-2.2.0\include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


