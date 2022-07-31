# [Final project in computer graphic course]:
## Presenting : 
* bar stoyer : 211760756
* meir maor biton : 206000556
* naomi koren : 211378971



# EngineForAnimationCourse
Graphic Engine based on Libigl

For compiling:
1. Clone or download the project
2. Download Cmake from the link in the site
3. Run Cmake gui. choose the project folder and destination folder for the cpp project files. choose after pressing configure choose compiler (VS2019 for example). After finish configuration successfully, press configure again and after it finishes press generate. 
4. If everything pass successfully got to the destination folder and launch the project. 
5. Copy configuration.txt from tutorial/sandBox to build/tutorial/sandBox sandBox as a startup project and compile the project (it could take few mineutes);   


## feature of the scene editor project :

* the scene can only be edited while the scene is not on animation mod.
* objects can be modified only if a single shape is picked.
* the default bezier := the bezier curve shown to the user when no shape is selected.
* if only one shape is selected its current bezier curve will be shown
* no bezier curve will be shown while multi picking.
* user can pick one or more object on the screen, by clicking them. object that are sellected will have a black highlight. 


### object move
1. each object has a unique bezier curve to follow (as shown in class) while animate 
2. the object's bezier curve can be modified by using the bezier curve editor on the right screen while the object is the only picked shape.
3. each object has a delay factor to wait until starting animation can be changed by the slider delay in the man menuץ



### the camera menu :
1. user can move the edit camera by the {left,right,up,down} arrow and by b (backword) , f (forword) or bt rhe mause curser 
2. the user view are spliting to to erea, the left one is the shcene editor , and the right one is the bezier editor.
3. the user can add camera to view in the shapes, and see them by the animation view , by clicking on the manu camera , and then click on add.
3. the user can switch betwin the camera after he add one more camera by opening the camera menue and click switch. in the animation camera and at animation , the user can't edit the schene.
4. the user can simply split schreen to watch the edit and the animation camera  in the same time by opening the camera menu and clicking on the split schreen button.
5. the user can zoom in the edit shcreen by click on the key z on the keyboard after he set the cursor on the disier place to zoom in.

### object layers:
1. each object is assigned to a layer
2. current layer can be changed using the layers list and will be changed to the current shape layer upon picking a single shape.
3. new objects will be created on current layer (shapes created by bezier will be created on layer 1 if no layer is picked)
4. layers can be hidden to hide all shapes assigned to the layer by using the checkbox with the layer's number.

### bezier curve uses:
1. the default bezier can be used to design objects based on bezier surfaces using the button "Add Shape" in the bezier menu.
2. the animation camera (can be seen in split screen and by pressing switch) can be toggled to move according to a bezier curve (move camera in bezier menu).
3. the bezier curve can be updated to the current default bezier state by using "update camera"


### the metirial menu:
1. the user can load .png file to make a new metirial to the presented objects, by clicking on the "Add" button on the Metirials menu.
2. the user can change a shape metirial , by clicking and selecting the disiered shape,select the disier metirial from the list and clickin on the "Change" button on the metirials menu.
3. the user also can give a Object mirror effect by selecting the disiered object and clikking on "Tugle Mirror", second click will remove the effect.

### Background :
1. the background is a cube map of the sky by defoult.
2. the user can change the Background by selecting one of the option list in the menu, and cick on change button.

### Transperent :
1. each object can be tugle to be transferent by picking a single shape and click on the trasferent chack-box.
2. the tranferent object has the alpha value (as part of the blending function) that means how many tranferent the object are.(0 is fully transferent, 1 is not trasferent)

### Animation :
1. move the shape: after selecting a single shape the user can edit the shape bezuer function by moving the red dots.
2. while clicking on space key , or on start on the menu, the animation start.
3. on the defoult bezier (while not selctig shape) , the user can edit the bezier and change the camera bezier movement in the animation by clicing the update button on the bezier menu.( [as mantion on the bezier part])
4. the camera movment in the animation an tugle on\off by dement in the bezier manu.( [as mantion on the bezier part])

### Blur effect :
1. after picking a singl shape , in the blur menu , the user can tuggle the blur effect and make the selected shape blured.
2. the blur function contains to controled parameter 'radius' and 'resolution' , that has scroll that change their value on the selected shape.

### Fog:
1. on the bottom of the menu , their is a fog toggle , at toggle on, all the object in screan will has a fog effect.
2. object effected by fog mean that : 
* *the shape will be blured with a fog color (grayies). 
* *the farrest from the camera , the tick the fog is.

## Bonus feature : 
* make a shape frome bezier line [nomber 1 in bezier use]
* [blur object] by a blur matrix with change radius and resolution 
* mirror objects - can put mirror material
* [global fog effect] compute by the depht of the shape , the farrest is the fogiest. 

## general hardships
*the cameras were hard to implement
*we had trouble working with stencil effects([for highlighting the shape]), draw infos([multiple cameras]), viewports([split screen]) and such




[multiple cameras]:#the-camera-menu-

[split screen]:[multiple cameras]

[for highlighting the shape]:#feature-of-the-scene-editor-project-

[Final project in computer graphic course]:https://github.com/maor226/CG3DBasicEngine/

[global fog effect]:#fog

[blur object]:#blur-effect-

[number 1 in bezier use]:#bezier-curve-uses

[as mantion on the bezier part]:#bezier-curve-uses
