# Building C/C++ Barcode Reader with CMake

## Linux

1. Copy **libDynamsoftBarcodeReader.so** to **platforms\linux**.
2. Build and install the project:
    ```bash
    cmake .
    sudo cmake --build . --target install
    ```
3. Create a `license_file` that contains only the license string.
4. Run the app:
    ```
    BarcodeReader [image_file] [license_file]
    ```
