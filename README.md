# reaction-time-tester
ESP32-S3 Reaction Time Tester tutorial website
# ESP32-S3 Reaction Time Tester

This repository contains the source code and tutorial webpage for my ESP32-S3 microcontroller final project.

## Project Overview

This project is a simple **reaction time tester**. It uses:

- One LED
- Two pushbuttons
- An ESP32-S3 microcontroller

### How it works

1. The user presses the **Start Button**.
2. The program waits for a random delay between 1 and 5 seconds.
3. The LED turns on.
4. The user presses the **Reaction Button** as quickly as possible.
5. The reaction time is printed in milliseconds.
6. If the user presses too early, the program displays a warning message.

---

## Repository Contents

### `index.html`
The GitHub Pages tutorial webpage for this project.

### `wokwi-version/sketch.ino`
The Arduino-style C++ version used in the Wokwi browser simulation.

### `esp-idf-c-version/main.c`
A C source file version of the reaction time tester, written in ESP-IDF style for a physical ESP32-S3 environment.

### `esp-idf-c-version/reaction_time_tester.h`
A header file containing GPIO definitions and timing constants for the ESP-IDF C version.

---

## Pin Assignments

| Function | ESP32-S3 Pin |
|---|---|
| LED | GPIO 4 |
| Start Button | GPIO 5 |
| Reaction Button | GPIO 6 |

---

## Online Tutorial Website

The GitHub Pages tutorial webpage is available here:

**https://qinge-crypto.github.io/reaction-time-tester/**

---

## Wokwi Simulation

The working Wokwi simulation is available here:

**https://wokwi.com/projects/464041653053491201**

---

## Notes on the Two Code Versions

This repository includes two code versions:

1. **Wokwi / Arduino version**
   - File: `sketch.ino`
   - Used for the browser-based Wokwi simulation and instructional video.

2. **C source version**
   - Files: `main.c` and `reaction_time_tester.h`
   - Included as human-readable C and header source files for the ESP32-S3 project submission.
   - This version follows ESP-IDF-style C structure for physical ESP32-S3 development.

Both versions implement the same reaction time tester logic.
