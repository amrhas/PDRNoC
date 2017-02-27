################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicKhalidDC.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicMinComm.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicRandom.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStatic.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStaticSM.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStaticTriCore.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicWeka.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZag.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZagSM.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZagThreeCore.cxx \
../src/proc/dreamcloud/commons/mapping_heuristic/uoyHeuristicModuleStatic.cxx 

OBJS += \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicKhalidDC.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicMinComm.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicRandom.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStatic.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStaticSM.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStaticTriCore.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicWeka.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZag.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZagSM.o \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZagThreeCore.o \
./src/proc/dreamcloud/commons/mapping_heuristic/uoyHeuristicModuleStatic.o 

CXX_DEPS += \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicKhalidDC.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicMinComm.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicRandom.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStatic.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStaticSM.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicStaticTriCore.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicWeka.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZag.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZagSM.d \
./src/proc/dreamcloud/commons/mapping_heuristic/dcMappingHeuristicZigZagThreeCore.d \
./src/proc/dreamcloud/commons/mapping_heuristic/uoyHeuristicModuleStatic.d 


# Each subdirectory must supply rules for building sources it contributes
src/proc/dreamcloud/commons/mapping_heuristic/%.o: ../src/proc/dreamcloud/commons/mapping_heuristic/%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -DSC_INCLUDE_DYNAMIC_PROCESSES -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -I"C:\Users\amr\workspace\Rechannel" -I"E:\simulators\boost_1_34_0" -I"C:\systemc-2.2.0\include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


