# SimuWare

SimuWare is a virtual platform that enables users to assemble physical components like cubes, cones, etc. in a 3D game-like environment and simulate their interactions following physical and mechanical principles. It provides an educational and prototyping tool for simulation enthusiasts, students, educators, and researchers.

## Software Requirement: Unreal Engine 4.27.2
## Table of Contents
1. [Introduction](#introduction)
2. [Overall Description](#overall-description)
3. [User Characteristics](#user-characteristics)
4. [Constraints](#constraints)
5. [Assumptions and Dependencies](#assumptions-and-dependencies)
6. [Specific Requirements](#specific-requirements)
    1. [Adding, Connecting, and Moving Objects](#adding-connecting-and-moving-objects)
    2. [Executing Arduino Code](#executing-arduino-code)
    3. [Using Arduino Objects to Interact with Other Objects](#using-arduino-objects-to-interact-with-other-objects)
    4. [Physics Simulation](#physics-simulation)
7. [Usage](#usage)
8. [Directory Structure](#directory-structure)
9. [References](#references)

## Introduction

The purpose of SimuWare is to provide a three dimensional virtual platform for assembling physical components and simulating their interactions. Users can prototype their designs without the need for physical construction, allowing for faster iteration and identification of design flaws in 3D environment.

## Overall Description

### Product Perspective

SimuWare provides a rapid-prototyping environment for physical and micro-controller based simulations. It enables users to test their prototypes within the software without building them in real life, reducing the need for costly components and hit-and-tries.

### Product Functions

- **3D Environment**: SimuWare offers a 3D virtual environment powered by Unreal Engine or Unity, allowing users to navigate intuitively.
- **Part Assembly System**: Users can select from a library of Arduino components and place them within the 3D environment. Components snap or attach to each other realistically.
- **Physics Simulation**: SimuWare incorporates physics simulation to simulate interactions between assembled components, including realistic interactions with the environment.
- **User Interface**: SimuWare provides a user-friendly interface for selecting, placing, and manipulating components, as well as accessing tools for assembly, simulation, and data visualization.

## User Characteristics

SimuWare caters to simulation enthusiasts, students, educators, and researchers. Users can range from beginners to advanced users familiar with microcontrollers and physics principles.

## Constraints

The performance of SimuWare may vary on different hardware configurations, depending on factors such as the presence of a graphics card and available RAM.

## Assumptions and Dependencies

SimuWare assumes some basic ideal-physics assumptions, and results might differ from real-world simulations. Users are expected to have basic knowledge of Arduino and familiarity with basic circuit design.

## Specific Requirements

### Adding, Connecting, and Moving Objects

- Users should be able to select components from an inventory and place them within the 3D environment.
- Components should snap/attach to each other.
- Object manipulation should include resizability and rotatability.


### Executing Arduino Code

- There should be a specific Arduino object capable of running code on itself.

### Using Arduino Objects to Interact with Other Objects

- Arduino objects should be able to connect to other objects.
- Their code should be able to manipulate other objects.

### Physics Simulation

- The software should be able to simulate objects in real-time.

## Usage
 1. Clone the repository: 
```
   git clone https://github.com/RadioactiveDK/SimuWare

```
2. Open the project in Unreal Engine or Unity.

3. Explore the user interface and features as described in the documentation.

## Directory Structure

<pre>
SimuWare_ue4/<br>
│<br>
├── Config/<br>
│   ├── DefaultEngine.ini<br>
│   ├── DefaultEditor.ini<br>
│   ├── DefaultGame.ini<br>
│   ├── DefaultInput.ini<br>
│   └── DefaultEditorPerProjectUserSettings.ini<br>
│<br>
├── Content/<br>
│   ├── FPWeapon/<br>
│   │   └── MainMap.umap<br>
│   │<br>
│   ├── FirstPerson/<br>
│   │<br>
│   │<br>
│   ├── FirstPersonCPP/<br>
│   │<br>
│   │<br>
│   └── Geometry/<br>
│       └── Meshes/<br>
│<br>
├── Source/<br>
│   ├── SimuWare_ue4/<br>
│   │<br>
│   └── SimuWare_ue4.Target.cs<br>
│   └── SimuWare_ue4Editor.Target.cs<br>
│<br>
├── .gitattributes<br>
├── .gitignore<br>
└── SimuWare_ue4.uproject
</pre>

## References

- [Arduino GitHub Repository](https://github.com/arduino)
- [Unreal Engine](https://www.unrealengine.com/en-US)

