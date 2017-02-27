################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/router/wormhole_pipeline/buffer.cpp \
../src/router/wormhole_pipeline/route_comp.cpp \
../src/router/wormhole_pipeline/sa_allocator.cpp \
../src/router/wormhole_pipeline/wormhole_pipeline.cpp \
../src/router/wormhole_pipeline/wormhole_pipeline_power_est.cpp 

OBJS += \
./src/router/wormhole_pipeline/buffer.o \
./src/router/wormhole_pipeline/route_comp.o \
./src/router/wormhole_pipeline/sa_allocator.o \
./src/router/wormhole_pipeline/wormhole_pipeline.o \
./src/router/wormhole_pipeline/wormhole_pipeline_power_est.o 

CPP_DEPS += \
./src/router/wormhole_pipeline/buffer.d \
./src/router/wormhole_pipeline/route_comp.d \
./src/router/wormhole_pipeline/sa_allocator.d \
./src/router/wormhole_pipeline/wormhole_pipeline.d \
./src/router/wormhole_pipeline/wormhole_pipeline_power_est.d 


# Each subdirectory must supply rules for building sources it contributes
src/router/wormhole_pipeline/%.o: ../src/router/wormhole_pipeline/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -DSC_INCLUDE_DYNAMIC_PROCESSES -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -I"C:\Users\amr\workspace\Rechannel" -I"E:\simulators\boost_1_34_0" -I"C:\systemc-2.2.0\include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


