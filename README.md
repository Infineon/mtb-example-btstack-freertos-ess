# Bluetooth&reg; LE Environmental Sensing Service

This code example demonstrates the implementation of a simple Bluetooth&reg; LE environmental sensing profile. It shows how to send environmental parameters like temperature by sending simulated temperature values over the Bluetooth&reg; LE connection using the Infineon&reg; AIROC&trade; WiFi & Bluetooth&reg; combo SoCs supported in the SDK. instructions provided in this document are applicable for all supported kits in the ModusToolbox™ software environment.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-anycloud-ble-ess)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzAyOTQiLCJTcGVjIE51bWJlciI6IjAwMi0zMDI5NCIsIkRvYyBUaXRsZSI6IkJsdWV0b290aCZyZWc7IExFIEVudmlyb25tZW50YWwgU2Vuc2luZyBTZXJ2aWNlIiwicmlkIjoicmFubSIsIkRvYyB2ZXJzaW9uIjoiMy4xLjAiLCJEb2MgTGFuZ3VhZ2UiOiJFbmdsaXNoIiwiRG9jIERpdmlzaW9uIjoiTUNEIiwiRG9jIEJVIjoiSUNXIiwiRG9jIEZhbWlseSI6IlBTT0MifQ==)

## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) v2.4
- Programming language: C
- Board support package (BSP) minimum required version: 3.0.0
- Associated parts: [PSoC&trade; 6 MCU](http://www.cypress.com/PSoC6) parts with AIROC™ CYW43012Wi-Fi & Bluetooth® combo chip, AIROC™ CYW43012Wi-Fi & Bluetooth® combo chip

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® embedded compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`
- Arm&reg; compiler v6.13 (`ARM`)
- IAR C/C++ compiler v8.42.2 (`IAR`)

## Supported kits (make variable 'TARGET')

- [PSoC&trade; 6 Wi-Fi Bluetooth® prototyping kit](https://www.cypress.com/CY8CPROTO-062-4343W) (`CY8CPROTO-062-4343W`) - Default value of `TARGET`
- [PSoC&trade; 6 Wi-Fi Bluetooth® prototyping kit](https://www.cypress.com/CY8CPROTO-062S3-4343W) (`CY8CPROTO-062S3-4343W`)
- [PSoC&trade; 62S2 Wi-Fi Bluetooth&reg; pioneer kit](https://www.cypress.com/CY8CKIT-062S2-43012) (`CY8CKIT-062S2-43012`)
- [PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; pioneer kit](https://www.cypress.com/CYW9P62S1-43012EVB-01) (`CYW9P62S1-43012EVB-01`)
- [PSoC&trade; 6 Wi-Fi Bluetooth&reg; pioneer kit](https://www.cypress.com/CY8CKIT-062-WiFi-BT) (`C8CKIT-062-WIFI-BT`)
- [PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; pioneer kit](https://www.cypress.com/CYW9P62S1-43438EVB-01) (`CYW9P62S1-43438EVB-01`)
- [PSoC 64 "Secure Boot" Wi-Fi Bluetooth pioneer kit](https://www.cypress.com/CY8CKIT-064B0S2-4343W) (`CY8CKIT-064B0S2-4343W`)
- [PSoC&trade; 62S2 evaluation kit](https://www.cypress.com/CY8CEVAL-062S2) (`CY8CEVAL-062S2`, `CY8CEVAL-062S2-LAI-4373M2`, `CY8CEVAL-062S2-MUR-43439M2`)

## Hardware setup

This example uses the kit’s default configuration. See the respective kit guide to ensure that the kit is configured correctly.

**Note:** The PSoC&trade; 6 Wi-Fi Bluetooth&reg; pioneer kit (CY8CKIT-062-WIFI-BT) ships with KitProg2 installed. The ModusToolbox&trade; software requires KitProg3. Before using this code example, make sure that the board is upgraded to KitProg3. The tool and instructions are available in the [Firmware Loader](https://github.com/Infineon/Firmware-loader) GitHub repository. If you do not upgrade, you will see an error such as "unable to find CMSIS-DAP device" or "KitProg firmware is out of date".

## Software setup

This code example consists of two parts: a GAP Peripheral and a GAP Central.

For the Central, download and install the CySmart app for [iOS](https://itunes.apple.com/us/app/cysmart/id928939093?mt=8) or [Android](https://play.google.com/store/apps/details?id=com.cypress.cysmart&hl=en).

You can also use the [CySmart host emulation tool](http://www.cypress.com/go/cysmart) Windows PC application if you have access to the [CY5677 CySmart Bluetooth&reg; LE 4.2 USB Dongle](http://www.cypress.com/documentation/development-kitsboards/cy5677-cysmart-bluetooth-low-energy-ble-42-usb-dongle). You can also use other Android or iOS apps that support the Environmental Sensing service.

Scan the following QR codes from your mobile phone to download the CySmart app.

![AppQR](./images/qr.png)

Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en).

## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](http://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. Optionally, change the suggested **New Application Name**.

5. The **Application(s) Root Path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) Root Path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; software provides the Project Creator as both a GUI tool and a command line tool, "project-creator-cli". The CLI tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the "project-creator-cli" tool. On Windows, use the command line "modus-shell" program provided in the ModusToolbox&trade; software installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; software tools. You can access it by typing `modus-shell` in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

The following table lists the arguments for this tool:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the `<id>` field of the [BSP](https://github.com/cypresssemiconductorco?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the `<id>` field of the [CE](https://github.com/cypresssemiconductorco?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional


The following example will clone the "[Bluetooth&reg; LE Environmental Sensing Service](https://github.com/Infineon/mtb-example-anycloud-ble-ess)" application with the desired name "mtb-example-anycloud-ble-ess" configured for the *CY8CPROTO-062-4343W* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CY8CPROTO-062-4343W --app-id mtb-example-anycloud-ble-ess --user-app-name mtb-example-anycloud-ble-ess --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For more details, see the "Project creator tools" section of the [ModusToolbox&trade; software user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [Project Creator](http://www.cypress.com/ModusToolboxProjectCreator) tool:**

   1. Launch Project Creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose Board Support Package** screen, select the BSP and click **Next**.

   3. In the **Select Application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

## Operation
1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. Use your favorite serial terminal application and connect to the KitProg3 COM port. Configure the terminal application to access the serial port using the following settings.

   Baud rate: 115200 bps; Data: 8 bits; Parity: None; Stop: 1 bit; Flow control: None; New line for receive data: Line Feed(LF) or Auto setting

3. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE for ModusToolbox&trade; software</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>

   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain and target are specified in the application's Makefile but you can override those values manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=CY8CPROTO-062-4343W TOOLCHAIN=GCC_ARM
      ```
   </details>

   **Note**: Before building the application, ensure that the *deps* folder contains the BSP file (*TARGET_xxx.lib*) corresponding to the TARGET. Execute the `make getlibs` command to fetch the BSP contents before building the application.

   After programming, the application starts automatically. Observe the messages on the UART terminal, and wait for the device to make all the required connections.

6. To test using the CySmart mobile app, do the following (see equivalent CySmart app screenshots in Figure 1):

    1. Turn ON Bluetooth&reg; on your Android or iOS device.

    2. Launch the CySmart app.

    3. Press the reset switch on the supported kit to start sending advertisements.

    4. Swipe down on the CySmart app home screen to start scanning for Bluetooth&reg; LE Peripherals; your device ("Thermistor") appears in the CySmart app home screen. Select your device to establish a Bluetooth&reg; LE connection. Once the connection is established, LED1 changes from OFF state to always ON state.

    5. Select the 'Environment Sensing' profile from the carousel view.

    6. Select an 'GATT DB'  on the 'Environmental Profile' screen.

    7. Select **Environmental Sensing Service** from the list and tap **Notify**.

        The temperature values will be updated every 5 seconds in different representations such as ASCII and HEX.


    **Figure 1. Testing with the CySmart app on android**

    ![](images/fig1.cysmart.png)

8.  Use the KitProg3 COM port to view the Bluetooth&reg; stack and application trace messages in the terminal window.

    **Figure 2. Log messages on KitProg3 COM port**

    ![](./images/fig2.logmessages.png)

## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For more details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; software User Guide](https://www.cypress.com/MTBEclipseIDEUserGuide).

**Note:** **(Only while debugging)** On the CM4 CPU, some code in `main()` may execute before the debugger halts at the beginning of `main()`. This means that some code executes twice - once before the debugger stops execution, and again after the debugger resets the program counter to the beginning of `main()`. See [KBA231071](https://community.cypress.com/docs/DOC-21143) to learn about this and for the workaround. *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*.

## Design and implementation

### Resources and settings

This section explains the ModusToolbox&trade; software resources and their configuration as used in this code example. Note that all the configuration explained in this section has already been done in the code example. ModusToolbox&trade; software stores the device configuration settings of the application in the *design.modus* file and Bluetooth&reg; configuration settings of the application in the *design.cybt* file. This file is used by the graphical configurators, which generate the configuration firmware. This firmware is stored in the application’s *GeneratedSource* folder.

- **Device configurator:** Used to enable/configure the peripherals and the pins used in the application. For detailed information on how to use it, see the
[Device configurator guide](https://www.cypress.com/ModusToolboxDeviceConfig).

- **Bluetooth&reg; configurator:** Used for generating/modifying the Bluetooth&reg; LE GATT database. For detailed information on how to use it, see the
[Bluetooth&reg; configurator Guide](https://www.cypress.com/ModusToolboxBluetooth LEConfig).

This code example is written in a way that is generic across multiple devices and platforms. The code example application runs on the Arm® Cortex® core of the PSoC&trade; 6 MCU device. The functionality of the code example will remain the same on all the supported kits; the application was developed using the ModusToolbox &trade; software.

This application demonstrates the Bluetooth&reg; LE peripheral capability of the supported kit. The simulated temperature value is sent over Bluetooth&reg; LE to a Central device, and to the UART as debug trace messages. This project demonstrates the following features:

- Bluetooth&reg; LE Environment Sensing Service (ESS) – GATT Read and Notify functionality
- Debug trace messages
- Connection with one Central device
- Connection status indication through LED

The project consists of the following files:

**Table 1. Important user-application-related source files**

|**File name**|**Comments**|
|-----------------------------------|-------------------------------------------------------|
|*main.c* | Contains the `main()` function, which is the entry point for execution of the user application code after device startup.|
|*cycfg_bt_settings.c, cycfg_bt_settings.h* |    Contain the runtime Bluetooth&reg; stack configuration parameters such as device name and  advertisement/ connection settings. Note that the name that the device uses for advertising (“Thermistor”) is defined in *app_bt_cfg.c*.|
|*app_bt_gatt_handler.c, app_bt_gatt_handler.h*|Contain the code for the Bluetooth&reg; stack GATT event handler functions. |
|*cycfg_gatt_db.c, cycfg_gatt_db.h*|    Contain the GATT database information generated using the Bluetooth&reg; configurator tool. These files reside in the *GeneratedSource* folder under the application folder.|

#### Flowchart

**Figure 3. Bluetooth&reg; LE environmental sensing service application**

![](images/fig3.flowchart.png)

This flowchart provides an overview of the Environmental Sensing Profile application. The Environmental Sensing Profile in this application consists of one of the Bluetooth&reg; SIG-defined services, namely the Environment Sensing Service (ESS). This project implements only the Temperature characteristic from the Environmental Sensing service. This characteristic supports notification and read operations, which allow the GATT Server to send data to the connected GATT Client device whenever new data is available, and a read to be performed from the GATT Client device.

The Bluetooth&reg; configurator provided by ModusToolbox&trade; software makes it easier to design and implement the GATT DB. The Bluetooth&reg; configurator generates the *cycfg_gatt_db.c* and *cycfg_gatt_db.h* files. All Environmental Sensing profile-related variables and functions are contained in these files. When the GATT DB is initialized after Bluetooth&reg; stack initialization, all profile-related values will be ready to be advertised to the Central device. See the *ModusToolbox&trade; software Bluetooth&reg; configurator guide* for details.

The code example generates dummy temperature values between 20 degree and 30 degree celsius. Every 5 seconds, the temperature varies by 1 degree celsius. A timer callback gives this simulated temperature value, which is then sent every 5 seconds to the Central device when connected and GATT notifications are enabled by the Central.

When the Peripheral device is connected, LED1 will be ON; when it is disconnected, LED1 will be OFF. To turn the LED ON and OFF, generic GPIO functions are used to drive the output pin HIGH or LOW. The LEDs present in the supported kits are active LOW LEDs, which means that the LED turns ON when the GPIO is driven LOW.

The *app_bt_gatt_handler.c* and *app_bt_gatt_handler.h* files handle the functionality of GATT callbacks from the Central device. On receiving a connection request, the Bluetooth&reg; stack gives a GATT event to the application of `wiced_bt_gatt_evt_t` type.

For example, the LED toggle functionality is implemented in the GATT connection callback. On a disconnection event, the code resets the Client Characteristic Configuration Descriptor (CCCD) value so that on a reconnect event, notifications will be disabled.

In *cycfg_bt_settings.c,* all the runtime Bluetooth&reg; stack configuration parameters are defined;  these will be initialized during Bluetooth&reg; stack initialization. Some of the configurations include the device name, connection interval, advertisement interval, advertisement channels to use, number of client connections, and maximum transmission unit (MTU). You also have the flexibility to configure the buffer pool size, which helps in optimizing the memory and transmission rate depending on the application use case.

The project also contains *app_bt_utils.c* and *app_bt_utils.h* which provide APIs to see meaningful messages in debug logs in the debug UART. Most of the status messages in Bluetooth&reg; stack are represented in enumerated values. These functions allow you to view the respective strings instead of the enumerated values.

This application uses a modified FreeRTOSConfig.h file to work.
we change the configTIMER_TASK_PRIORITY to 6 for the timer task and configTIMER_TASK_STACK_DEPTH to 256. These are the two minor modifications to default FreeRTOSConfig.h

## Related resources

| Application Notes                                            |                                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [AN228571](https://www.cypress.com/AN228571) – Getting started with PSoC&trade; 6 MCU on ModusToolbox&trade; software | Describes PSoC&trade; 6 MCU devices and how to build your first application with ModusToolbox&trade; software |
| [AN221774](https://www.cypress.com/AN221774) – Getting started with PSoC&trade; 6 MCU on PSoC&trade; Creator | Describes PSoC&trade; 6 MCU devices and how to build your first application with PSoC&trade; Creator |
| [AN210781](https://www.cypress.com/AN210781) – Getting started with PSoC&trade; 6 MCU with Bluetooth&reg; Low Energy connectivity on PSoC&trade; Creator | Describes PSoC&trade; 6 MCU with Bluetooth&reg; LE connectivity devices and how to build your first application with PSoC&trade; Creator |
| [AN215656](https://www.cypress.com/AN215656) – PSoC&trade; 6 MCU: dual-CPU system design | Describes the dual-CPU architecture in PSoC&trade; 6 MCU, and shows how to build a simple dual-CPU design |
| **Code examples**                                            |                                                              |
| [Using ModusToolbox&trade; software](https://github.com/cypresssemiconductorco/Code-Examples-for-ModusToolbox-Software) | [Using PSoC&trade; Creator](https://www.cypress.com/documentation/code-examples/psoc-6-mcu-code-examples) |
| **Device documentation**                                     |                                                              |
| [PSoC&trade; 6 MCU datasheets](https://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A575&f[2]=field_related_products%3A114026) | [PSoC&trade; 6 technical reference manuals](https://www.cypress.com/search/all/PSoC%206%20Technical%20Reference%20Manual?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A583) |
| **Development kits**                                         |Visit www.cypress.com/microcontrollers-mcus-kits and use the options in the **Select your kit** section to filter kits by *Product family* or *Features*.                          |
| **Libraries**                                                 |                                                              |
| PSoC&trade; 6 peripheral driver library (PDL) and docs  | [mtb-pdl-cat1](https://github.com/Infineon/mtb-pdl-cat1) on GitHub |
| Hardware abstraction layer (HAL) library and docs    | [mtb-hal-cat1](https://github.com/Infineon/mtb-hal-cat1) on GitHub |
| RetargetIO - A utility library to retarget the standard input/output (STDIO) messages to a UART port | [retarget-io](https://github.com/Infineon/retarget-io) on GitHub |
| freeRTOS library and docs                                    | [freeRTOS](https://github.com/Infineon/freertos) on GitHub |
| **Middleware**                                               |                                                              |
| CapSense&trade; library and docs                                    | [capsense](https://github.com/Infineon/capsense) on GitHub |
| Links to all PSoC&trade; 6 MCU middleware                           | [psoc6-middleware](https://github.com/Infineon/modustoolbox-software) on GitHub |
| **Tools**                                                    |                                                              |
| [Eclipse IDE for ModusToolbox&trade; software](https://www.cypress.com/modustoolbox) | ModusToolbox&trade software is a collection of easy-to-use software and tools enabling rapid development with Infineon MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC&trade; Wi-Fi and Bluetooth® connectivity devices. |

## Other resources

Cypress&trade; provides a wealth of data at www.cypress.com to help you select the right device, and quickly and effectively integrate it into your design.

For PSoC&trade; 6 MCU devices, see [How to design with PSoC 6 MCU - KBA223067](https://community.cypress.com/docs/DOC-14644) in the Cypress&trade; community.

## Document history

Document title: *CE230294* - *Bluetooth&reg; LE Environmental Sensing Service*

| Version | Description of change |
| ------- | --------------------- |
| 1.0.0   | New code example      |
| 2.0.0   | Code Example updated to support ModusToolbox&trade; software v2.3.1 and it is not backward compatible with v2.3 and below|
| 3.0.0   | Added support for 43439 kit <br /> Updated BSP to 3.0.0|
| 3.1.0   | Code example updated|

------

![Banner](images/banner.png)

-------------------------------------------------------------------------------

© Cypress Semiconductor Corporation, 2019-2021. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product.  CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications.  To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document.  Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device.  You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device.  Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolBox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries.  For a more complete list of Cypress trademarks, visit cypress.com.  Other names and brands may be claimed as property of their respective owners.
