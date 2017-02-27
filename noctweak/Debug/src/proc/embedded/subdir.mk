################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/proc/embedded/embedded_app_info.cpp \
../src/proc/embedded/embedded_evaluation.cpp \
../src/proc/embedded/graph_parser.cpp \
../src/proc/embedded/task_mapping.cpp 

OBJS += \
./src/proc/embedded/embedded_app_info.o \
./src/proc/embedded/embedded_evaluation.o \
./src/proc/embedded/graph_parser.o \
./src/proc/embedded/task_mapping.o 

CPP_DEPS += \
./src/proc/embedded/embedded_app_info.d \
./src/proc/embedded/embedded_evaluation.d \
./src/proc/embedded/graph_parser.d \
./src/proc/embedded/task_mapping.d 


# Each subdirectory must supply rules for building sources it contributes
src/proc/embedded/%.o: ../src/proc/embedded/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -DSC_INCLUDE_DYNAMIC_PROCESSES -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -I"C:\Users\amr\workspace\Rechannel" -I"E:\simulators\boost_1_34_0" -I"C:\systemc-2.2.0\include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


