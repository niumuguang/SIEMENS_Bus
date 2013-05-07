/*|------------------------------------------------------------

   MAGELLAN Windows NT Driver  HEADER      Version 1.0 -6.10.95

   Logitech Inc.
   6505 Kaiser Drive
   Fremont, CA 94555-3615

   Volker Senft:

   Main:   (510) 795-8500
   Direct: (510) 713-4055
   Sales:  (800) 231-7717
   Fax:    (510) 505-0979

  06-Oct-95 G-01-08  VS   $$1 Link functions to the Magellan driver
  --------------------------------------------------------------|*/
#ifndef _MAGELLAN_NT_H_
#define _MAGELLAN_NT_H_

#ifndef TRUE
#define TRUE					(1==1)
#endif

#ifndef FALSE
#define FALSE					(1==0)
#endif

#define MagellanClassName 	"MAGELLAN_3D_CONTROLLER"
#define MagellanWindowName	"Magellan 3D Controller for Windows NT/95"

enum _MagellanEventTypes_    { MotionEvent=24869, ButtonPressEvent, ButtonReleaseEvent, CommandEvent };
enum _MagellanCommands_		 { MagellanSetWindowCommand = 27695, MagellanApplicationSensitivity, MagellanRingBell,
                               MagellanApplicationStarts, MagellanModeChange, MagellanNullRadiusChange,
                               MagellanControlPanel };
enum _MagellanApplications_  { SDRC_IDEAS, PROENGINEER, SOLIDWORKS, MICROSTATION, CUBE3D };

#define MagellanMotionEventType(Handle)			(Handle->MagellanMotionEvent)
#define MagellanButtonPressEventType(Handle)	(Handle->MagellanButtonPressEvent)
#define MagellanButtonReleaseEventType(Handle)	(Handle->MagellanButtonReleaseEvent)

enum _MagellanData_ { MagellanX, MagellanY, MagellanZ,
                      MagellanA, MagellanB, MagellanC };

struct _MagellanIntegerEvent_ {
 HWND MagellanWindow;
 int MagellanType;						/*| Event Type: MotionEvent, ButtonPressEvent, ButtonReleaseEvent |*/
 int MagellanButton;					/*| Button Number 1 .. 9                                          |*/
 int MagellanData[6];					/*| 3D Data Integer Value X,Y,Z and A,B,C                         |*/
 int MagellanPeriod;					/*| Data Period Time  60 ms                                       |*/
};
typedef struct _MagellanIntegerEvent_ MagellanIntegerEvent;

struct _MagellanControl_ {
 LONG MagellanUsageCounter;				/*| Counts the usage of the DLL             |*/
 HANDLE MagellanProcess;				/*| Process Handle of the Magellan Driver   |*/
 HANDLE MagellanThread;					/*| Thread Handle of Magellan Thread        |*/
 DWORD MagellanThreadID;				/*| Thread ID from CreateThread             |*/
 HWND MagellanWindow;					/*| Application Window                      |*/
 HWND MagellanDriverWindow;				/*| Magellan Driver Window                  |*/ 
 MagellanIntegerEvent MagellanEvent;	/*| Magellan Data, Motion and Button Events |*/
 HANDLE MagellanSerial;					/*| Serial Port Handle                      |*/
 DWORD MagellanMotionEvent;				/*| Motion Event registered Message         |*/
 DWORD MagellanButtonPressEvent;		/*| Button Press Event registered Message   |*/
 DWORD MagellanButtonReleaseEvent;		/*| Button Release Event registerd Message  |*/
 DWORD MagellanCommandEvent;			/*| Command Event registered Message       	|*/
};
typedef struct _MagellanControl_ MagellanControl;

typedef MagellanControl *HMAGELLAN;		/*| Handle Type |*/

struct _MagellanDataCompress_ {			/*| Compressed 3D Values |*/
 long ValueAX : 11;
 long ValueBY : 11;
 long ValueCZ : 10;
};
typedef struct _MagellanDataCompress_ MagellanDataCompress;

union _MagellanCompress_ {
 MagellanDataCompress MagellanValues;
 DWORD MagellanDWord;
};
typedef union _MagellanCompress_ MagellanCompress;

union _MagellanTypeConversion_ {
 LPARAM lParam;
 float  Float;
};
typedef union _MagellanTypeConversion_ MagellanTypeConversion;
				 

HMAGELLAN WINAPI MagellanInit( HWND );
int       WINAPI MagellanInfoWindow( HMAGELLAN, WPARAM, LPARAM );
int       WINAPI MagellanSetWindow( HMAGELLAN, HWND );
int       WINAPI MagellanTranslateEvent( HMAGELLAN, LPMSG, MagellanIntegerEvent *);
int       WINAPI MagellanClose( HMAGELLAN );

#endif /* _MAGELLAN_NT_H_ */
