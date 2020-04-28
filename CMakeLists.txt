cmake_minimum_required (VERSION 3.12.11)
project (DustAndTempSender)

find_library(wiringPi_LIB wiringPi)

add_executable(DustAndTempSenderWiringPi src/wiring.cpp)
target_link_libraries(DustAndTempSenderWiringPi ${wiringPi_LIB})

add_executable(DustAndTempSenderPigs src/pigs.cpp)
target_link_libraries(DustAndTempSenderPigs pigpio pthread)