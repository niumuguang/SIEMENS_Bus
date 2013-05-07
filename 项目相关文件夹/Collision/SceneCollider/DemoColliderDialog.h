#ifndef  _DEMO_COLLIDER_DIALOG_
#define  _DEMO_COLLIDER_DIALOG_

#include <DialogViz/SoDialogVizAll.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

class DemoCollider;

/////////////////////////////////////////////////////////////
class DemoColliderDialog {

public:
  DemoColliderDialog() {}
  DemoColliderDialog(Widget main_widget);
  Widget getWidget();

  void init(DemoCollider *collider);

  DemoCollider* getCollider() const {
    return m_collider;
  }

  SoXtExaminerViewer* getViewer() const {
    return m_viewer;
  }

  SoSeparator *readFile(const SbString &filename);

  void messageStatic(const char *);
  void messageMoving(const char *);
  void messageCollision(const char *);
  void messageWarning(const char *);

private:
  void buildAuditors();
  DemoCollider *m_collider;

  SoTopLevelDialog *v_TopSettings;
  SoTopLevelDialog *v_MainWindow;
  SoXtExaminerViewer *m_viewer;
  SoDialogLabel *m_messageFile;
  SoDialogLabel *m_messageStatic;
  SoDialogLabel *m_messageMoving;
  SoDialogLabel *m_messageCollision;
  SoDialogLabel *m_messageWarning;
  SoDialogCustom *m_parentViewer;
};

#endif

