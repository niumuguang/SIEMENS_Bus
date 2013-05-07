#include <DemoColliderDialog.h>
#include <DemoCollider.h>

/////////////////////////////////////////////////////////////
class NewStaticSceneAuditor : public SoMenuFileSelectionAuditor {
  DemoColliderDialog *dialog;
public:
  NewStaticSceneAuditor(DemoColliderDialog *dialog) { this->dialog = dialog; }

  void menuFileSelection(SoMenuFileSelection* fs) {
     SoSeparator *static_scene = dialog->readFile(fs->filename.getValue());
     dialog->getCollider()->newStaticScene(static_scene);
     dialog->getViewer()->viewAll();
  }
};


/////////////////////////////////////////////////////////////
class NewMovingSceneAuditor : public SoMenuFileSelectionAuditor {
  DemoColliderDialog *dialog;
public:
  NewMovingSceneAuditor(DemoColliderDialog *dialog) { this->dialog = dialog; }

  void menuFileSelection(SoMenuFileSelection* fs) {
     SoSeparator *static_scene = dialog->readFile(fs->filename.getValue());
     dialog->getCollider()->newMovingScene(static_scene);
     dialog->getViewer()->viewAll();
  }
};


/////////////////////////////////////////////////////////////
class MaxTriangleMovingSliderAuditor : public SoDialogIntegerSliderAuditor {
  DemoColliderDialog *dialog;
public:
  MaxTriangleMovingSliderAuditor(DemoColliderDialog *dialog) { this->dialog = dialog; }
  void dialogIntegerSlider(SoDialogIntegerSlider *slider) {
    dialog->getCollider()->setMaxTriangleForMoving(slider->value.getValue());
  }  
};


/////////////////////////////////////////////////////////////
class MaxTriangleStaticSliderAuditor : public SoDialogIntegerSliderAuditor {
  DemoColliderDialog *dialog;
public:
  MaxTriangleStaticSliderAuditor(DemoColliderDialog *dialog) { this->dialog = dialog; }
  void dialogIntegerSlider(SoDialogIntegerSlider *slider) {
    dialog->getCollider()->setMaxTriangleForStatic(slider->value.getValue());
  }  
};


/////////////////////////////////////////////////////////////
class ActivateAuditor : public SoDialogCheckBoxAuditor {
  DemoColliderDialog *dialog;
public:
  ActivateAuditor(DemoColliderDialog *dialog) { this->dialog = dialog; }
  void dialogCheckBox(SoDialogCheckBox *checkBox) {
    dialog->getCollider()->enableCollisions(checkBox->state.getValue());
  }
};


/////////////////////////////////////////////////////////////
DemoColliderDialog::DemoColliderDialog(Widget main_widget)
{
  v_MainWindow = new SoTopLevelDialog;
  v_MainWindow->position.setValue(300,0);
  v_MainWindow->label = "Demo Collider";
  SoMenuBar *v_MenuBar = (SoMenuBar *)SoDialogViz::loadFromFile("GuiMenuBar.iv");

  m_parentViewer = new SoDialogCustom();
  m_parentViewer->height = 400;
  m_parentViewer->width = 600;

  v_MainWindow->addChild(v_MenuBar);
  v_MainWindow->addChild(m_parentViewer);

  v_MainWindow->buildDialog(main_widget,TRUE);

  SoTopComponent  *v_settings = (SoTopComponent*)SoDialogViz::loadFromFile("GuiSettings.iv");
  v_TopSettings = new SoTopLevelDialog();
  v_TopSettings->addChild(v_settings);
  v_TopSettings->buildDialog(main_widget);

  m_messageFile = NULL;
}

/////////////////////////////////////////////////////////////
void 
DemoColliderDialog::init(DemoCollider *collider) 
{
  m_collider = collider;
  m_collider->setDialog(this);
  
  // Create a viewer
  m_viewer = new SoXtExaminerViewer(m_parentViewer->getWidget()); 
  m_viewer->setSceneGraph(m_collider->getRoot());

  buildAuditors();
  v_TopSettings->show();
  v_MainWindow->show();
}

/////////////////////////////////////////////////////////////
void 
DemoColliderDialog::buildAuditors()
{
  SoDialogCheckBox* activeCheck = (SoDialogCheckBox *)v_TopSettings->searchForAuditorId("ActiveCollision");
  activeCheck->addAuditor(new ActivateAuditor(this));

  SoDialogIntegerSlider* maxTriangleStaticSlider = (SoDialogIntegerSlider*)v_TopSettings->searchForAuditorId("MaxTriangleStaticSlider");
  maxTriangleStaticSlider->addAuditor(new MaxTriangleStaticSliderAuditor(this));

  SoDialogIntegerSlider* maxTriangleMovingSlider = (SoDialogIntegerSlider*)v_TopSettings->searchForAuditorId("MaxTriangleMovingSlider");
  maxTriangleMovingSlider->addAuditor(new MaxTriangleMovingSliderAuditor(this));

  SoMenuFileSelection* newStaticScene = (SoMenuFileSelection*)v_MainWindow->searchForAuditorId("NewStaticScene");
  newStaticScene->addAuditor(new NewStaticSceneAuditor(this));

  SoMenuFileSelection* newMovingScene = (SoMenuFileSelection*)v_MainWindow->searchForAuditorId("NewMovingScene");
  newMovingScene->addAuditor(new NewMovingSceneAuditor(this));

  m_messageFile = (SoDialogLabel*)v_TopSettings->searchForAuditorId("MessageFile");
  m_messageStatic = (SoDialogLabel*)v_TopSettings->searchForAuditorId("MessageStatic");
  m_messageMoving = (SoDialogLabel*)v_TopSettings->searchForAuditorId("MessageMoving");
  m_messageCollision = (SoDialogLabel*)v_TopSettings->searchForAuditorId("MessageCollision");
  m_messageWarning = (SoDialogLabel*)v_TopSettings->searchForAuditorId("MessageWarning");

 // newStaticScene->filename = v_IvBinaryFileName;

}


/////////////////////////////////////////////////////////////
SoSeparator *
DemoColliderDialog::readFile(const SbString &filename)
{
  if (m_messageFile != NULL) m_messageFile->label = "Reading file started";

  // Open the input file
  SoInput mySceneInput;
  if (!mySceneInput.openFile(filename.getString())) {
    fprintf(stderr, "Cannot open file %s\n", filename.getString());
    return NULL;
  }
  
  // Read the whole file into the database
  SoSeparator *myGraph = SoDB::readAll(&mySceneInput);
  if (myGraph == NULL) {
    fprintf(stderr, "Problem reading file\n");
    return NULL;
  } 
  
  mySceneInput.closeFile();
  if (m_messageFile != NULL) m_messageFile->label = "Reading file finished";

  return myGraph;
}

/////////////////////////////////////////////////////////////
void 
DemoColliderDialog::messageStatic(const char *msg)
{
  m_messageStatic->label = msg;
}

/////////////////////////////////////////////////////////////
void 
DemoColliderDialog::messageMoving(const char *msg)
{
  m_messageMoving->label = msg;
}

/////////////////////////////////////////////////////////////
void 
DemoColliderDialog::messageCollision(const char *msg)
{
  m_messageCollision->label = msg;
}

/////////////////////////////////////////////////////////////
void 
DemoColliderDialog::messageWarning(const char *msg)
{
  m_messageWarning->label = msg;
}

