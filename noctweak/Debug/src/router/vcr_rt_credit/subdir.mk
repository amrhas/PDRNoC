################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/router/vcr_rt_credit/route_compvc_rt.cpp \
../src/router/vcr_rt_credit/vc_rt_buffer.cpp 

OBJS += \
./src/router/vcr_rt_credit/route_compvc_rt.o \
./src/router/vcr_rt_credit/vc_rt_buffer.o 

CPP_DEPS += \
./src/router/vcr_rt_credit/route_compvc_rt.d \
./src/router/vcr_rt_credit/vc_rt_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
src/router/vcr_rt_credit/%.o: ../src/router/vcr_rt_credit/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -DSC_INCLUDE_DYNAMIC_PROCESSES -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -I"C:\Users\amr\workspace\Rechannel" -I"E:\simulators\boost_1_34_0" -I"C:\systemc-2.2.0\include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


