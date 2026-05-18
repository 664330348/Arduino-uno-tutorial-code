# Arduino OLED (SSD1306) Function Notes (.md)

This is a Markdown reference sheet for common functions used with the Adafruit SSD1306 OLED library on Arduino UNO R3.

---

## 1. Screen Control

| Function           | Syntax                           | Description                                   |
|--------------------|----------------------------------|-----------------------------------------------|
| Clear Screen       | `display.clearDisplay();`        | Clears everything on the OLED screen buffer   |
| Update Display     | `display.display();`             | Sends all buffered changes to the OLED screen |
| Fill Screen        | `display.fillScreen(color);`     | Fills the entire screen with a color          |
| Invert Display ON  | `display.invertDisplay(true);`   | Inverts black and white colors                |
| Invert Display OFF | `display.invertDisplay(false);`  | Restores normal colors                        |
| Rotate Screen      | `display.setRotation(rotation);` | Sets screen rotation (0–3)                    |

---

## 2. Text Functions

| Function       | Syntax                         | Description                               |
|----------------|--------------------------------|-------------------------------------------|
| Set Cursor     | `display.setCursor(x, y);`     | Sets text starting position               |
| Set Text Size  | `display.setTextSize(size);`   | Sets text size (1, 2, 3...)               |
| Set Text Color | `display.setTextColor(color);` | Sets text color (usually `SSD1306_WHITE`) |
| Print Text     | `display.print("Text");`       | Prints text without newline               |
| Print Line     | `display.println("Text");`     | Prints text and moves to next line        |

---

## 3. Drawing Functions

| Function               | Syntax                                                 | Description                    |
|------------------------|--------------------------------------------------------|--------------------------------|
| Draw Pixel             | `display.drawPixel(x, y, color);`                      | Draws a single pixel           |
| Draw Line              | `display.drawLine(x1, y1, x2, y2, color);`             | Draws a line                   |
| Draw Rectangle         | `display.drawRect(x, y, w, h, color);`                 | Draws empty rectangle          |
| Fill Rectangle         | `display.fillRect(x, y, w, h, color);`                 | Draws filled rectangle         |
| Draw Circle            | `display.drawCircle(x, y, r, color);`                  | Draws empty circle             |
| Fill Circle            | `display.fillCircle(x, y, r, color);`                  | Draws filled circle            |
| Draw Triangle          | `display.drawTriangle(x1, y1, x2, y2, x3, y3, color);` | Draws empty triangle           |
| Fill Triangle          | `display.fillTriangle(x1, y1, x2, y2, x3, y3, color);` | Draws filled triangle          |
| Draw Rounded Rectangle | `display.drawRoundRect(x, y, w, h, r, color);`         | Draws rounded rectangle        |
| Fill Rounded Rectangle | `display.fillRoundRect(x, y, w, h, r, color);`         | Draws filled rounded rectangle |

---

## 4. Graphics / Bitmap

| Function    | Syntax                                           | Description           |
|-------------|--------------------------------------------------|-----------------------|
| Draw Bitmap | `display.drawBitmap(x, y, bitmap, w, h, color);` | Displays bitmap image |

---

## 5. Typical Usage Flow

```cpp
// 1. Clear buffer
display.clearDisplay();

// 2. Draw content
display.setCursor(0, 0);
display.print("Hello World");

// 3. Push to screen
display.display();
```

---

## 6. Notes

- OLED uses a **buffer system** → nothing shows until `display.display()` is called
- Always call `display.clearDisplay()` before drawing a new frame
- Coordinate system starts at **(0,0) top-left corner**
- Resolution is **128x64 pixels**