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
objects can be modified only if a single shape is picked.
* the default bezier := the bezier curve shown to the user when no shape is selected.
* if only one shape is selected its current bezier curve will be shown
* no bezier curve will be shown while multi picking.

### object move
1. each object has a unique bezier curve to follow (as shown in class) while animate 
2. the object's bezier curve can be modified by using the bezier curve editor on the right screen while the object is the only picked shape.
3. each object has a delay factor to wait until starting animation can be changed by the slider delay in the man menu

user can pick one or more object on the screen, by clicking them. object that are sellected will have a black highlight. 


### the camera menu :
1. user can move the edit camera by the {left,right,up,down} arrow and by b (backword) , f (forword) or bt rhe mause curser 
2. the user view are spliting to to erea, the left one is the shcene editor , and the right one is the bezier editor.
3. the user can add camera to view in the shapes, and see them by the animation view , by clicking on the manu camera , and then click on add.
3. the user can switch betwin the camera after he add one more camera by opening the camera menue and click switch. in the animation camera and at animation , the user can't edit the schene.
4. the user can simply split schreen to watch the edit and the animation camera  in the same time by opening the camera manu and clicking on the split schreen button.

### object layers
1. each object is assigned to a layer
2. current layer can be changed using the layers list and will be changed to the current shape layer upon picking a single shape.
3. new objects will be created on current layer (shapes created by bezier will be created on layer 1 if no layer is picked)
4. layers can be hidden to hide all shapes assigned to the layer by using the checkbox with the layer's number.

### bezier curve uses
1. the default bezier can be used to design objects based on bezier surfaces using the button "Add Shape" in the bezier menu.
2. the animation camera (can be seen in split screen and by pressing switch) can be toggled to move according to a bezier curve (move camera in bezier menu).
3. the bezier curve can be updated to the current default bezier state by using "update camera"
