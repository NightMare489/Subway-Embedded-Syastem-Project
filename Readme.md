## Table of Contents

[![Awesome](https://awesome.re/badge.svg)](https://awesome.re)
- [Introduction](#introduction)
  - [Project Overview](#project-overview)
  - [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Hardware Setup](#hardware-setup)
- [How to Play](#how-to-play)
- [Code Explanation](#code-explanation)
  - [Abstract Overview](#abstract-overview)
  - [Main Components](#main-components)
  - [Interrupts and Timers](#interrupts-and-timers)
  - [Game Logic](#game-logic)
- [Images](#images)
- [Authors](#authors)
- [Acknowledgments](#acknowledgments)
- [References](#references)
- [License](#license)

---

## Introduction

This project is an embedded system implementation of a simple "Subway" game developed on an **ATmega32** microcontroller using **Atmel Studio** and simulated with **Proteus Design Suite**. The game features a player character navigating through obstacles on an LCD screen. It was developed as part of the **CS427 Embedded Systems Programming** course at the **Arab Academy for Science, Technology & Maritime Transport (AASTMT)**.

### Project Overview

In the Subway game, the player controls a character represented by **'A'** on a 16x2 LCD display, avoiding obstacles represented by **'B'** that move across the screen. The objective is to survive as long as possible by switching the position of the player between the two rows of the LCD to avoid collisions with obstacles. The game keeps track of the score, which increases as the player successfully avoids obstacles.

### Features

- Real-time obstacle generation using hardware timers and pseudo-random number generation.
- Interrupt-driven input handling for responsive controls.
- LCD display interface for game visualization.
- Scoring system to track player progress.
- Simple and intuitive gameplay mechanics.

---

## Getting Started

### Prerequisites

To set up and run this project, you will need the following software and hardware:

#### Software

- **Atmel Studio 7**: Integrated Development Environment (IDE) for writing and compiling the AVR C code.
  - Download: [Atmel Studio](https://www.microchip.com/en-us/tools-resources/archives/avr-sam-mcus)
- **Proteus Design Suite**: For simulating the microcontroller circuit and LCD display.
  - Download: [Proteus Design Suite](https://www.labcenter.com/downloads/)

#### Hardware

- **ATmega32 Microcontroller**
- **16x2 LCD Display (LM016L)**
- **Push Button**
- **220Ω Resistor**
- **Breadboard and Jumper Wires**
- **Power Supply (e.g., 5V DC)**

### Installation

1. **Clone the Repository**

   ```bash
   git clone https://github.com/NightMare489/Subway-Embedded-Syastem-Project.git
   ```
2.  Open the Project in Atmel Studio

    -   Launch Atmel Studio.
    -   Navigate to `File` > `Open` > `Project/Solution`.
    -   Open the project file located in the cloned repository.
2.  Build the Project

    -   In Atmel Studio, build the project by clicking on `Build` > `Build Solution` or pressing `F7`.
    -   Ensure there are no compilation errors.
4.  Set Up Proteus Simulation

    -   Open the Proteus Design Suite.
    -   Create a new project and design the circuit as per the hardware setup instructions.
    -   Upload the compiled `.hex` file from Atmel Studio to the ATmega32 microcontroller in Proteus.

### Hardware Setup

#### Circuit Components and Connections

-   ATmega32 Microcontroller
-   16x2 LCD Display (LM016L)
-   Push Button connected to INT0 (Pin PD2) with a pull-down resistor.
-   220Ω Resistor for the push button.
-   Power Supply and Ground Connections

#### LCD Connections

-   Data Pins (DB0 - DB7): Connect to PORTB pins (PB0 - PB7).
-   Control Pins:
    -   RS (Register Select): Connect to PA0.
    -   EN (Enable): Connect to PA1.
-   VSS, VDD, VEE:
    -   Connect VSS to ground.
    -   Connect VDD to +5V.
    -   Connect VEE through a potentiometer to adjust contrast.

#### Push Button Connection

-   Connect one terminal of the push button to PD2 (INT0).
-   Connect the other terminal to ground through a 220Ω resistor.
-   Optionally, connect a pull-up resistor between PD2 and +5V to ensure a default HIGH state.

#### Power Supply

-   Ensure all VCC pins of the ATmega32 and LCD are connected to +5V.
-   Ensure all GND pins are connected to ground.

#### Note:

-   For detailed pin configurations, refer to the ATmega32 and LM016L datasheets.

Hardware Connection Diagram:

![Hardware Setup Diagram](https://github.com/user-attachments/assets/bf19afa7-f8bf-4fc2-bdf0-83c1c83caf31)

* * * * *

How to Play
-----------

-   Objective: Avoid colliding with the obstacles (represented by 'B') by switching the player's position between the top and bottom rows.
-   Controls:
    -   Push Button: Pressing the button connected to INT0 (PD2) toggles the player's position between the top and bottom rows.
-   Gameplay:
    -   Obstacles appear randomly on the right side of the screen and move left towards the player.
    -   The player starts on the top row at a fixed column position.
    -   Each time an obstacle is avoided, the score increases.
    -   The game ends when the player collides with an obstacle.
-   Scoring:
    -   The score is displayed at the end of the game as "You Lost: *score*".

* * * * *

Code Explanation
----------------

### Abstract Overview

The game code is written in C for the ATmega32 microcontroller. It uses the AVR libraries to control the LCD display, handle interrupts, and manage timers. The game logic is implemented using interrupts for responsive gameplay and timers for obstacle generation and movement.

### Main Components

-   LCD Display Functions: Functions to initialize the LCD, send commands, display characters, and strings.
-   Game Variables:
    -   `Bird_pos`: The fixed horizontal position of the player character.
    -   `Bird_Row`: The current row (top or bottom) of the player character.
    -   `obs_x[]`, `obs_y[]`: Arrays storing the positions of obstacles.
    -   `lcd_Buffer`: A 2D array representing the LCD content buffer.
    -   `rewrite_Buffer`: A 2D array to track which positions need to be updated on the LCD.
-   Interrupt Service Routines (ISRs):
    -   `TIMER1_OVF_vect`: Generates new obstacles at intervals.
    -   `TIMER0_OVF_vect`: Moves obstacles across the screen and checks for collisions.
    -   `INT0_vect`: Handles the push button input to toggle the player's position.

### Interrupts and Timers

-   Timer1 Overflow Interrupt:
    -   Used to generate new obstacles randomly.
    -   A pseudo-random number generator creates obstacles on the top or bottom row.
-   Timer0 Overflow Interrupt:
    -   Moves existing obstacles one position to the left.
    -   Checks for collision between the player and obstacles.
    -   Updates the score when obstacles are avoided.
-   External Interrupt INT0:
    -   Triggered by the push button.
    -   Toggles the player's position between the two rows.
    -   Checks for immediate collision upon movement.

### Game Logic

-   Initialization:
    -   The game initializes the LCD and sets up the initial game state.
-   Obstacle Generation:
    -   Obstacles are generated at random intervals and positions.
-   Obstacle Movement:
    -   Obstacles move left across the screen at each timer tick.
    -   When obstacles reach the leftmost position, they reset or are removed.
-   Collision Detection:
    -   Collision is detected if an obstacle occupies the same position as the player.
    -   The game ends when a collision is detected.
-   Score Keeping:
    -   The score increments each time an obstacle is successfully avoided.
    -   The final score is displayed when the game ends.

* * * * *

Images
------

Below are some screenshots of the game and hardware setup:

### Game Over Screen

![Game Over - Final Score](https://github.com/user-attachments/assets/39893528-10c2-403b-bb99-f79c52016a4e)

*The LCD display shows "You Lost: 21", indicating the final score.*

### Gameplay Screenshot 1

![Gameplay with Player on Top Lane](https://github.com/user-attachments/assets/f66554d9-4f34-4906-bf3c-7004f960f15d)

*The player 'A' is on the top lane with three obstacles 'B' on the screen.*

### Hardware Setup

![Hardware Setup](https://github.com/user-attachments/assets/034629ec-966b-4a5d-b430-9050803ec36e)

*The hardware connection of the ATmega32 microcontroller with the LCD display and push button.*

### Gameplay Screenshot 2

![Gameplay with Player on Bottom Lane](https://github.com/user-attachments/assets/c79f08d6-c51f-41aa-a8da-aa97d757aab2)

*The player 'A' is on the bottom lane with four obstacles 'B', one of which has passed the player.*

*Please place your images in the `images` folder with the filenames:*

-   `game_over.png`
-   `gameplay_top_lane.png`
-   `hardware_setup.png`
-   `gameplay_bottom_lane.png`

* * * * *

Authors
-------

-   Yasser Mohamed Shaban
-   Abdulrahman Ahmed Tolba

* * * * *

Acknowledgments
---------------

-   Dr. Abdallah Hassan - Course Supervisor
-   Eng. Fatma Tarek - Teaching Assistant

* * * * *

References
----------

-   Atmel Studio: [Download and Documentation](https://www.microchip.com/en-us/tools-resources/archives/avr-sam-mcus)
-   Proteus Design Suite: [Official Website](https://www.labcenter.com/)
-   ATmega32 Datasheet: [Microchip Technology](https://ww1.microchip.com/downloads/en/DeviceDoc/2503S.pdf)
-   16x2 LCD Datasheet (LM016L): [Datasheet](https://www.mouser.com/datasheet/2/126/LCD-16x2-Character-Module-Datasheet-925964.pdf)
-   AVR Libc Reference Manual: [Documentation](https://www.nongnu.org/avr-libc/user-manual/modules.html)

* * * * *

License
-------

This project is for educational purposes and does not have a specific license. Please contact the authors for any inquiries regarding usage.

* * * * *

Note: If you have any questions or need further assistance setting up or understanding the project, please feel free to contact us. 🫡
