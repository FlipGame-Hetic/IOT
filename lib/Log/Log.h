#pragma once
#include <Arduino.h>

//Setup for terminal color
#define LOG_USE_COLOR 0

// Color sets
#if LOG_USE_COLOR
#define COLOR_RESET "\033[0m"
#define COLOR_INFO  "\033[36m"
#define COLOR_WARN  "\033[33m"
#define COLOR_ERROR "\033[31m"
#else
#define COLOR_RESET ""
#define COLOR_INFO  ""
#define COLOR_WARN  ""
#define COLOR_ERROR ""
#endif

// Log base
#define LOG(level_color, level, tag, msg) \
do {\
    Serial.print("[");\
    Serial.print(tag);\
    Serial.print("]");\
    Serial.print(level_color "[");\
    Serial.print(level);\
    Serial.print("]" COLOR_RESET " ");\
    Serial.println(msg);\
} while(0)

// Log base printf
#define LOGF(level_color, level, tag, fmt, ...) \
do {\
    Serial.print("[");\
    Serial.print(tag);\
    Serial.print("]");\
    Serial.print(level_color "[");\
    Serial.print(level);\
    Serial.print("]" COLOR_RESET " ");\
    Serial.printf(fmt, ##__VA_ARGS__);\
    Serial.println();\
} while(0)

// Info logs
#define LOG_INFO(tag, msg) LOG(COLOR_INFO, "INFO", tag, msg)
#define LOG_INFOF(tag, fmt, ...) LOGF(COLOR_INFO, "INFO", tag, fmt, ##__VA_ARGS__)

// Warn logs
#define LOG_WARN(tag, msg) LOG(COLOR_WARN, "WARN", tag, msg)

// Error logs
#define LOG_ERROR(tag, msg) \
do {\
    Serial.print("[");\
    Serial.print(tag);\
    Serial.print("] ");\
    Serial.print(COLOR_ERROR "[ERROR]" COLOR_RESET " ");\
    Serial.print(__FILE__);\
    Serial.print(":");\
    Serial.print(__LINE__);\
    Serial.print(" ");\
    Serial.println(msg);\
} while(0)

// Module logs
#define LOG_WIFI(msg) LOG_INFO("WiFi", msg)
#define LOG_WIFIF(fmt, ...) LOG_INFOF("WiFi", fmt, ##__VA_ARGS__)
#define LOG_WIFI_ERR(msg) LOG_ERROR("WiFi", msg)