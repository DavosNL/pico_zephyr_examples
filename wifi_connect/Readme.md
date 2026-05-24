
    # WiFi Connect Example Pico WiFi Family

    This example demonstrates how to connect a Raspberry Pi Pico WiFi family to a WiFi network using Zephyr’s networking stack.

    To build this project successfully, two things are required:

    1. Infineon CYW43439 WiFi firmware blobs
    2. WiFi credentials passed from Windows into WSL


    ## WiFi Firmware

    Zephyr does not include the Infineon WiFi firmware by default.
    Before building this project, activate the Pico SDK Zephyr environment and fetch the required blobs:

    ```bash
    # Activate the Zephyr environment created by the Pico SDK wizard
    source ~/.pico-sdk/zephyr_workspace/venv/bin/activate

    # Download the Infineon WiFi firmware blobs
    west blobs fetch hal_infineon

    # Leaving the Zephyr environment
    deactivate
    ```

    ## WiFi‑credentials (WSL Setup)

    If you are developing inside WSL (Windows Subsystem for Linux) and compiling via VS Code, you must share your WiFi credentials from Windows to the WSL environment.

    ### 1. Set environment variables in Windows
    Open **PowerShell** on Windows and run the following commands (replace with your own WiFi details):

    ```powershell
    [Environment]::SetEnvironmentVariable("WIFI_SSID", "YourWifiName", "User")
    [Environment]::SetEnvironmentVariable("WIFI_PSK", "YourPassword", "User")
    ```

    ### 2. Share variables with WSL
    Tell Windows to pass these variables into WSL by configuring `WSLENV` with the `/u` flag (which flags them for UTF-8 translation):

    ```powershell
    [Environment]::SetEnvironmentVariable("WSLENV", "WIFI_SSID/u:WIFI_PSK/u", "User")
    ```

    **Restart VS Code completely** to apply the new environment variables.