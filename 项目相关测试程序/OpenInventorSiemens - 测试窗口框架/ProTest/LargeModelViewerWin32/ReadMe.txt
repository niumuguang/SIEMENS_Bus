The LargeModelViewer program is a demonstration of adaptive viewing
and ordering traversal techniques.

AdaptiveViewing 
Under the menu bar are displayed rendering and scene complexity
statistics.  The first section of the statistics bar displays the
current frames per second and the decimation percentage.
It will take a few frames to get an average fps, so the frames per
second display may be off immediately after loading a new file.
The decimation percentage will not display until it drops below 100%.

The "Edit Adaptive Viewing.." menu button will display options for
controlling the scene complexity through the new "Decimation"
element.  There are four radio buttons for choosing the
strategy the viewer should use:
   - Normal : 
   	do not control decimation element; performs the same as previous
   	versions of OpenInventor.
   - Fixed Number of Triangles :
	the viewer will use a GetPrimitiveCountAction to find how many
	triangles are in the scene, and then adjust the decimation element
	so that the number of triangles rendered remains constant.
   - Frames Per Second :
   	the viewer will time each render traversal and then adjust the
   	decimation element to keep the render time close to the
   	desired frame rate.
   - Fixed Percentage :
   	the same decimation percentage will be used always.  In addition,
   	the DecimationType element will be set to PERCENTAGE, so that
   	SoLOD and SoLevelOfSimplification nodes will choose a child based
   	on the DecimationPercentage element alone, without looking at
   	the camera position.  Because the LOD node is no longer dependent
   	on the camera position, its GL calls can be cached and may result
   	in quicker rendering.
   	
The "Do Full Rendering When Still" check box tells the viewer to render at 
1.0 decimation (full detail) when the scene is still.  This allows you
to interact with the scene at a lower decimation, and then see all of the
details when done.


Reorganizing

Under the "Processing" is the button "Reorganize as TriStrips".  This
applies an SoReorganizeAction that reorganize the scene graph similar
to the "ivfix" utility program.  In files with no Inlines or LOD nodes,
this will group shapes by material and texture and then convert the
shapes into SoIndexedTriangleStripSet nodes.  The reorganized scene graph
should render quicker, although the effects will be more noticeable on 
hardware-accelerated graphics cards.

Notice that the TriangleStripping algorithm has been improved, so if you
have used ivfix before and were unsatisfied, be sure to try it again 
now in version 2.5! 

Simplifying

Two of the SimplifyActions are accessible from the "Processing" menu,
ShapeSimplify and GlobalSimplify.  Both of these actions will produce 
multiple levels of detail for the scene.  You can then use the "Adaptive
Viewing" dialog to choose which level of detail to view.

The ShapeSimplifyAction will simplify each shape individually,
leaving the rest of the scene graph intact.  The GlobalSimplifyAction
will group the shapes together and simplify the scene as a whole,
replacing the old scene graph.

The last two buttons on the menu combine the Simplify and Reorganize 
actions.  First the Simplify action is run on the scene, and then 
each of the simplified shapes are organized into triangle strips.

Notice that you can only simplify the scene once.  If you want to try
a different simplify technique on the same scene, you will have to
reload the original file again.


Traversal

The traversal menu allows you to experiment with the new Ordering nodes,
derived from SoRenderList.  This menu alters the type of grouping node
kept above the loaded scene.  This node is kept below the lights and camera,
but above the actual geoemtry. The default option is SoSeparator, which will 
result in the usual traversal behavior; the other two options use either
a SoValueOrdering node or a SoOctreeOrdering node.  

The ValueOrdering node will work well with assemblies, where there
are many parts of various sizes.  This node allows you to use the 
AdaptiveViewing techniques on scenes that normally would not respond 
to the Decimation element, if for instance there are no LODs in the scene.

The OctreeOrdering node, in contrast, is better in spatially large
scenes, where the viewer moves through the scene instead of around it.
The OctreeOrdering node organizes the shapes into an octree spatial
organization, which allows it to cull large areas of space quickly 
that will not be within the view frustrum.

If you load a file with VRML 2.0 animations, the flag under the
"Processing" menu "Find VRML 2.0 Animations" will search
for VRML 2.0 Time Sensors to determine which subgraphs will be
changing in the animation, and mark these as render objects for the
Ordering nodes.  If you are not using VRML 2.0 files, you can
turn the flag off for quicker file loading.

Notice that after Reorganizing, some small shapes with the same material
that are far away from each other will be grouped into one large shape.  
This will prevent the Ordering nodes from drop culling effectively.
If you are wondering how the shapes are grouped, try changing the
draw style to bounding box with the right mouse button menu.
The same is true for the GlobalSimplifyAction because it groups together 
all of the shapes; however, the Ordering nodes should work well
with the ShapeSimplifyAction.


LargeModel Example files

Some large model files have been included in the distribution. 
The "formula1" example contains many SoLOD nodes, so the best method would
be to use the DecimationStrategy dialog to set the desired level of detail,
and leave the Traversal as "Separator".  This would also be the case
for files output from the "Simplifier/ivsimplify" demo program.
The fishing_reel.wrl and Harley.iv files contain many parts, but do not 
contain LOD nodes, so the ValueOrdering node should be used to allow
the scene to be decimated through bounding boxes and drop cullingi of
individual shapes.
A new directory "allModels" has been added under "data/models", which uses
WWWInlines to link the other example files into one scene.  Each file
represents a directory under data/models.  The "allModels.iv" file in this
directory joins these files into one large scene.
This can be used as an example for the OctreeOrdering node.

Fetching Inlines

This demo program assumes that any inlines can be resolved on the
local filesystem.  It will not fetch http requests over the
network.  From the "Processing" menu, you can set whether to load
all of the Inlines immediately, or only as needed.

"Viewing" menu

This menu contains a number of flags that can boost performance 
in special cases.  You can experiment with these flags to see which
ones work with your data.

* Show Primitive Count
Although the primitive count is cached under separators that do not change,
it does take some time to apply the GetPrimitiveCountAction, get the results 
and update the display.  You may find that turning off the display
when not needed gives the viewer a performance boost.  This is
especially the case with the OctreeOrdering and ValueOrdering nodes 
and with some scenes with LODs.

* Backface Culling
Inventor by default draws both sides of triangles, but if you
have an assembly of solid parts, the interior sides of the triangles
are not needed.  This flag, when on, sets fields of a ShapeHints
node such that vertexOrdering is COUNTERCLOCKWISE and 
shapeType is SOLID.  When the flag is off, these fields are ignored,
letting the scene (and any ShapeHints in it) decide whether to
do backface culling.

* Disable GL Normalize
If turned on, the OpenGL flag, GL_NORMALIZE will be disabled.  This
may increase rendering speed for scenes that do not contain any 
scaling.  If the model contains scales (SoTransform's scaleFactor field), 
then these parts will either glow too bright, or be too dark.
Often, using the ReorganizeAction (under menu "Processing") will
remove the scales, allowing you to disable GL_NORMALIZE for 
increased performance. (Try the data/models/vehicles/kittyHawk.iv
file for an example; the engine will glow too brightly if
GL_NORMALIZE is disabled).

* Generate Smooth Normals
If the model has no Normal nodes, Inventor will generate 
smooth normals for the shape if this is on.  This flag sets the 
creaseAngle field of a ShapeHints node to 0.5.

* Delayed Blended Transparency
Switches between SCREEN_DOOR and DELAYED_BLEND transparency.  With this
switch on, transparent objects will appear correct, but the viewer will
run slower.




