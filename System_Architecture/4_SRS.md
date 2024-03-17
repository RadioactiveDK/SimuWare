# Software Requirments Specification
## Introduction
### Purpose
The purpose of our software (SimuWare) is to provide a virtual platform that enables users to assemble physical and electrical components in a 3D game-like environment and to simulate their interactions. SimuWare aims to provide an educational and prototyping tool for simulation enthusiasts, students, educators, and researchers.

### Scope
#### Goals
- SimuWare will feature a user-friendly interface for selecting, placing, and connecting components within a virtual environment.
- Users will also be able to simulate the behavior of assembled components.

#### Benefits
- Users will be able to simulate their prototypes without having to build them in real life.
- They will easily get to know the flaws in their circuits and will be able to correct those before building a real-life prototype.

### Definitions and Abbreviations
- SimuWare: The virtual platform developed for assembling physical components, simulating their interactions, and exploring physics and engineering principles in a 3D environment.
- Simulation: The process of creating a computer-based model of a real-world system or process to study its behavior and outcomes.
- Physics Engine: Software component responsible for simulating physical interactions and dynamics within a virtual environment.
- Component Library: Repository of pre-built physical objects, materials, and mechanisms that can be used within the simulation environment.
- Multiplayer Collaboration: A feature of SimuWare enabling multiple users to collaborate and interact within the same simulation environment simultaneously, fostering teamwork and knowledge sharing.

## Overall Description
### Product Perspective
This product aims to provide a rapid-prototyping environment for circuits and micro-controller based simulations. This will enable users to test their prototypes within our software without building them in real life. As a result, they will not have to do many hit-and-tries and waste their precious costly components.

#### User Interfaces
SimuWare is a desktop application (for windows).

#### Software Interfaces
- Unreal Engine / Unity 
- C++
- Arduino APIs

### Product Functions
#### 3D Environment
- SimuWare will provide a 3D virtual environment using Unreal Engine / Unity.
- Users will be able to navigate the environment using intuitive controls.

#### Part Assembly System
- Users shall be able to select from a library of Arduino components.
- Users shall be able to place selected components within the 3D environment.
- Components shall snap or attach to each other realistically when placed adjacent to each other.

#### Physics Simulation
- SimuWare will incorporate physics simulation to simulate interactions between assembled components.
- Components shall interact realistically with each other and the environment (e.g., gravity, collisions).

#### User Interface
- SinuGear shall provide a user-friendly interface for selecting, placing, and manipulating components.
- The interface shall allow users to access tools for assembly, simulation, and data visualization.

### User Characteristics
SimuWare aims to provide an educational and prototyping tool for simulation enthusiasts, students, educators, and researchers. Users can range from beginners to advanced users who like to tinker and create stuff using microcontrollers.

### Constraints
- The performance may vary on different hardwares depending upon factors like the presence of graphic card and RAM available.

### Assumptions and Dependencies
- The platform assumes some basic ideal-physics assumptions and some results might differ from real-world simulations.
- The user should have basic knowledge of arduino and familiarity with basic circuit design.

## Specific Requirements
### User shall be able to ADD, CONNECT AND MOVE OBJECTS in the virtual environment
- User should be able to select components from an inventory and place them within the 3D environment.
- Components should be able to snap/attach to each other.
- Object manipulation should include resizability and rotatability
- Components should be categorized into mechanical and electrical (sensors, actuators, etc) for easy searching.

### User shall be able to EXECUTE ARDUINO CODE on a specific ARDUINO OBJECT
- There should be a specific arduino object.
- It should have the ability to run a code on itself.

### User shall be able to use an ARDUINO OBJECT to INTERACT with OTHER OBJECTS
- Arduino object should be able to connect to other objects.
- Its code should be able to manipulate other objects.

### User shall be able to do PHYSICS-SIMULATION on the OBJECT
- The software shall be able to simulate objects in real time.
