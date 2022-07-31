# EngineForAnimationCourse
Graphic Engine based on Libigl

For compiling:
1. Clone or download the project
2. Download Cmake from the link in the site
3. Run Cmake gui. choose the project folder and destination folder for the cpp project files. choose after pressing configure choose compiler (VS2019 for example). After finish configuration successfully, press configure again and after it finishes press generate. 
4. If everything pass successfully got to the destination folder and launch the project. 
5. Copy configuration.txt from tutorial/sandBox to build/tutorial/sandBox sandBox as a startup project and compile the project (it could take few mineutes);   


## fiture of the shcene edithor project :

user can pick one or more object on the screen, by clicking them. object that are sellected will have a black highlight. 


### the camera menu :
1. user can move the edit camera by the {left,right,up,down} arrow and by b (backword) , f (forword) or bt rhe mause curser 
2. the user view are spliting to to erea, the left one is the shcene editor , and the right one is the bezier editor.
3. the user can add camera to view in the shapes, and see them by the animation view , by clicking on the manu camera , and then click on add.
3. the user can switch betwin the camera after he add one more camera by opening the camera menue and click switch. in the animation camera and at animation , the user can't edit the schene.
4. the user can simply split schreen to watch the edit and the animation camera  in the same time by opening the camera menu and clicking on the split schreen button.
5. the user can zoom in the edit shcreen by click on the key z on the keyboard after he set the cursor on the disier place to zoom in.


### the metirial menu:
1. the user can load .png file to make a new metirial to the presented objects, by clicking on the "Add" button on the Metirials menu.
2. the user can change a shape metirial , by clicking and selecting the disiered shape,select the disier metirial from the list and clickin on the "Change" button on the metirials menu.
3. the user also can give a Object mirror effect by selecting the disiered object and clikking on "Tugle Mirror", second click will remove the effect.

### layers :
1. each shape has their layer, as difault the layer is layer 1.
2. user can change the shape layer by: clicking the disier shape, picking the disier layer from the leyer selected tub, and click on the change button.
3. user can add new layer by clicking on the add button on the layers manu.
4. the user can hide / unhide layers:  on the hide/unhide list their is a list off all the layer, selectind ther box mean the layer is shown to on the shcreen , un check a box mean the object on that layer will not e present to the user (hide) .


### Background :
1. the background is a cube map of the sky by defoult.
2. the user can change the Background by selecting one of the option list in the menu, and cick on change button.

