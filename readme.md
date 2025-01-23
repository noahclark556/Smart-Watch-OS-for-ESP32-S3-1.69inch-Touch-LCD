# Smart Watch OS for ESP32-S3 1.69inch Touch LCD

**Written by**: Noah Clark 

**Date Created**: 2024-07-01

**Last Updated**: 2025-01-23

**Language**: C/C++

**Target Platform**: Waveshare ESP32-S3 1.69inch Touch LCD

**Development Environment**: macOS Sequoia 15.2, MacBook M2 Pro 2023 14-inch

**External Hardware**: Waveshare ESP32-S3 1.69inch Touch LCD

## Overview

This project is a comprehensive operating system built for the ESP32-S3 1.69inch Touch LCD, utilizing the LVGL (Light and Versatile Graphics Library) for creating a user-friendly graphical interface. The application integrates various functionalities, including Wi-Fi connectivity, a dashboard layout, and a customizable agenda feature. This project is more a proof of concept / starting point for a more complex operating system. This code can be ported into arduino IDE if needed, however I recommend using PlatformIO in VSCode for development.

## Features

- **Wi-Fi Connectivity**: Easily connect to Wi-Fi network.
- **Dynamic Dashboard**: A visually appealing dashboard that displays real-time information.
- **Customizable Layout**: The layout can be modified to suit user preferences.
- **Image Support**: Includes support for displaying images using LVGL.

## Note to Viewers

- Normally when I do comprehensive projects like this, I keep detailed notes for each and every step, bug, etc. Unfortunately, I didn't have the time to do that during development. I'll try to swing back around later and add more detailed notes.

## Directory Structure

```
.
├── include
│   └── README
├── lib
│   ├── Arduino_DriveBus
│   │   ├── LICENSE
│   │   ├── library.properties
│   │   └── examples
│   └── README
├── src
│   ├── layout
│   │   └── layout.cpp
│   ├── main.cpp
│   └── resources
│       └── IMG_NOAH.h
├── test
│   └── README
├── .gitignore
└── platformio.ini
```

## Getting Started

### Prerequisites

- **PlatformIO**: Ensure you have 'PlatformIO IDE' and 'ESP-IDF' extensions installed in your VSCode development environment.
- **ESP32-S3 Board**: This project is designed to run on the ESP32-S3 1.69inch Touch LCD.

### Installation

1. Clone this repository:

2. Open the project in VSCode with PlatformIO IDE and ESP-IDF extensions installed.

3. Install the required libraries specified in `platformio.ini`.

4. Connect your ESP32-S3 1.69inch Touch LCD to your computer via USB.

5. Upload the code to your board.

### Usage

- After uploading, open the serial monitor to view debug messages.

## Contributing

Contributions are welcome!