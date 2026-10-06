# ALARM CLOCK

## Operating System and System Programming

### Project Description

Alarm Clock is a Linux-based system programming project that allows users
to schedule an alarm at a specified time.

The application provides a simple command-line interface through which
the user can:

- Set an alarm
- View the scheduled alarm
- Cancel an active alarm
- View the current system time
- Exit the application

The project demonstrates operating system and system programming concepts
using Linux and POSIX APIs.

---

## Objectives

The main objectives of the project are:

1. To create a Linux-based alarm scheduling application.
2. To demonstrate process management using `fork()`.
3. To demonstrate signal handling.
4. To use POSIX timers for time-based scheduling.
5. To monitor and display the system time.
6. To provide a simple command-line interface.
7. To demonstrate communication between the parent process and alarm process.

---

## Features

### 1. Set Alarm

The user can enter an alarm time in:

    HH:MM

format.

The program creates a separate child process to manage the alarm.

### 2. View Alarm

The user can view the currently scheduled alarm and its status.

### 3. Cancel Alarm

The user can cancel an active alarm before it rings.

### 4. View Current Time

The program displays the current system date and time.

### 5. Alarm Notification

When the scheduled time is reached, the alarm process receives a
signal from the POSIX timer and displays:

    ALARM RINGING!

---

## Technologies Used

- C Programming
- Linux / Ubuntu
- GCC Compiler
- POSIX APIs
- Linux Processes
- Linux Signals
- POSIX Timers
- System Time APIs

---

## Operating System Concepts Demonstrated

### Process Management

The program uses:

    fork()

to create a child process.

The parent process manages the menu while the child process manages
the scheduled alarm.

### Signal Handling

Signals are used to notify and control the alarm process.

The project uses:

- `SIGUSR1` — used when the timer expires.
- `SIGTERM` — used to cancel the alarm process.

### POSIX Timer

The program uses:

    timer_create()

and

    timer_settime()

to create and schedule the alarm timer.

### Time Management

The program uses Linux/POSIX time functions including:

- `time()`
- `localtime()`
- `mktime()`
- `strftime()`

These functions are used to obtain, calculate and display time.

### Process Synchronization / Management

The parent process uses:

    waitpid()

to monitor and wait for the alarm child process when required.

---

## Program Flow

1. Start the Alarm Clock.
2. Display the menu.
3. User selects an operation.
4. If the user selects Set Alarm:
   - Read the alarm time.
   - Validate the time.
   - Create a child process using `fork()`.
   - Create a POSIX timer.
   - Wait for the timer signal.
5. When the scheduled time is reached:
   - The POSIX timer generates `SIGUSR1`.
   - The signal handler marks the alarm as triggered.
   - The alarm notification is displayed.
6. If the user selects Cancel Alarm:
   - The parent sends `SIGTERM` to the alarm process.
   - The alarm process terminates.
7. The menu continues until the user selects Exit.

---

## Compilation

Open Ubuntu/Linux terminal and go to the project directory:

    cd ~/alarm_clock

Compile the program:

    gcc alarm_clock.c -o alarm_clock

---

## Running the Project

The recommended launcher is:

    ./run_alarm.sh

This opens the Alarm Clock in a separate terminal window.

The program can also be run directly using:

    ./alarm_clock

---

## Input Validation

The program checks whether the entered alarm time is valid.

Examples of invalid input:

    25:70
    hello

The program displays an error message instead of crashing.

---

## Testing

The following test cases were performed:

| Test Case | Expected Result |
|-----------|-----------------|
| Set valid alarm | Alarm is created |
| Wait until alarm time | Alarm notification appears |
| View active alarm | Scheduled alarm is displayed |
| Cancel active alarm | Alarm process is cancelled |
| Cancel without alarm | Appropriate message displayed |
| View current time | Current system time displayed |
| Enter invalid time | Error message displayed |
| Enter text instead of time | Error message displayed |
| Set second alarm while one is active | Second alarm is rejected |
| Run launcher | Alarm Clock opens in separate terminal |

---

## Hardware Requirements

- Computer or Laptop
- Minimum 4 GB RAM
- 1 GHz or higher processor
- Speaker for alarm notification

## Software Requirements

- Ubuntu/Linux
- GCC Compiler
- POSIX libraries
- VS Code / Vim or another text editor

---

## Conclusion

The Alarm Clock project demonstrates how operating system and system
programming concepts can be combined to build a practical Linux
application.

The project demonstrates process management, signal handling,
POSIX timers, system time management and command-line interaction.

The application provides a simple mechanism for setting, viewing,
cancelling and triggering alarms.
