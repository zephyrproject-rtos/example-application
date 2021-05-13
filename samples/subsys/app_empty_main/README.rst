.. _app_empty_main:

App Empty Main
##############

Overview
********

A sample of an application that is:

- configured, built, and CI tested without having any content
  in the the body of ``main()``
- without console output
- recognized and runnable by ``twister``


Building and Running
********************

This application can be built and executed on QEMU Cortex M0 as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/app_empty_main
   :host-os: unix
   :board: qemu_cortex_m0
   :goals: run
   :compact:

Sample Output
=============

[00:00:00.000,000] <inf> custom_subsys: Hello from "example-application"
