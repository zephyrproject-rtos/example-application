# Application button_simple
## About the project

The aim of this application is to implement the 4 buttons on the nrf5340dk board in a simple way. The application is based on the zephyr application in `samples/basic/button`. The contribution of this application is to try and simplify the use of the GPIO API for our use cases.

## Built with

[![](https://zephyrproject.org/wp-content/uploads/sites/38/2020/09/ZephyrLogo.png)](https://www.zephyrproject.org/)

## Functionement 

- To create a button, first define the device tree alias with 
    ```c
    #define SW0_NODE	DT_ALIAS(sw0)
    ```
Here is for the sw0 (button1) declared in the device tree of nrf5340dk.
- Then define a structure for each button (or an array of structures, for example) with 
  ```c
  static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
  ```
Here is always for the sw0. Replace the `{0}` with the corresponding sw.

- Then, the `button_init(&button)` function takes the address of the corresponding button as a parameter, and initializes the corresponding button.

- Finally,  `button_state(&button)` function function is used to retrieve the state of the button when the function is called. The value returned is an integer, but the value can be 0 or 1.

## Expected behaviour

The behavior expected by the program is as follows: each button is read with an interval of 1ms. The values are displayed as follows:

```shell
Value btn1: ?, btn2 : ?, btn3 : ?, btn4 : ?
```

With questions marks replaced by the buttons values

## Knowing issues

Empty for the moment

## Contacts

| First Name        |  Last Name        |   Mail  
|---                |:-:                |:-:    
| Caldeira          | Quentin           | quentin89.caldeira@gmail.com 
| Brunet            | Jordan            | jordan.brunet12@gmail.com
| Pedroli           | Johan             | johan.pedroli@gmail.com
| Phung             | Billy             | billy.phung45@gmail.com

