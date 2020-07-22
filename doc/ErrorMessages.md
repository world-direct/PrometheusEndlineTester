# Request Token

**>>Token successfully requested<<**
**Meaning** 
SUCCESS

**>>CURL error Couldn't resolve host name<<**
**Meaning** 
the hostname of the identity server cannot be resolved

**Possible Reasons** 
- No Internet connection
- No Connection to Word Direct Backend

**Possible Actions**
- try to override the PrometheusEndlineTester.ini with the default one given by world direct
- Check the connection to the internet
    ```
    ping google.com
    ```
- Try to ping the server defined in PrometheusEndlineTester.ini under identity --> server 
    ```
    ping https://identity-energy-solutions.world-direct.at
    ```

**>>HTTP error BAD REQUEST<<**
**Meaning** 
400 Bad Request response status code indicates that the server cannot or will not process the request due to something that is perceived to be a client error

**Possible Reasons** 
- the client secret defined in PrometheusEndlineTester.ini under identity --> secret is empty or wrong
- the client id defined in PrometheusEndlineTester.ini under identity --> client is empty or wrong
- the client scope defined in PrometheusEndlineTester.ini under identity --> secret is not empty or wrong

**Possible Actions**
- try to override the PrometheusEndlineTester.ini with the default one given by world direct
- request a valid secret from world direct 

# Download Firmware

**>>latest Firmware successfully downloaded<<**
**Meaning** 
SUCCESS

**>>HTTP error UNAUTHORIZED<<**
**Meaning**
401 Unauthorized client error status response code indicates that the request has not been applied because it lacks valid authentication credentials for the target resource.

**Possible Reasons** 
- request token failed(see above)
- the requested tokens lifetime has ended

**Possible Actions**
- request a new token with the request token action

**>>CURL error Couldn't resolve host name<<**
**Meaning** 
the hostname of the provisioning server cannot be resolved

**Possible Reasons** 
- No Internet connection
- No Connection to Word Direct Backend

**Possible Actions**
- try to override the PrometheusEndlineTester.ini with the default one given by world direct
- Check the connection to the internet
    ```
    ping google.com
    ```
- check the connection to the server defined in PrometheusEndlineTester.ini under provisioning --> server
    ```
    ping https://device-provisioning-energy-solutions.world-direct.at
    ```

**>>error could not open file: Firmware.hex<<**
**Meaning**
the file Firmware.hex could not be opened for writing

**Possible Reasons** 
- the file is open in a different program
- the file is currentl beeing downloaded 
- the file can not be created/read due to permission or path restrictions

**Possible Actions**
- close the program in which the file is opened( most likely STIM32 ST-LINK Utility)
- restart the PrometheusEndlineTester (to abbort all downloads and all references to the file)
- check if the path to the file exist and the current user can create a file there

# new Target

# Connect Programmer

**>>ST-LINK Connect successfully finished<<**
**Meaning** 
SUCCESS

**>>ST-LINK Connect returned with error: 2<<**
**Meaning**
Unable to connect to ST-LINK!

**Possible Reasons** 
- the ST-Link is not connected to the usb
- the ST-Link is not connected to the controller
- the controller is not powered

**Possible Actions**
- check all ST-Link connections
- try to unplug and replug the usb of the ST-Link
- try to connect with the ST-Link Utility UI

**>>any other error<<**
if another error occurs please refere to the st-link utiliy user manual

# Program Test
**>>ST-LINK Program successfully finished<<**
**Meaning** 
SUCCESS

**>>any other error<<**
if another error occurs please refere to the st-link utiliy user manual

# Reset Target
**>>any other error<<**
if another error occurs please refere to the st-link utiliy user manual

# Connect Serial

# Send Sync

# Program Firmware
**>>any other error<<**
if another error occurs please refere to the st-link utiliy user manual

# get validate and write PSK

# print Nameplate
