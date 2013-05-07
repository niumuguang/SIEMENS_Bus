///////////////////////////////////////////////////////////////////////
// 
// TGS Open Inventor example to set key binding
//
// Copyright (C) 2003, TGS
// All Rights Reserved.
//
// Permission is granted for licensed customers of the Mercury
// Open Inventor library to use this source for any reasonable
// purpose that involves the Mercury Open Inventor library.
//
///////////////////////////////////////////////////////////////////////

#include <Inventor/Win/SoWinResource.h>
#include "SoWinExaminerViewerSetKeyBinding.h"

// This part is use to create your own key and mouse binding for your Examiner viewer.
SoWinExaminerViewerSetKeyBinding::SoWinExaminerViewerSetKeyBinding(
  Widget parent,
  const char *name, 
  SbBool buildInsideParent, 
  SoWinFullViewer::BuildFlag flag, 
  SoWinViewer::Type type)
 : SoWinExaminerViewer(parent, name, buildInsideParent, flag, type)
{
  // New key binding.

  // Reverse zoom
  addFunctionKeyBinding(SoKeyboardEvent::F2, new SoViewingReverseDolly());
  printf("Press F2 for Reverse Dolly viewing\n");
  // Zoom
  addFunctionKeyBinding(SoKeyboardEvent::F3, new SoViewingDolly());
  printf("Press F3 for Dolly viewing\n");
  // Translation
  addFunctionKeyBinding(SoKeyboardEvent::F4, new SoViewingTranslation());
  printf("Press F4 for Translation viewing\n");
  // X Rotation
  addFunctionKeyBinding(SoKeyboardEvent::F5, new SoViewingRotationX());
  printf("Press F5 for RotationX viewing\n");
  // Y Rotation
  addFunctionKeyBinding(SoKeyboardEvent::F6, new SoViewingRotationY());
  printf("Press F6 for RotationY viewing\n");
  // Z Rotation
  addFunctionKeyBinding(SoKeyboardEvent::F7, new SoViewingRotationZ());
  printf("Press F7 for RotationZ viewing\n");
  // Spherical rotation
  addFunctionKeyBinding(SoKeyboardEvent::F9, new SoViewingSphericalRotation());
  printf("Press F9 for Spherical Rotation viewing\n");
  

  // Assign new mouse binding.
  // Equivalent to F3 -> Zoom.
  SoKeyboardEvent::Key *spinModifierKeys = new SoKeyboardEvent::Key[2];
  spinModifierKeys[0]=SoKeyboardEvent::LEFT_CONTROL;
  spinModifierKeys[1]=SoKeyboardEvent::LEFT_SHIFT;
  SoMouseButtonEvent::Button *newButton = new SoMouseButtonEvent::Button[1];
  newButton[0] = SoMouseButtonEvent::BUTTON1;
  addViewingMouseBinding(spinModifierKeys, 2,newButton, 1, new SoViewingDolly());
  printf("Press LEFT_CONTROL + LEFT_SHIFT + Btn1 for Dolly viewing\n");
  
  // Equivalent to F4 -> Translation.
  SoKeyboardEvent::Key *panModifierKeys = new SoKeyboardEvent::Key[1];
  panModifierKeys[0]=SoKeyboardEvent::LEFT_SHIFT;
  SoMouseButtonEvent::Button *panButton = new SoMouseButtonEvent::Button[1];
  panButton[0] = SoMouseButtonEvent::BUTTON1;
  addViewingMouseBinding(panModifierKeys, 1, panButton, 1, new SoViewingTranslation());
  printf("Press LEFT_SHIFT + Btn1 for Translation viewing\n");
  
  // Equivalent to F2
  SoKeyboardEvent::Key *reverseDollyModifierKeys = new SoKeyboardEvent::Key[1];
  reverseDollyModifierKeys[0]=SoKeyboardEvent::LEFT_SHIFT;
  SoMouseButtonEvent::Button *reverseDollyButton = new SoMouseButtonEvent::Button[1];
  reverseDollyButton[0] = SoMouseButtonEvent::BUTTON2;
  addViewingMouseBinding(reverseDollyModifierKeys, 1, reverseDollyButton, 1, new SoViewingReverseDolly());
  printf("Press LEFT_SHIFT + Btn2 for reverse Dolly viewing\n");

  // Pick Mode
  setViewing(FALSE);
}

// Local resource.
#include "resource.h"

// To define new cursors for this viewer ! Without this, you will have same
// cursors as in the Examiner Viewer.
void 
SoWinExaminerViewerSetKeyBinding::defineCursors()
{
  // Get handle of SoWin resource DLL
  HMODULE hInst = (HMODULE)SoWin::getResDllSafeHandle().getHandle();

  // Get handle of local application. This to use local cursor !
  HMODULE hmod = GetModuleHandle(NULL);
  // Note: Any cursor that fails to load will be NULL and our cursor
  // treats that as a request for the default cursor, so that's OK.
  if (hInst && hmod) {
    switch (cursorStyle) {  
      // Following cursor are defined in the user project ressources !
    case SoWinViewer::XOR:
      flyCursor   = LoadCursor(hInst, MAKEINTRESOURCE(IDC_XOR_FLY_CURSOR));
      rollCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_XOR_ROLL_CURSOR));
      seekCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_XOR_SEEK_CURSOR));
      spinCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_XOR_SPIN_CURSOR));
      upCursor    = LoadCursor(hInst, MAKEINTRESOURCE(IDC_XOR_UP_CURSOR));
      walkCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_XOR_WALK_CURSOR));
      
      panCursor   = LoadCursor(hmod, MAKEINTRESOURCE(IDC_XOR_MANIP_CURSOR));
      dollyCursor = LoadCursor(hmod, MAKEINTRESOURCE(IDC_XOR_MANIP_CURSOR));
      pickCursor  = LoadCursor(hmod, MAKEINTRESOURCE(IDC_XOR_PICK_CURSOR));
      manipCursor = LoadCursor(hmod, MAKEINTRESOURCE(IDC_XOR_MANIP_CURSOR));
      break;
      
    case SoWinViewer::SHADOW:
      flyCursor   = LoadCursor(hInst, MAKEINTRESOURCE(IDC_SHADOW_FLY_CURSOR));
      rollCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_SHADOW_ROLL_CURSOR));
      seekCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_SHADOW_SEEK_CURSOR));
      spinCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_SHADOW_SPIN_CURSOR));
      upCursor    = LoadCursor(hInst, MAKEINTRESOURCE(IDC_SHADOW_UP_CURSOR));
      walkCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_SHADOW_WALK_CURSOR));
      
      panCursor   = LoadCursor(hmod, MAKEINTRESOURCE(IDC_SHADOW_MANIP_CURSOR));
      dollyCursor = LoadCursor(hmod, MAKEINTRESOURCE(IDC_SHADOW_MANIP_CURSOR));
      pickCursor  = LoadCursor(hmod, MAKEINTRESOURCE(IDC_SHADOW_PICK_CURSOR));
      manipCursor = LoadCursor(hmod, MAKEINTRESOURCE(IDC_SHADOW_MANIP_CURSOR));
      break;
      
    default:    
      flyCursor   = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CLASSIC_FLY_CURSOR));
      rollCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CLASSIC_ROLL_CURSOR));
      seekCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CLASSIC_SEEK_CURSOR));
      spinCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CLASSIC_SPIN_CURSOR));
      upCursor    = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CLASSIC_UP_CURSOR));
      walkCursor  = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CLASSIC_WALK_CURSOR));
      
      panCursor   = LoadCursor(hmod, MAKEINTRESOURCE(IDC_CLASSIC_MANIP_CURSOR));
      dollyCursor = LoadCursor(hmod, MAKEINTRESOURCE(IDC_CLASSIC_MANIP_CURSOR));
      pickCursor  = LoadCursor(hmod, MAKEINTRESOURCE(IDC_CLASSIC_PICK_CURSOR));
      manipCursor = LoadCursor(hmod, MAKEINTRESOURCE(IDC_CLASSIC_MANIP_CURSOR));
      break;
    }
  }
  createdCursors = TRUE;
}
