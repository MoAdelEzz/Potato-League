# Game Engine Project

## Part 1: Game Engine

### Req 1: Shader Program

* Complete TODOs in `source\common\shader\shader.hpp` and `source\common\shader\shader.cpp`.
* Complete some shaders in `assets\shaders\triangle.vert`,`assets\shaders\color-mixer.frag`,`assets\shaders\checkerboard.frag`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests shader-test`

### Req 2: Mesh

* Complete TODOs in `source\common\mesh\mesh.hpp`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests mesh-test`

### Req 3: Transform

* Complete TODOs in `source\common\ecs\transform.hpp`,`source\common\ecs\transform.cpp`.
* Complete some shaders in `assets\shaders\transform-test.vert`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests transform-test`

### Req 4: Pipeline State

* Complete TODOs in `source\common\material\pipeline-state.hpp`,`source\common\texture\texture-utils.hpp`,`source\common\texture\texture-utils.cpp`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests pipeline-test`

### Req 5: Texture

* Complete TODOs in `source\common\texture\texture2d.hpp`,`source\common\ecs\transform.cpp`.
* Complete some shaders in `assets\shaders\texturetest.frag`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests texture-test`

### Req 6: Sampler

* Complete TODOs in `source\common\texture\sampler.hpp`,`source\common\texture\sampler.cpp`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests sampler-test`

### Req 7: Material

* Complete TODOs in `source\common\material\material.hpp`,`source\common\material\material.cpp`.
* Complete some shaders in `assets\shaders\tinted.vert`,`assets\shaders\tinted.frag`,`assets\shaders\textured.vert`,`assets\shaders\textured.frag`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests material-test`

### Req 8: ECS

* Complete TODOs in `source\common\ecs\entity.hpp`,`source\common\ecs\entity.cpp`,`source\common\ecs\world.hpp`,`source\common\ecs\world.cpp`,`source\common\components\camera.hpp`,`source\common\components\camera.cpp`,`source\common\components\meshrenderer.hpp`,`source\common\components\meshrenderer.cpp`,`source\common\components\componentdeserializer.hpp`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests entity-test`

### Req 9: Forward Renderer System

* Complete TODOs in `source\common\systems\forward-renderer.hpp`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests renderer-test`

### Req 10: Sky Rendering

* Complete TODOs in `source\common\systems\forward-renderer.hpp`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests sky-test`

### Req 11: Postprocessing

* Complete TODOs in `source\common\systems\forward-renderer.hpp`,`source\common\texture\textureutils.cpp`.
* Complete some shaders in `assets\shaders\postprocess\vignette.frag`,`assets\shaders\postprocess\chromatic-aberration.frag`.
* Pass all the test using the following script : `powershell -executionpolicy bypass -file ./scripts/run-all.ps1 -tests postprocess-test`

## Part 2: Game Development

**Rules**
* Usage of 3D models (created using any modeling program or downloaded from the internet).
* Lighting with multiple lights in the scene.
* Texture mapping (for models, environment, … etc.) with support for different texture types in lit
materials (albedo, specular, roughness, etc).
* A sky.
* At least one postprocessing effect that we was not implemented or provided in Phase 2.
* 3D motion.
* Any form of collision detection in 3D (obstacles, ray-picking, etc).
* Game logic (some form of game-like user interaction with the world where the player has to work
towards one or more goals).
* Scene Deserialization (All scenes should be defined in external data files. No scene data should be
hard coded).
* At least one play state (where the gameplay is implemented) and one menu state. You are already
supplied with a menu state, so you don't need to implement it yourself, but at least you have to make
sure that you can go from it to the play state and vice versa.
