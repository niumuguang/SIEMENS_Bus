TGS - MFCSDI
MFC Single Document Interface, Open Inventor, No IVF
----------------------------------------

This example demonstrates the key items you need to have and where
to place them to create a straight OpenInventor application within
MFC MDI.

OpenInventor sections in the code are bracketed with the following
comment convention:

//
// BEGIN OIV
//
		(oiv code...)
//
// END OIV
//

The primary areas you need to look at are:

   CMFCSDIApp (MFCSDI.cpp)

	InitInstance:
		initializes the OpenInventor Systems, except
		for SoWin.

	OnIdle:
		this calls the SoWin::doIdleTasks function to
		allow Inventor to process any messages/other
		tasks it needs to do on idle time.


   CMainFrame (mainfrm.cpp)

	OnCreate:
		initializes SoWin with the mainframe window.


   CMFCSDIView (MFCSDIView.cpp)

	OnInitialUpdate:
		creates an examiner viewer for this view and a 
		separator node.  The separator node is set to the
		viewer as the scenegraph.

	OnDestroy:
		deletes the examiner viewer member.

	OnObjectCone:
	OnObjectCube:
	OnObjectSphere:
		command handlers for each of the selections under
		the "Object" menu item.

	AddItem:
		Generic add routine.  Collects the object and color
		from the above command handler, creates a material
		and transform node and adds all the nodes to the
		separator node as children.
		


Note the document class (CMFCSDIDoc) is bypassed, since we're not
reading or writing any documents.
