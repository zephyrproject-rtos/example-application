.. _module_hello:

Module Hello
############

Overview
********

A simple sample that prints "Hello from <module>" to the console.

Building and Running
********************

This application can be built and executed on QEMU Cortex M0 as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/module_hello
   :host-os: unix
   :board: qemu_cortex_m0
   :goals: run
   :compact:

Sample Output
=============

.. code-block:: console

    Hello from example-application!

