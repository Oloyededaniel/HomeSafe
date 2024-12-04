# HomeSafe
HomeSafe: An IoT-driven smart home security system with real-time monitoring and Firebase integration.

# HomeSafe - An IoT-Driven Smart Home Security System
Project Overview
HomeSafe is a smart home security system built using IoT technology, designed to monitor the security and environmental conditions of a home. This project utilizes an Arduino MKR WiFi 1010 board with multiple sensors and Firebase integration to deliver real-time data on motion, temperature, humidity, and distance (for detecting object proximity). The system also includes a web application for data visualization and a live video feed for remote monitoring.

# Hardware Components
Arduino MKR WiFi 1010 - Microcontroller with built-in Wi-Fi for cloud connectivity.
PIR Motion Sensor - Detects human motion within a specified range.
DHT11 Temperature and Humidity Sensor - Monitors environmental conditions.
HC-SR04 Ultrasonic Distance Sensor - Measures the distance to nearby objects.
RGB LEDs - Provide visual alerts based on sensor data.

# Software Components
Arduino IDE - Used for programming the Arduino MKR.
Firebase - Cloud platform for real-time data storage and retrieval.
Web Application - A front-end interface for visualizing sensor data and the video feed.
HTML, CSS, JavaScript - Technologies for building the web dashboard.
Python/AI Library (Optional) - For processing the video feed and detecting human presence in real-time.

# System Architecture
The HomeSafe system is composed of three main layers:
Sensor Layer: Gathers environmental and security data.
Cloud Layer (Firebase): Stores and retrieves data, enabling remote access and visualization.
Application Layer: Provides an interactive dashboard for users, including the live video feed, and data visualizations.

# System Flow
Run arduino(finalProject.ino) -> Check firebase -> Run python code(humandection.py) ->  Start website.  
