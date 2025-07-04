
# STM32F411CEU6 DFU Bootloader + Application

This project demonstrates a **USB DFU (Device Firmware Upgrade) bootloader** with a **sample user application** for the STM32F411CEU6 microcontroller, such as the **Black Pill** board.

It allows firmware upgrades over USB without needing ST-Link or toggling BOOT0 — perfect for production or remote updates.

---

## 📁 Project Structure

```
stm32f411ceu6_dfu_bootloader_applicationer/
├── stm32_bootloader     # USB DFU Bootloader (starts at 0x08000000)
└── stm32_application    # User application (starts at 0x08008000)
```

Each project includes STM32CubeMX `.ioc`, `makefile`, USB stack, HAL drivers, and build folders.

---

## 🧠 Bootloader Details

The bootloader is flashed to the **first 32KB of flash** (`0x08000000–0x08007FFF`).

### 📦 Linker script (bootloader `.ld`)
```ld
MEMORY
{
  BOOTFLAG (xrw)  : ORIGIN = 0x20000000 , LENGTH = 0x8           /* 8 bytes for boot_flag */
  RAM      (xrw)  : ORIGIN = 0x20000008 , LENGTH = 128K - 0x8
  FLASH    (rx)   : ORIGIN = 0x08000000 , LENGTH = 32K
}
```

- **BOOTFLAG** is an 8-byte RAM section used to pass DFU/app boot info.
- **FLASH** is limited to the first 32KB for the bootloader.

---

## 🚀 Application Details

The user application is flashed to **0x08008000** onward.

### 📦 Linker script (application `.ld`)
```ld
MEMORY
{
  RAM      (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  FLASH    (rx)  : ORIGIN = 0x08008000, LENGTH = 480K
}
```

- This ensures it does **not overwrite** the bootloader.
- RAM is shared fully (application does not touch boot flag logic).

---

## 🛠️ Build & Flash

You can build, flash via DFU, and open a serial monitor using the included helper `Makefile` in each folder.

### Example helper `Makefile`
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

> Make sure `dfu-util` and `pyserial` are installed:
```bash
sudo apt install dfu-util
pip install pyserial
```

---

## ✅ How It Works

1. On power-up/reset:
   - Bootloader checks a boot flag in RAM to decide whether to enter DFU or jump to the app.
2. On DFU request (from host or condition):
   - Bootloader stays in DFU mode and exposes the USB firmware update interface.
3. Otherwise:
   - Jumps to user app at `0x08008000`.

You can modify this logic in `main.c` of the bootloader to customize trigger methods (e.g. GPIO pin, UART command, button, etc.).

---

## 📦 Requirements

- STM32F411CEU6 (e.g., Black Pill board)
- USB cable
- ST-Link (for first bootloader flash only)
- `dfu-util`
- Python 3 + pyserial

---

## 📄 License

This project is MIT licensed. You're free to use, modify, and distribute.

---

## 💬 Need Help?

Feel free to open issues or discussions on the GitHub repository:
👉 [https://github.com/riteshtheone/stm32f411ceu6_dfu_bootloader_application](https://github.com/riteshtheone/stm32f411ceu6_dfu_bootloader_application)
