// Отрисовка картинок с карты памяти
void drawSdJpeg(const char *filename, int xpos, int ypos) {
  // Open the named file (the Jpeg decoder library will close it)
  File jpegFile = SD.open(filename, FILE_READ);  // or, file handle reference for SD library
  if (!jpegFile) {
    return;
  }

  // Use one of the following methods to initialise the decoder:
  bool decoded = JpegDec.decodeSdFile(jpegFile);  // Pass the SD file handle to the decoder,
  // bool decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)

  if (decoded) {
    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
    jpegFile.close();
  }
}
void jpegRender(int xpos, int ypos) {
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  bool swapBytes = tft.getSwapBytes();
  tft.setSwapBytes(true);

  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
  uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

  // save the coordinate of the right and bottom edges to assist image cropping to the screen size
  max_x += xpos;
  max_y += ypos;

  uint16_t* pixelBuffer = (uint16_t*)malloc(mcu_w * mcu_h * sizeof(uint16_t));
  if (pixelBuffer == nullptr) {
    // Memory allocation failed
    tft.setSwapBytes(swapBytes);
    return;
  }

  // Fetch data from the file, decode and display
  while (JpegDec.read()) {
    pImg = JpegDec.pImage;

    // Calculate coordinates of top left corner of current MCU
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // Calculate the width and height of the current MCU block
    uint32_t win_w = (mcu_x + mcu_w <= max_x) ? mcu_w : min_w;
    uint32_t win_h = (mcu_y + mcu_h <= max_y) ? mcu_h : min_h;

    // Copy pixels into the buffer
    for (int y = 0; y < win_h; y++) {
      for (int x = 0; x < win_w; x++) {
        pixelBuffer[y * win_w + x] = pImg[y * mcu_w + x];
      }
    }

    // Draw image MCU block only if it will fit on the screen
    if ((mcu_x + win_w) <= tft.width() && (mcu_y + win_h) <= tft.height()) {
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pixelBuffer);
    } else if ((mcu_y + win_h) >= tft.height()) {
      JpegDec.abort();  // Image has run off bottom of screen so abort decoding
    }
  }

  free(pixelBuffer);
  tft.setSwapBytes(swapBytes);
}
