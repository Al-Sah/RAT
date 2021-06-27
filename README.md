# RAT - remote administration tool or remote access trojan

Goal of this project is to consolidate existing knowledge and develop a cross-platform system of
remote access for the wide variety of devices (workstations, servers, single-board computers, etc). Designed suite of tools consists of 3 components: GUI app for managing, C&C server and a “bot” (control-side, server, bot-side)


- control-side: Crafts control messages/instructions and passes them to the server
- server: Implements routing of messages (Passes data between connected components/apps)
- bot-side: Executes instruction, passes execution results to the requesting node via server

---

## Components interaction

<img src="./images/rat-components-interaction.png" width="600">

<img src="./images/modules-interaction.png" width="600">

---

## Messages transferring

<img src="./images/messages-transferring.png" width="500">

---


## Client core structure

<img src="./images/core-and-modules.png" width="500">

<img src="./images/core-structure.png" width="500">

<img src="./images/inheritance.png" width="400">

---




