# 🔥 OLED Bitmap Editor (Jupyter Widget Tool)

A Jupyter Notebook-based tool for converting images into **Arduino SSD1306 OLED bitmap data**.

It provides an interactive UI for uploading images, adjusting parameters, previewing results, and exporting ready-to-use Arduino code.

---

# ✨ Features

- 🖼 Image upload (PNG / JPG)
- 🔄 Image rotation (0°, 90°, 180°, 270°)
- 🎚 Threshold-based black/white conversion
- 🔁 Invert (0/1 flip)
- 📍 X/Y offset positioning
- 📺 OLED canvas preview (binary display)
- 📦 Generate Arduino `PROGMEM bitmap`
- 📋 One-click copy for Arduino code

---

# 🚀 Installation

Install required dependencies:

```bash
pip install numpy pillow ipywidgets