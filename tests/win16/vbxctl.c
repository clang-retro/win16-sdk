/* A Visual Basic custom control (VBX): a counter. A click adds 1 to the
 * Value property and fires the Click event; the control shows the value.
 * It tests vbapi.h and the stubs in vbapi.lib. Built with -shared and
 * linked with -lvbapi, it imports only the Windows modules, and it
 * exports VBINITCC, VBTERMCC and VBGETMODELINFO. */
#include <windows.h>
#include <vbapi.h>

/* The data of each counter control. */
typedef struct {
  long value;
} COUNTER;
typedef COUNTER FAR *PCOUNTER;

/* The layouts that Visual Basic reads: packed to 1 byte, near pointers. */
_Static_assert(sizeof(MODEL) == 35, "MODEL");
_Static_assert(sizeof(PROPINFO) == 15, "PROPINFO");
_Static_assert(sizeof(EVENTINFO) == 14, "EVENTINFO");
_Static_assert(sizeof(MODELINFO) == 6, "MODELINFO");
_Static_assert(sizeof(DATASTRUCT) == 12, "DATASTRUCT");
_Static_assert(sizeof(PIC) == 11, "PIC");

static HANDLE hmodDLL;

static char szValue[] = "Value";
static PROPINFO propValue = {szValue, DT_LONG | PF_fGetData | PF_fSetData |
                                          PF_fSaveData,
                             0, 0, 0, NULL, 0};

static PPROPINFO props[] = {PPROPINFO_STD_CTLNAME, PPROPINFO_STD_INDEX,
                            PPROPINFO_STD_LEFT,    PPROPINFO_STD_TOP,
                            PPROPINFO_STD_WIDTH,   PPROPINFO_STD_HEIGHT,
                            &propValue,            NULL};

static PEVENTINFO events[] = {PEVENTINFO_STD_CLICK, NULL};

static char szDefName[] = "Counter";
static char szClass[] = "Counter";

/* The control procedure: paints the value, and counts clicks. */
LONG FAR PASCAL __export CounterProc(HCTL hctl, HWND hwnd, USHORT msg,
                                            USHORT wp, LONG lp) {
  PCOUNTER pc = (PCOUNTER)VBDerefControl(hctl);
  switch (msg) {
  case WM_LBUTTONDOWN:
    pc->value++;
    InvalidateRect(hwnd, NULL, TRUE);
    VBFireEvent(hctl, IEVENT_STD_CLICK, NULL);
    return 0;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    char buf[16];
    HDC hdc = BeginPaint(hwnd, &ps);
    int n = wsprintf(buf, "%ld", pc->value);
    TextOut(hdc, 2, 2, buf, n);
    EndPaint(hwnd, &ps);
    return 0;
  }
  }
  return VBDefControlProc(hctl, hwnd, msg, wp, lp);
}

static MODEL model = {
    VB100_VERSION,                 /* usVersion: works with VB 1.0 and up */
    MODEL_fFocusOk,                /* fl */
    (PCTLPROC)CounterProc,         /* pctlproc */
    CS_VREDRAW | CS_HREDRAW,       /* fsClassStyle */
    0L,                            /* flWndStyle */
    sizeof(COUNTER),               /* cbCtlExtra */
    0,                             /* idBmpPalette */
    szDefName,                     /* npszDefCtlName */
    szClass,                       /* npszClassName */
    NULL,                          /* npszParentClassName */
    props,                         /* npproplist */
    events,                        /* npeventlist */
    6,                             /* nDefProp: Value */
    0,                             /* nDefEvent: Click */
    6,                             /* nValueProp: Value */
    1,                             /* usCtlVersion */
};

static LPMODEL models[] = {&model, NULL};
static MODELINFO modelinfo = {VB100_VERSION, models};

/* The DLL entry point: keeps the module handle for VBRegisterModel. */
int FAR PASCAL LibMain(HANDLE hinst, WORD ds, WORD heap, LPSTR cmd) {
  (void)ds, (void)heap, (void)cmd;
  hmodDLL = hinst;
  return 1;
}

/* Visual Basic calls this when it loads the VBX: registers the model. */
BOOL FAR PASCAL __export VBINITCC(USHORT usVersion, BOOL fRuntime) {
  (void)usVersion, (void)fRuntime;
  return VBRegisterModel(hmodDLL, &model) && VBGetVersion() >= VB100_VERSION;
}

/* Visual Basic calls this before it unloads the VBX. */
VOID FAR PASCAL __export VBTERMCC(VOID) {}

/* Visual Basic 2.0 and later call this to get the models. */
LPMODELINFO FAR PASCAL __export VBGetModelInfo(USHORT usVersion) {
  (void)usVersion;
  return &modelinfo;
}
