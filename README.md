#  Computer Graphics Project
## Students Team Data

Name: Inbar Lev Tov

Student Number: 316327246


Name: Avital Berendorf

Student Number: 205815111

## Project Description:
In this project we would load models to the screen and render the silhouette edges (outlines) around the models

## Reading Materials:
### Truly Relevant:
https://prideout.net/blog/old/blog/index.html@tag=opengl-silhouette.html

https://gamedev.stackexchange.com/questions/68401/how-can-i-draw-outlines-around-3d-models

### The Rest Of The Research:
https://blog.vfrz.fr/2d-silhouette-effect-in-opengl/

https://developer.download.nvidia.com/CgTutorial/cg_tutorial_chapter09.html

http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/

https://en.wikibooks.org/wiki/GLSL_Programming/Unity/Specular_Highlights_at_Silhouettes

https://en.wikibooks.org/wiki/GLSL_Programming/Unity/Silhouette_Enhancement

https://gamedev.stackexchange.com/questions/103365/glsl-2d-silhouette

## Progress:

The first two weeks: 23/10/2022 - 05/11/2022

- Discussion about the project's main subject with Roi.
- Researching silhouette rendering
- Total of 8 hours

Week 3: 06/11/2022 - 12/11/2022

- Researching silhouette shading opengl (because the original research was irrelevant)
- Finally nailing project's subject: using shaders to generate silhouette edges (outlines)
- Receiving initial skeleton from Roi
- Total 0f 12 hours

Week 4-5 13/11/2022 - 26/11/2022

- Research about OpenGL
- Research about .frag files
- Total of 6 hours

Week 6: 27/11/2022 - 03/12/2022

- Checkbox for silhouette shading
- Beginning shader - can't find this file at this point
- Total of 8 hours

Week 7: 04/12/2022 - 10/12/2022

- Changing strategy to focus on adjacency calculation
- Changing tiny obj file in order to get faces vector
- Total of 10 hours

Week 8: 11/12/2022 - 17/12/2022

- Calculating adjacency, storing in vector
- Checking correctness of adjacency calculation
- Finishing draft of shader
- Total of 15 hours

Week 9: 18/12/2022 - 24/12/2022

- Discussion with Roi regarding progress in project
- Fixing inheritance problems
- Passing data correctly between vertex, geometry, fragment shader
- Total of 12 hours

Week 10: 25/12/2022 - 31/12/2022

- There is a bug: we draw half filled faces instead of only silhouette lines.
- Trying to understand the source of the problem.
- Total of 10 hours

Week 11: 01/01/2023 - 07/01/2023

- We fixed the problem - drawing only lines at this point.
- New bug: the lines don't correspond with the correct shape of the object. Searching how to solve this.
- Total of 15 hours

Week 12-13: 08/01/2023 - 21/01/2023

- We took a 2 weeks break in order to focus on finishing the semester
- That break was discussed and agreed upon with Roi in advance

Week 14: 22/01/2023 - 28/01/2023

- Removing redundant code
- Fixing shader - making it more readable and correct
- Total of 16 hours

07/02/2023:

- Testing silhouette on a number of objects
- Recording videos of silhouette models
![cube](cube.gif)
![star](star.gif)