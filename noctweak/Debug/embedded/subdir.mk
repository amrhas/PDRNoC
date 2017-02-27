################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../embedded/embedded_app_info.cpp \
../embedded/embedded_evaluation.cpp \
../embedded/graph_parser.cpp \
../embedded/task_mapping.cpp 

OBJS += \
./embedded/embedded_app_info.o \
./embedded/embedded_evaluation.o \
./embedded/graph_parser.o \
./embedded/task_mapping.o 

CPP_DEPS += \
./embedded/embedded_app_info.d \
./embedded/embedded_evaluation.d \
./embedded/graph_parser.d \
./embedded/task_mapping.d 


# Each subdirectory must supply rules for building sources it contributes
embedded/%.o: ../embedded/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -DSC_INCLUDE_DYNAMIC_PROCESSES -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -I"C:\Users\amr\workspace\Rechannel" -I"E:\simulators\boost_1_34_0" -I"C:\systemc-2.2.0\include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


