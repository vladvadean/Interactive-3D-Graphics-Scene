



# Photorealistic Presentation of a Scene with 3D Objects

![ss1](https://github.com/vladvadean/Interactive-3D-Graphics-Scene/assets/126804850/d009dfd2-638b-4bc1-8f7c-84ba91c65b44)


![ss2](https://github.com/vladvadean/Interactive-3D-Graphics-Scene/assets/126804850/dcaca4eb-bbd5-47ed-b352-e6fb76037453)

![ss3](https://github.com/vladvadean/Interactive-3D-Graphics-Scene/assets/126804850/28333c7e-0cf7-40aa-8e75-15fe807a1d41)
![ss4](https://github.com/vladvadean/Interactive-3D-Graphics-Scene/assets/126804850/691dde79-bde2-4a8d-8f38-a1a0594eeae6)
![ss5](https://github.com/vladvadean/Interactive-3D-Graphics-Scene/assets/126804850/292140bb-57cb-40c1-9068-852a55008106)
![ss6](https://github.com/vladvadean/Interactive-3D-Graphics-Scene/assets/126804850/10077fd3-684a-4820-8cfa-6a2375ab675c)


## Table of Contents

# Table of Contents
1. [Presentation of the Theme](#presentation-of-the-theme)
2. [Scenario](#scenario)
3. [Implementation Details](#implementation-details)
4. [User Manual](#user-manual)
5. [Conclusions and Further Developments](#conclusions-and-further-developments)
6. [References](#references)


## Presentation of the Theme

&nbsp;&nbsp;&nbsp;&nbsp;The process of creating a photorealistic presentation of a scene with 3D objects is a complex combination of art and technique, involving multiple stages to achieve a high level of realism. Starting with the modeling of 3D objects, artists must ensure that every detail of the geometry is accurately represented. This requires attention to shapes, proportions, and textures.

&nbsp;&nbsp;&nbsp;&nbsp;Once the models are completed, the next crucial step is texturing and material assignment. Here, textures are added and materials are configured to reflect the real behavior of the objects. From shiny metal to finely grained wood, each object must have a texture and material that gives it a believable appearance.

&nbsp;&nbsp;&nbsp;&nbsp;Lighting is another vital aspect of achieving photorealism. Artists need to set up lights so that they create realistic shadows, reflections, and refractions. Using ray tracing techniques can add additional depth to images, considering how lights interact with objects.

&nbsp;&nbsp;&nbsp;&nbsp;If applicable, animation can be integrated into the scene to bring movement and life to the objects. This can be a complex process but adds an additional level of realism and actively engages the viewer.

&nbsp;&nbsp;&nbsp;&nbsp;Finally, rendering and post-production are the final stages of the project. Images or animations are created in high resolutions, and then they can be brought into editing software for final adjustments, such as color correction or adding special effects.

&nbsp;&nbsp;&nbsp;&nbsp;To accomplish this project, the objects were modeled and textured in Blender 4.0 and integrated into OpenGL.

## Scenario

### Scene Description

&nbsp;&nbsp;&nbsp;&nbsp;The scene is set in a desert and features an abandoned factory, with the user having the option to choose the time of day. It is surrounded by an oil extraction field, while a military convoy heads towards it for investigation. All objects are textured in a way that gives the impression of abandonment and desolation. The road to the factory is the path along which the military convoy moves towards the objective, generating an additional light source, observable during the night.

### Functionalities

-   Camera movement within coordinate systems and its rotation
-   Scene presentation animation
-   Two light sources: global and spot
-   Texturing all objects to give a realistic feel to the scene
-   Presentation of the scene in three different viewing modes: solid, point, and vertex
-   Demonstrating the animation of objects in the scene
-   Viewing at different times of the day
-   Implementation of fog mechanism

## Implementation Details

### Functions and Algorithms

&nbsp;&nbsp;&nbsp;&nbsp;Possible solutions include using two pairs of shaders: one for the scene object itself and another for the skybox background. The project features two light sources: one global and another spotlight type. The animation of the helicopter rotor represents an object rendered separately subject to rotation around its own center.

### Rationale for the Chosen Approach

&nbsp;&nbsp;&nbsp;&nbsp;The algorithms used for generating lights, animating objects, illuminating objects, etc., were chosen to allow the scene to be processed as easily and quickly as possible, allowing the user to move smoothly with the camera through the scene.

### Graphic Model

&nbsp;&nbsp;&nbsp;&nbsp;In OpenGL, the graphic model represents the foundation for displaying 3D objects on screen. This involves defining and manipulating three-dimensional geometry to create a visually realistic scene. Fundamentally, this model involves using vertices to represent points in 3D space. Each vertex is defined by its three-dimensional coordinates and can have other properties such as color or texture.

&nbsp;&nbsp;&nbsp;&nbsp;These vertices are then connected to form the surfaces of objects, most often using triangles or other polygons. This allows for the definition of the shape and structure of 3D objects.

&nbsp;&nbsp;&nbsp;&nbsp;To bring objects into the scene and position them in 3D space, modeling matrices are used. These matrices allow for transformations such as translation, scaling, and rotation of objects, thus achieving the desired position and orientation in the scene.

&nbsp;&nbsp;&nbsp;&nbsp;Overall, the graphic model in OpenGL is a fundamental concept that facilitates the process of representing 3D objects and creating complex and realistic graphic scenes.

### Data Structures

&nbsp;&nbsp;&nbsp;&nbsp;The data structures used are standard from OpenGL & GLFW, GLM, and GL libraries such as: vec3, vec4, mat3, and mat4.

## User Manual

&nbsp;&nbsp;&nbsp;&nbsp;To start the scene, run the Scene.exe executable. After loading objects and textures, the user can navigate through the scene with WASD and rotate the camera with QE. To view the scene in animation and exit this mode, press the L key. To switch from day to night and vice versa, press N, and similarly O to change the scene to sunset. Fog activation is done from the F button, while increasing or decreasing the fog level is done from G and H. To switch between viewing modes, we have keys 1, 2, and 3.

## Conclusions and Further Developments

&nbsp;&nbsp;&nbsp;&nbsp;The 3D scene created in OpenGL represents a captivating exploration in the field of computer graphics. This project allowed for the creation and display of three-dimensional objects in a virtual environment, using fundamental concepts and technologies of 3D graphics and the OpenGL library.

&nbsp;&nbsp;&nbsp;&nbsp;In this process, we learned that proper modeling and texturing of objects are essential for achieving a realistic appearance. Using triangles to define the shapes and surfaces of objects helped us accurately represent their geometry. We also discovered the importance of correct lighting in achieving a photorealistic look, accomplished by setting up light sources and object materials.

&nbsp;&nbsp;&nbsp;&nbsp;Regarding further developments, there are numerous directions in which this project can be extended and improved. Some of these include:

-   Creating shadows
-   Creating different weather phenomena
-   Adding more objects
-   Using much more realistic textures

## References

-   Laboratory works
-   Graphic design courses
