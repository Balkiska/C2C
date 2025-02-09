# C2C : LD_PRELOAD Malware & C2 Server

**Table of Contents**  
- [Overview](#overview)  
- [Project Structure](#project-structure)  
- [Requirements](#requirements)  
- [Instructions](#instructions)  
  - [Victim Side (Malware)](#victim-side-malware)  
  - [Attacker Side (C2 Server)](#attacker-side-c2-server)  
- [Usage Example](#usage-example)  
- [Notes and Caveats](#notes-and-caveats)  
- [Troubleshooting](#troubleshooting)  
- [Disclaimer](#disclaimer)

---

## Overview
This project demonstrates a **malware** (LD_PRELOAD rootkit approach) that captures SSH credentials and hides log files on a **victim** machine, then sends stolen data to a **C2 server** via a **port-knocking mechanism**.

- **Victim side (VM)**: Builds and runs the malware, which hooks system calls to intercept credentials and block log access.  
- **Attacker side (C2)**: Listens on a port, waits for a valid port-knocking sequence, and manages incoming stolen credentials via a local database.

---

## Project Structure

- **Malware** (Victim):
  - `main.c`: Kills existing SSH, restarts SSH with LD_PRELOAD, performs port knocking, then connects to the C2.  
  - `hook.c`/`.h`: Hooks `write()` and `open()` to capture credentials and block log files.  
  - `hide.c`/`.h`: Clears or hides logs (optional).  
  - `connect.c`/`.h`: Connects to C2 and sends captured data.  
  - `port_knocking.c`/`.h`: Implements port knocking.  
  - `Makefile`: Builds both the `.so` and the main malware executable.

- **C2 Server** (Attacker):
  - `listener.c`/`.h`: Port knocking & connection handling.  
  - `database.c`/`.h`: Stores and retrieves credentials in a local file.  
  - `server_ui.c`/`.h`: Simple menu-based user interface for managing credentials.  
  - `main.c` (or `server.c`): Entry point for the C2 server, possibly creating threads for the listener and UI.  
  - `Makefile`: Builds the server.

---

## Requirements

**Victim (Malware VM)**  
- Linux with support for `LD_PRELOAD`.  
- A C compiler (e.g., `gcc`).  
- Permission to modify SSH if you want to intercept real system SSH traffic. Otherwise, run on a higher, non-privileged port.

**Attacker (C2 Server)**  
- Linux with `gcc` and POSIX threads (`-lpthread`).  
- (Optional) `ncurses` or other libraries if you want a more advanced interface.  
- May require `sudo` if binding to port 22.

---

## Instructions

### Victim Side (Malware)

1. **Obtain source** on the victim VM.  
2. **Build**  
   ``` make```
3. **Run** (possibly as root if binding to port 22)
   ```sudo ./malware```
This kills the old SSH, restarts with LD_PRELOAD=/path/to/malware.so, performs port knocking, and connects to the C2.
5. **Confirm** SSH is running with LD_PRELOAD by checking
   ``` ps aux | grep sshd```
6. Any SSH password typed in the system should get intercepted (write hook) and sent to the C2.

### Attacker Side (C2 Server)

1. **Obtain source** on the attacker machine.
2. **Build**
   ```make```
4. **Run** (on port 22, requires sudo or change the port)
   ```sudo ./c2_server```
6. The server waits for the correct port-knocking sequence. Once received, it unlocks the IP.
7. The interface (if threaded) will let you list, search, and delete credentials in credentials.bin.

## Usage Example

**Start the C2** on the attacker machine:
 ```sudo ./c2_server```
**Launch the malware** on the victim:
(Port knocking then occurs automatically if coded in perform_port_knocking())
**Check the C2**
  It shows messages like:
  ```
  Correct sequence received from x.x.x.x
  SSH port is unlocked for x.x.x.x
  New client connected.
  ```
Any captured data is now visible in the logs or in credentials.bin.

## Notes and Caveats

**LD_PRELOAD** can be blocked by certain security frameworks (SELinux, AppArmor).
Binding to port 22 usually requires root privileges; use a high port for non-privileged operation.
The hooking and port knocking examples are minimal and for educational use.
**Do not** deploy this on systems you do not own or have explicit permission to test.

## Troubleshooting

**Cannot bind port 22**: run as ```root```, or pick ```2222``` instead.
**Credentials not appearing**: ensure your hooking logic actually intercepts the keyword “password” or adapt to your environment.
**No port knocking success**: check that the client’s ```perform_port_knocking()``` and the server’s sequence match (same ports, same order).

## Disclaimer

This code is intended for **educational and testing purposes** in a controlled environment. Unauthorized usage on production or third-party systems is illegal and unethical. Use responsibly.
