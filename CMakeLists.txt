cmake_minimum_required (VERSION 3.12.11)
project (DustAndTempSender)

find_library(wiringPi_LIB wiringPi)

add_executable(DustAndTempSenderWiringPi src/wiring.cpp)
target_link_libraries(DustAndTempSenderWiringPi ${wiringPi_LIB})

add_executable(DustAndTempSenderPigs src/pigs.cpp)
target_link_libraries(DustAndTempSenderPigs pigpio pthread)

add_executable(McpComm src/digipot.cpp)
target_link_libraries(McpComm pigpio pthread)

add_executable(mprls src/mprlsRead.cpp DS18B20/src/DS18B20.cpp)
target_link_libraries(mprls pigpio pthread)