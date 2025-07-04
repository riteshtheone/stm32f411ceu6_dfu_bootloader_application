# STM32F411CEU6 (Black Pill) DFU Bootloader + Application

This project demonstrates a **USB DFU (Device Firmware Upgrade) bootloader** with a **sample user application** for the STM32F411CEU6 microcontroller, such as the **Black Pill** board.

It allows firmware upgrades over USB without needing ST-Link or toggling BOOT0 — perfect for production or remote updates.

---

## 🧭 How to Enter DFU Bootloader Mode

### ✅ Method (No BOOT0 Needed)
To trigger DFU mode on boot:

- **Step 1:** Press and hold the **Boot Button** (connected to PA0) available on the board.
- **Step 2:** Power on the board or press the reset button (while still holding the Boot Button).
- **Step 3:** After ~0.5 seconds, release the Boot Button.

> 🔄 The **LED on PC13** will blink rapidly (100ms interval) to indicate DFU mode is active.
> 
> ❌ If no valid application is found, the **LED on PC13** will double-blink every 1.5 seconds to indicate the application is not found.

---

## 🔌 Connection Methods

- **USB DFU Mode:**  
  Connect a USB data cable to the board's **USB Micro or Type-C port**.

- **No UART/Serial Needed:**  
  This bootloader does **not require BOOT0** or UART (PA9/PA10) usage.

---

## 🚀 Application Details (Important)

The user application is flashed to **0x08008000** onward.

### 📦 Linker Script (`application.ld`) — ⚠️ Do Not Ignore This
```ld
MEMORY
{
  RAM      (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  FLASH    (rx)  : ORIGIN = 0x08008000, LENGTH = 480K
}
```

> ⚠️ **Important:** This ensures the application does **not overwrite the bootloader** and has access to the full RAM.

---

## 💾 Flashing Firmware

Use one of the following tools that support USB DFU:

- ✅ [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
- ✅ `dfu-util` (Linux/macOS/WSL)

Flash your firmware binary (`.bin` or `.hex`) starting at address:
```
0x08008000
```

---

### ❎ Bootloader Internal Details (Can Be Ignored)

---

## 🧠 Bootloader Details

The bootloader is flashed to the **first 32KB of flash** (`0x08000000–0x08007FFF`).

### 📦 Linker Script (`bootloader.ld`)
```ld
MEMORY
{
  BOOTFLAG (xrw)  : ORIGIN = 0x20000000 , LENGTH = 0x8           /* 8 bytes for boot_flag */
  RAM      (xrw)  : ORIGIN = 0x20000008 , LENGTH = 128K - 0x8
  FLASH    (rx)   : ORIGIN = 0x08000000 , LENGTH = 32K
}
```

---

## 📁 Project Structure

```
stm32f411ceu6_dfu_bootloader_applicationer/
├── stm32_bootloader     # USB DFU Bootloader (starts at 0x08000000)
└── stm32_application    # User application (starts at 0x08008000)
```

Each project includes STM32CubeMX `.ioc`, `Makefile`, USB stack, HAL drivers, and build folders.

---

## 🛠️ Build & Flash

You can build, flash via DFU, and open a serial monitor using the included helper `Makefile` in each folder.

### 🧰 Example Helper `Makefile`
```make
# Build target (requires sub-make in ./Debug)
make all

# Flash using dfu-util (must have sudo or permissions)
make flash

# Open serial monitor (uses miniterm from pyserial)
make serial

# Clean build
make clean
```

Update this line in the Makefile to match your serial device:
```make
SERIAL := /dev/ttyUSB0
```

> 💡 **Tip:** Make sure `dfu-util` and `pyserial` are installed:
```bash
sudo apt install dfu-util
pip install pyserial
```

---

## ✅ How It Works

### 🔌 1. Power-Up / Reset
- Bootloader checks a **boot flag in RAM** to decide whether to enter DFU or jump to the application.

### 💻 2. DFU Mode (Firmware Update)
- If a DFU request is detected (via PA0 or host), bootloader enters **USB DFU mode**.
- While in DFU mode, **LED (PC13)** blinks at **100ms** interval.

### 🚀 3. Application Launch
- If no DFU request is detected and a **valid application** is found at `0x08008000`, it jumps to the app.
- If the application is **not valid**, **LED (PC13)** shows a **double-blink every 1.5 seconds**.

> 🛠️ *You can customize this logic in `main.c` — GPIO, UART, button triggers, etc.*

---

## 📦 Requirements

- STM32F411CEU6 (e.g., Black Pill board)
- USB cable
- ST-Link (for initial bootloader flash only)
- `dfu-util`
- Python 3 + `pyserial`

---

## 📄 License

This project is licensed under the [MIT License](./LICENSE). You're free to use, modify, and distribute.

---

## 💬 Need Help?

Open issues or discussions on the GitHub repository:  
👉 [https://github.com/riteshtheone/stm32f411ceu6_dfu_bootloader_application](https://github.com/riteshtheone/stm32f411ceu6_dfu_bootloader_application)

---
