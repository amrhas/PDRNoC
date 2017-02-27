################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../src/proc/dreamcloud/dc_platform.cxx \
../src/proc/dreamcloud/dc_proc.cxx \
../src/proc/dreamcloud/dc_proc_type.cxx 

OBJS += \
./src/proc/dreamcloud/dc_platform.o \
./src/proc/dreamcloud/dc_proc.o \
./src/proc/dreamcloud/dc_proc_type.o 

CXX_DEPS += \
./src/proc/dreamcloud/dc_platform.d \
./src/proc/dreamcloud/dc_proc.d \
./src/proc/dreamcloud/dc_proc_type.d 


# Each subdirectory must supply rules for building sources it contributes
src/proc/dreamcloud/%.o: ../src/proc/dreamcloud/%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -DSC_INCLUDE_DYNAMIC_PROCESSES -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -I"C:\Users\amr\workspace\Rechannel" -I"E:\simulators\boost_1_34_0" -I"C:\systemc-2.2.0\include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


