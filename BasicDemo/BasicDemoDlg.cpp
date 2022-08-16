// Shrimp Counting Project
// This code implemented by The Nho 2022
// Interfacing with Camera HikRobot
// Sort Tracking
// Pass line counting
// BasicDemoDlg.cpp : implementation file
#include "stdafx.h"
#include "BasicDemo.h"
#include "BasicDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Global variable
// Parameters Setting 
// using global to load parameters to setting window
extern CString blur_method;
extern int blur_kernel;
extern CString segment_binary_method;
extern CString bgs_method;
extern float bgs_threshold;
extern CString bgs_shadows;
extern int bgs_history;
extern CString adaptiveThreshold_method;
extern int adaptiveThreshold_KSize;
extern int adaptiveThreshold_C;
extern CString morphological_method;
extern int morphological_kernel;
extern int morphological_iterations;
extern int line_position;
extern float max_distance;
extern CString couting_method;
extern float  distance_threshold;
extern int min_hits;
extern int max_age;
extern int tolerance_x;
extern double avg_area;
extern double min_area;
extern double max_area;
extern int min_width;
extern int min_height;
extern int max_width;
extern int max_height;

// global filename, used in all windows
extern CString global_filename;

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog {
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CBasicDemoDlg dialog
CBasicDemoDlg::CBasicDemoDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CBasicDemoDlg::IDD, pParent)
    , m_pcMyCamera(NULL)
    , m_nDeviceCombo(0)
    , m_bOpenDevice(FALSE)
    , m_bStartGrabbing(FALSE)
    , m_hGrabThread(NULL)
    , m_bThreadState(FALSE)
    , m_nTriggerMode(MV_TRIGGER_MODE_OFF)
    , m_dExposureEdit(0)
    , m_dGainEdit(0)
    , m_dFrameRateEdit(0)
    , m_bSoftWareTriggerCheck(FALSE)
    , m_nTriggerSource(MV_TRIGGER_SOURCE_SOFTWARE)
    , m_pSaveImageBuf(NULL)
    , m_nSaveImageBufSize(0)
    , counter(0)
    , frame_count(0)
{
    // Load icon
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hIcon = AfxGetApp()->LoadIcon(IDI_HPC_ICON);
    memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
}

void CBasicDemoDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DEVICE_COMBO, m_ctrlDeviceCombo);
    DDX_CBIndex(pDX, IDC_DEVICE_COMBO, m_nDeviceCombo);
    DDX_Text(pDX, IDC_EXPOSURE_EDIT, m_dExposureEdit);
    DDX_Text(pDX, IDC_GAIN_EDIT, m_dGainEdit);
    DDX_Text(pDX, IDC_FRAME_RATE_EDIT, m_dFrameRateEdit);
    DDX_Check(pDX, IDC_SOFTWARE_TRIGGER_CHECK, m_bSoftWareTriggerCheck);
    DDX_Text(pDX, IDC_SHIRMP_NUMBER_STATIC, counter);
    DDX_Text(pDX, IDC_FRAME_COUNT_EDIT, frame_count);
    
}

BEGIN_MESSAGE_MAP(CBasicDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// }}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_ENUM_BUTTON, &CBasicDemoDlg::OnBnClickedEnumButton)
    ON_BN_CLICKED(IDC_OPEN_BUTTON, &CBasicDemoDlg::OnBnClickedOpenButton)
    ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CBasicDemoDlg::OnBnClickedCloseButton)
    ON_BN_CLICKED(IDC_CONTINUS_MODE_RADIO, &CBasicDemoDlg::OnBnClickedContinusModeRadio)
    ON_BN_CLICKED(IDC_TRIGGER_MODE_RADIO, &CBasicDemoDlg::OnBnClickedTriggerModeRadio)
    ON_BN_CLICKED(IDC_START_GRABBING_BUTTON, &CBasicDemoDlg::OnBnClickedStartGrabbingButton)
    ON_BN_CLICKED(IDC_STOP_GRABBING_BUTTON, &CBasicDemoDlg::OnBnClickedStopGrabbingButton)
    ON_BN_CLICKED(IDC_GET_PARAMETER_BUTTON, &CBasicDemoDlg::OnBnClickedGetParameterButton)
    ON_BN_CLICKED(IDC_SET_PARAMETER_BUTTON, &CBasicDemoDlg::OnBnClickedSetParameterButton)
    ON_BN_CLICKED(IDC_SOFTWARE_TRIGGER_CHECK, &CBasicDemoDlg::OnBnClickedSoftwareTriggerCheck)
    ON_BN_CLICKED(IDC_SOFTWARE_ONCE_BUTTON, &CBasicDemoDlg::OnBnClickedSoftwareOnceButton)
    ON_BN_CLICKED(IDC_SAVE_BMP_BUTTON, &CBasicDemoDlg::OnBnClickedSaveBmpButton)
    ON_BN_CLICKED(IDC_SAVE_JPG_BUTTON, &CBasicDemoDlg::OnBnClickedSaveJpgButton)
    ON_BN_CLICKED(IDC_SAVE_TIFF_BUTTON, &CBasicDemoDlg::OnBnClickedSaveTiffButton)
    ON_BN_CLICKED(IDC_SAVE_PNG_BUTTON, &CBasicDemoDlg::OnBnClickedSavePngButton)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_SETTING_BUTTON, &CBasicDemoDlg::OnBnClickedSettingButton)
    ON_BN_CLICKED(IDC_START_COUNT_BUTTON, &CBasicDemoDlg::OnBnClickedStartCountButton)
    ON_BN_CLICKED(IDC_STOP_COUNT_BUTTON, &CBasicDemoDlg::OnBnClickedStopCountButton)
    ON_BN_CLICKED(IDC_RESET_NUMBER_BUTTON, &CBasicDemoDlg::OnBnClickedResetNumberButton)
    ON_BN_CLICKED(IDC_LOG_BUTTON, &CBasicDemoDlg::OnBnClickedLogButton)
END_MESSAGE_MAP()

//en:Grabbing thread
unsigned int __stdcall GrabThread(void* pUser) {
    if (pUser) {
        CBasicDemoDlg* pCam = (CBasicDemoDlg*)pUser;

        pCam->GrabThreadProcess();
        
        return 0;
    }

    return -1;
}

// CBasicDemoDlg message handlers
BOOL CBasicDemoDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
    // Set font for shrimp number in dialog
    Font_big.CreatePointFont(400, _T("Microsoft Sans Serif"));
    GetDlgItem(IDC_SHIRMP_NUMBER_STATIC)->SetFont(&Font_big);
    
	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	DisplayWindowInitial();             //en:Display Window Initialization

    InitializeCriticalSection(&m_hSaveImageMux);

    SettingInitial();

    UpdateData(FALSE); // update intial data to windown

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBasicDemoDlg::SettingInitial() {
    // install the first run variables
    blur_method = L"AVG"; //AVG;GAUSS;MEDIAN
    blur_kernel = 3;
    segment_binary_method = L"Background Subtraction"; // ; Adaptive Threshold; Background Subtraction
    bgs_method = L"MOG2"; //MOG2;MOG
    bgs_threshold = 15;
    bgs_shadows = L"False"; //True
    bgs_history = 500;
    adaptiveThreshold_method = L"MEAN_C"; //GAUSSIAN_C
    adaptiveThreshold_KSize = 25;
    adaptiveThreshold_C = 5;
    morphological_method = L"Open"; //Close;Erode;Dilate
    morphological_kernel = 3;
    morphological_iterations = 1;
    line_position = 400;
    max_distance = 100; // 20 pixels
    couting_method = L"SORT"; //L"SORT" ; L"My Simple Tracking"
    distance_threshold = 15; // 10 pixel
    min_hits = 1;
    max_age = 3;
    tolerance_x = 5;
    avg_area = 100;
    min_area = 10;
    max_area = 1000;
    min_width = 1;
    min_height = 1;
    max_width = 50;
    max_height = 50;

    // initialize directory global save result file, use global variable in many sub-window
    global_filename = nFilename;

    // Segmentation
    if (segment_binary_method == L"Adaptive Threshold") {
        Size Adap_size = Size(adaptiveThreshold_KSize, adaptiveThreshold_KSize);
        if (adaptiveThreshold_method == L"MEAN_C") {
            gpu_adaptiveThreshold_filter = cuda::createBoxFilter(CV_8U, CV_8U, Adap_size);
        }
        else if (adaptiveThreshold_method == L"GAUSSIAN_C") {
            gpu_adaptiveThreshold_filter = cuda::createGaussianFilter(CV_8U, CV_8U, Adap_size, 1);
        }
        else {
            AfxMessageBox(L"Adaptive Threshold Initial Error!");
            return;
        }
    }
    else if (segment_binary_method == L"Background Subtraction") {
        if (bgs_method == L"MOG2") {
            if (bgs_shadows == L"True") {
                pBackSub = cuda::createBackgroundSubtractorMOG2(bgs_history, bgs_threshold, true);
            }
            else if (bgs_shadows == L"False") {
                pBackSub = cuda::createBackgroundSubtractorMOG2(bgs_history, bgs_threshold, false);
            }
            else {
                AfxMessageBox(L"Shadow Error!");
                return;
            }
        }
        else if (bgs_method == L"MOG") {
            pBackSub = cuda::createBackgroundSubtractorMOG(bgs_history, 5, 0.7);
        }
        else {
            AfxMessageBox(L"Backgruond Subtraction Initail Error!");
            return;
        }
    }
    else {
        AfxMessageBox(L"Segmentation Initial to Binary Error!");
        return;
    }
    // cuda blur image
    if (blur_method == L"MEDIAN") {
        cuda_filter = createGaussianFilter(CV_8U, CV_8U, Size(blur_kernel, blur_kernel), 1);
    }
    else if (blur_method == L"AVG") {
        cuda_filter = cuda::createBoxFilter(CV_8U, CV_8U, Size(blur_kernel, blur_kernel));
    }
    else if (blur_method == L"GAUSS") {
        cuda_filter = cuda::createMedianFilter(CV_8U, blur_kernel);
    }
    else {
        AfxMessageBox(L"Blur Method Initial Error!");
        return;
    }
    // Morphological filter
    mo_kernel = getStructuringElement(MORPH_RECT, Size(morphological_kernel, morphological_kernel));
    if (morphological_method == L"Dilate") {
        mo_filter = cuda::createMorphologyFilter(MORPH_DILATE, CV_8U, mo_kernel, Point(-1, -1), morphological_iterations);
    }
    else if (morphological_method == L"Erode") {
        mo_filter = cuda::createMorphologyFilter(MORPH_ERODE, CV_8U, mo_kernel, Point(-1, -1), morphological_iterations);
    }
    else if (morphological_method == L"Open") {
        mo_filter = cuda::createMorphologyFilter(MORPH_OPEN, CV_8U, mo_kernel, Point(-1, -1), morphological_iterations);
    }
    else if (morphological_method == L"Close") {
        mo_filter = cuda::createMorphologyFilter(MORPH_CLOSE, CV_8U, mo_kernel, Point(-1, -1), morphological_iterations);
    }
    else {
        AfxMessageBox(L"Morphological method Initail Error!");
        return;
    }

}

void CBasicDemoDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CBasicDemoDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBasicDemoDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

//en:Enable/Disable some controls when you call this function
void CBasicDemoDlg::EnableControls(BOOL bIsCameraReady) {
    GetDlgItem(IDC_OPEN_BUTTON)->EnableWindow(m_bOpenDevice ? FALSE : (bIsCameraReady ? TRUE : FALSE));
    GetDlgItem(IDC_CLOSE_BUTTON)->EnableWindow((m_bOpenDevice && bIsCameraReady && !m_bStartGrabbing) ? TRUE : FALSE);
    GetDlgItem(IDC_START_GRABBING_BUTTON)->EnableWindow((m_bStartGrabbing && bIsCameraReady) ? FALSE : (m_bOpenDevice ? TRUE : FALSE));
    GetDlgItem(IDC_STOP_GRABBING_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SOFTWARE_TRIGGER_CHECK)->EnableWindow(m_bOpenDevice && ((CButton*)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->GetCheck() ? TRUE : FALSE);
    GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON)->EnableWindow(m_bStartGrabbing && m_bSoftWareTriggerCheck? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_BMP_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_TIFF_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_PNG_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_JPG_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_EXPOSURE_EDIT)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_GAIN_EDIT)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_FRAME_RATE_EDIT)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_GET_PARAMETER_BUTTON)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SET_PARAMETER_BUTTON)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_CONTINUS_MODE_RADIO)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_TRIGGER_MODE_RADIO)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_SETTING_BUTTON)->EnableWindow(m_bStartGrabbing ? FALSE : TRUE);
    GetDlgItem(IDC_START_COUNT_BUTTON)->EnableWindow(!b_start_count && m_bStartGrabbing ? TRUE : FALSE); //flow stop grabbing button
    GetDlgItem(IDC_STOP_COUNT_BUTTON)->EnableWindow(b_start_count ? TRUE : FALSE); // enable when counting
}

//en:Initial window initialization / SDK function
void CBasicDemoDlg::DisplayWindowInitial() {
    CWnd *pWnd = GetDlgItem(IDC_DISPLAY_STATIC);
    if (pWnd) {
        m_hwndDisplay = pWnd->GetSafeHwnd();
        if (m_hwndDisplay) {
            EnableControls(FALSE);
        }
    }
}

//en:Show error message / SDK function
void CBasicDemoDlg::ShowErrorMsg(CString csMessage, int nErrorNum) {
    CString errorMsg;
    if (nErrorNum == 0) {
        errorMsg =  csMessage;
    }
    else {
        CString nE;
        nE.Format(L"%d", nErrorNum);
        errorMsg = csMessage + L": Error=" + nE + L"\n";
    }

    switch(nErrorNum) {
    case MV_E_HANDLE:           errorMsg += "Error or invalid handle ";                                         break;
    case MV_E_SUPPORT:          errorMsg += "Not supported function ";                                          break;
    case MV_E_BUFOVER:          errorMsg += "Cache is full ";                                                   break;
    case MV_E_CALLORDER:        errorMsg += "Function calling order error ";                                    break;
    case MV_E_PARAMETER:        errorMsg += "Incorrect parameter ";                                             break;
    case MV_E_RESOURCE:         errorMsg += "Applying resource failed ";                                        break;
    case MV_E_NODATA:           errorMsg += "No data ";                                                         break;
    case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed ";              break;
    case MV_E_VERSION:          errorMsg += "Version mismatches ";                                              break;
    case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory ";                                             break;
    case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
    case MV_E_UNKNOW:           errorMsg += "Unknown error ";                                                   break;
    case MV_E_GC_GENERIC:       errorMsg += "General error ";                                                   break;
    case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";                                  break;
    case MV_E_ACCESS_DENIED:	errorMsg += "No permission ";                                                   break;
    case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
    case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
    }

    MessageBox(errorMsg, TEXT("PROMPT"), MB_OK | MB_ICONWARNING);
}

//en:Close Device
int CBasicDemoDlg::CloseDevice() {
    m_bThreadState = FALSE;
    if (m_hGrabThread) {
        WaitForSingleObject(m_hGrabThread, INFINITE);
        CloseHandle(m_hGrabThread);
        m_hGrabThread = NULL;
    }

    if (m_pcMyCamera) {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    m_bStartGrabbing = FALSE;
    m_bOpenDevice = FALSE;

    if (m_pSaveImageBuf) {
        free(m_pSaveImageBuf);
        m_pSaveImageBuf = NULL;
    }
    m_nSaveImageBufSize = 0;

    return MV_OK;
}

//en:Get Trigger Mode / SDK function
int CBasicDemoDlg::GetTriggerMode() {
    MVCC_ENUMVALUE stEnumValue = {0};

    int nRet = m_pcMyCamera->GetEnumValue("TriggerMode", &stEnumValue);
    if (MV_OK != nRet) {
        return nRet;
    }

    m_nTriggerMode = stEnumValue.nCurValue;
    if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode) {
        OnBnClickedTriggerModeRadio();
    }
    else {
        m_nTriggerMode = MV_TRIGGER_MODE_OFF;
        OnBnClickedContinusModeRadio();
    }

    return MV_OK;
}

// en:Set Trigger Mode / SDK function
int CBasicDemoDlg::SetTriggerMode()
{
    return m_pcMyCamera->SetEnumValue("TriggerMode", m_nTriggerMode);
}

//en:Get Exposure Time / SDK function
int CBasicDemoDlg::GetExposureTime() {
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &stFloatValue);
    if (MV_OK != nRet) {
        return nRet;
    }

    m_dExposureEdit = stFloatValue.fCurValue;

    return MV_OK;
}

// en:Set Exposure Time / SDK function
int CBasicDemoDlg::SetExposureTime() {
    // en:Adjust these two exposure mode to allow exposure time effective
    int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
    if (MV_OK != nRet) {
        return nRet;
    }

    m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

    return m_pcMyCamera->SetFloatValue("ExposureTime", (float)m_dExposureEdit);
}

// en:Get Gain / SDK function
int CBasicDemoDlg::GetGain() {
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("Gain", &stFloatValue);
    if (MV_OK != nRet) {
        return nRet;
    }
    m_dGainEdit = stFloatValue.fCurValue;

    return MV_OK;
}

//en:Set Gain / SDK function
int CBasicDemoDlg::SetGain() {
    //en:Set Gain after Auto Gain is turned off, this failure does not need to return
    m_pcMyCamera->SetEnumValue("GainAuto", 0);

    return m_pcMyCamera->SetFloatValue("Gain", (float)m_dGainEdit);
}

//en:Get Frame Rate / SDK function
int CBasicDemoDlg::GetFrameRate() {
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("ResultingFrameRate", &stFloatValue);
    if (MV_OK != nRet) {
        return nRet;
    }
    m_dFrameRateEdit = stFloatValue.fCurValue;

    return MV_OK;
}

//en:Set Frame Rate / SDK function
int CBasicDemoDlg::SetFrameRate() {
    int nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet) {
        return nRet;
    }

    return m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", (float)m_dFrameRateEdit);
}

//en:Get Trigger Source / SDK function
int CBasicDemoDlg::GetTriggerSource() {
    MVCC_ENUMVALUE stEnumValue = {0};

    int nRet = m_pcMyCamera->GetEnumValue("TriggerSource", &stEnumValue);
    if (MV_OK != nRet) {
        return nRet;
    }

    if ((unsigned int)MV_TRIGGER_SOURCE_SOFTWARE == stEnumValue.nCurValue) {
        m_bSoftWareTriggerCheck = TRUE;
    }
    else {
        m_bSoftWareTriggerCheck = FALSE;
    }

    return MV_OK;
}

//en:Set Trigger Source / SDK function
int CBasicDemoDlg::SetTriggerSource() {
    int nRet = MV_OK;
    if (m_bSoftWareTriggerCheck) {
        m_nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
        nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet) {
            ShowErrorMsg(TEXT("Set Software Trigger Fail"), nRet);
            return nRet;
        }
        GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(TRUE);
    }
    else {
        m_nTriggerSource = MV_TRIGGER_SOURCE_LINE0;
        nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet) {
            ShowErrorMsg(TEXT("Set Hardware Trigger Fail"), nRet);
            return nRet;
        }
        GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(FALSE);
    }

    return nRet;
}

//en:Save Image / SDK function
int CBasicDemoDlg::SaveImage(enum MV_SAVE_IAMGE_TYPE enSaveImageType) {
    MV_SAVE_IMG_TO_FILE_PARAM stSaveFileParam;
    memset(&stSaveFileParam, 0, sizeof(MV_SAVE_IMG_TO_FILE_PARAM));

    EnterCriticalSection(&m_hSaveImageMux);
    if (m_pSaveImageBuf == NULL || m_stImageInfo.enPixelType == 0) {
        LeaveCriticalSection(&m_hSaveImageMux);
        return MV_E_NODATA;
    }

    if(RemoveCustomPixelFormats(m_stImageInfo.enPixelType)) {
        LeaveCriticalSection(&m_hSaveImageMux);
        return MV_E_SUPPORT;
    }

    stSaveFileParam.enImageType = enSaveImageType; // en:Image format to save
    stSaveFileParam.enPixelType = m_stImageInfo.enPixelType;  // en:Camera pixel type
    stSaveFileParam.nWidth      = m_stImageInfo.nWidth;         // en:Width
    stSaveFileParam.nHeight     = m_stImageInfo.nHeight;          //en:Height
    stSaveFileParam.nDataLen    = m_stImageInfo.nFrameLen;
    stSaveFileParam.pData       = m_pSaveImageBuf;
    stSaveFileParam.iMethodValue = 0;

    //en:jpg image nQuality range is (50-99], png image nQuality range is [0-9]
    if (MV_Image_Bmp == stSaveFileParam.enImageType) {
        sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.bmp", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Jpeg == stSaveFileParam.enImageType) {
        stSaveFileParam.nQuality = 80;
        sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.jpg", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Tif == stSaveFileParam.enImageType) {
        sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.tif", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Png == stSaveFileParam.enImageType) {
        stSaveFileParam.nQuality = 8;
        sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.png", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }

    int nRet = m_pcMyCamera->SaveImageToFile(&stSaveFileParam);
    LeaveCriticalSection(&m_hSaveImageMux);

    return nRet;
}

// importance function!
// grap image from buffer, copy to cv Mat
int CBasicDemoDlg::GrabThreadProcess() {

    // for debug
    String dir_file = "C:/Users/ADMIN/Desktop/Data3/";
    uint idx_img = 0;
    //VideoCapture cap = VideoCapture(video_file);

    MV_FRAME_OUT stImageInfo = {0}; // get image from buffer
    
    int nRet = MV_OK;
    double start_time_fps = 0;
    // Thread 0
    while(m_bThreadState) {
        nRet = m_pcMyCamera->GetImageBuffer(&stImageInfo, 1000);
        if (nRet == MV_OK) {
            //Enter Critical Thread get save image buffer
            EnterCriticalSection(&m_hSaveImageMux);
            if (NULL == m_pSaveImageBuf || stImageInfo.stFrameInfo.nFrameLen > m_nSaveImageBufSize) {
                if (m_pSaveImageBuf) {
                    free(m_pSaveImageBuf);
                    m_pSaveImageBuf = NULL;
                }

                m_pSaveImageBuf = (unsigned char *)malloc(sizeof(unsigned char) * stImageInfo.stFrameInfo.nFrameLen);
                if (m_pSaveImageBuf == NULL) {
                    LeaveCriticalSection(&m_hSaveImageMux);
                    return 0;
                }
                m_nSaveImageBufSize = stImageInfo.stFrameInfo.nFrameLen;
            }
            // copy buffer to save
            memcpy(m_pSaveImageBuf, stImageInfo.pBufAddr, stImageInfo.stFrameInfo.nFrameLen);
            //copy infomation to save
            memcpy(&m_stImageInfo, &(stImageInfo.stFrameInfo), sizeof(MV_FRAME_OUT_INFO_EX));
            LeaveCriticalSection(&m_hSaveImageMux);
            // Rmove format, free buffer
            if(RemoveCustomPixelFormats(stImageInfo.stFrameInfo.enPixelType)) {
                m_pcMyCamera->FreeImageBuffer(&stImageInfo);
                continue;
            }
            // convert data to cv Mat
            bool retm = Convert2Mat(&stImageInfo.stFrameInfo, stImageInfo.pBufAddr, &Mat_src);
            if (retm == false || Mat_src.empty()) {
                AfxMessageBox(L"Error while convert data to cv Mat!");
                return 0;
            }

            // for debug
            //cap >> Mat_src;
            String img_file = dir_file + to_string(idx_img) + ".bmp";
            Mat_src = imread(img_file, 0);
            //resize(Mat_src, Mat_src, Size(640, 480));
            //Mat_src = Mat_src(Rect(30, 0, 600, 480));
            //cv::cvtColor(Mat_src, Mat_src, COLOR_BGR2GRAY);
            //retm = true;
            ///

            /// Press start count?
            if (b_start_count==true) {
                // image processing
                ImageProcessing_GPU();
                
                // Counting method
                if (couting_method == L"My Simple Tracking")
                    My_Simple_Counting(tolerance_x, max_distance);
                else if (couting_method == L"SORT") {
                    SORT(max_age, min_hits, distance_threshold);
                    SORT_Counting();
                }
                else
                    return 0;

                // for debug
                imshow("dst image", dst);
            }
            
            m_pcMyCamera->FreeImageBuffer(&stImageInfo);

            int64 end_time_fps = getTickCount();
            real_fps = int(getTickFrequency() / (end_time_fps - start_time_fps));
            start_time_fps = end_time_fps;

            
        }
        else {
            if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode) {
                Sleep(5);
            }
        }
        
        // for debuge
        if (idx_img >= 227)
            break;
        idx_img++;
        waitKey();
    }
    // end thread, destroy all window
    cv::destroyAllWindows();

    return MV_OK;
}

//en:Click Find Device button:Enumeration / SDK function
void CBasicDemoDlg::OnBnClickedEnumButton() {
    CString strMsg;

    m_ctrlDeviceCombo.ResetContent();
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    //en:Enumerate all devices within subnet
    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (MV_OK != nRet) {
        return;
    }

    // en:Add value to the information list box and display
    for (unsigned int i = 0; i < m_stDevList.nDeviceNum; i++) {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (NULL == pDeviceInfo) {
            continue;
        }

        wchar_t* pUserName = NULL;
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE) {
            int nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            int nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            int nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            int nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

            if (strcmp("", (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0) {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else {
                char strUserName[256] = {0};
                sprintf_s(strUserName, 256, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]GigE:    %s  (%d.%d.%d.%d)"), i, pUserName, nIp1, nIp2, nIp3, nIp4);
        }
        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE) {
            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0) {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else {
                char strUserName[256] = {0};
                sprintf_s(strUserName, 256, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]UsbV3:  %s"), i, pUserName);
        }
        else {
            ShowErrorMsg(TEXT("Unknown device enumerated"), 0);
        }
        m_ctrlDeviceCombo.AddString(strMsg);

        if (pUserName) {
            delete[] pUserName;
            pUserName = NULL;
        }
    }

    if (0 == m_stDevList.nDeviceNum) {
        ShowErrorMsg(TEXT("No device"), 0);
        return;
    }
    m_ctrlDeviceCombo.SetCurSel(0);

    EnableControls(TRUE);
}

// en:Click Open button: Open Device / SDK function
void CBasicDemoDlg::OnBnClickedOpenButton() {
    if (TRUE == m_bOpenDevice || NULL != m_pcMyCamera) {
        return;
    }
    UpdateData(TRUE);

    int nIndex = m_nDeviceCombo;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM)) {
        ShowErrorMsg(TEXT("Please select device"), 0);
        return;
    }

    //en:Device instance created by device information
    if (NULL == m_stDevList.pDeviceInfo[nIndex]) {
        ShowErrorMsg(TEXT("Device does not exist"), 0);
        return;
    }

    m_pcMyCamera = new CMvCamera;
    if (NULL == m_pcMyCamera) {
        return;
    }

    int nRet = m_pcMyCamera->Open(m_stDevList.pDeviceInfo[nIndex]);
    if (MV_OK != nRet) {
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
        ShowErrorMsg(TEXT("Open Fail"), nRet);
        return;
    }

    //en:Detection network optimal package size(It only works for the GigE camera)
    if (m_stDevList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE) {
        unsigned int nPacketSize = 0;
        nRet = m_pcMyCamera->GetOptimalPacketSize(&nPacketSize);
        if (nRet == MV_OK) {
            nRet = m_pcMyCamera->SetIntValue("GevSCPSPacketSize",nPacketSize);
            if(nRet != MV_OK) {
                ShowErrorMsg(TEXT("Warning: Set Packet Size fail!"), nRet);
            }
        }
        else {
            ShowErrorMsg(TEXT("Warning: Get Packet Size fail!"), nRet);
        }
    }

    m_bOpenDevice = TRUE;
    EnableControls(TRUE);
    OnBnClickedGetParameterButton(); //en:Get Parameter
}

//en:Click Close button: Close Device
void CBasicDemoDlg::OnBnClickedCloseButton() {
    CloseDevice();
    EnableControls(TRUE);
}

//en:Click Continues button / SDK function
void CBasicDemoDlg::OnBnClickedContinusModeRadio() {
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(TRUE);
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(FALSE);
    ((CButton *)GetDlgItem(IDC_SOFTWARE_TRIGGER_CHECK))->EnableWindow(FALSE);
    m_nTriggerMode = MV_TRIGGER_MODE_OFF;
    int nRet = SetTriggerMode();
    if (MV_OK != nRet) {
        return;
    }
    GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON)->EnableWindow(FALSE);
} 

//en:Click Trigger Mode button / SDK function
void CBasicDemoDlg::OnBnClickedTriggerModeRadio() {
    UpdateData(TRUE);
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(FALSE);
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(TRUE);
    ((CButton *)GetDlgItem(IDC_SOFTWARE_TRIGGER_CHECK))->EnableWindow(TRUE);
    m_nTriggerMode = MV_TRIGGER_MODE_ON;
    int nRet = SetTriggerMode();
    if (MV_OK != nRet) {
        ShowErrorMsg(TEXT("Set Trigger Mode Fail"), nRet);
        return;
    }

    if (m_bStartGrabbing == TRUE) {
        if (TRUE == m_bSoftWareTriggerCheck) {
            GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(TRUE);
        }
    }
}

// Thread Display
unsigned int __stdcall Display(void* pUser) {
    if (pUser) {
        CBasicDemoDlg* display_Cam = (CBasicDemoDlg*)pUser;

        display_Cam->DisplayThread();

        return 0;
    }
    return -1;
}
void CBasicDemoDlg::DisplayThread() {

    MV_DISPLAY_FRAME_INFO stDisplayInfo = { 0 };
    Mat Mat_display;

    while (m_bThreadState) {
      
        // wait until Mat_src not NULL
        if (Mat_src.empty()) {
            Sleep(5);
            //AfxMessageBox(L" Mat Display Error!");
            continue;
        }

        // copy Mat_src and draw
        //Mat_display = dst.clone();
        Mat_display = Mat_src.clone(); // Access violation 2 thead-> fixed by waiting the first thread started
        
        putText(Mat_display, "FPS: "+ to_string(real_fps), Point(5, 25), FONT_HERSHEY_COMPLEX, 1, 128, 1, 8);
        line(Mat_display, Point(0, line_position), Point(640, line_position), 0, 2, 8, 0);
        // display frame
        stDisplayInfo.hWnd = m_hwndDisplay;
        stDisplayInfo.pData = Mat_display.data;
        stDisplayInfo.nDataLen = m_stImageInfo.nFrameLen;
        stDisplayInfo.nWidth = m_stImageInfo.nWidth;
        stDisplayInfo.nHeight = m_stImageInfo.nHeight;
        stDisplayInfo.enPixelType = m_stImageInfo.enPixelType;
        m_pcMyCamera->DisplayOneFrame(&stDisplayInfo);

        UpdateData(FALSE); // update shrimp number and frame count to window

        // sleep 15ms/60fps
        waitKey(15);
    }
    Mat_display.release();
    return;
}

// en:Click Start button
void CBasicDemoDlg::OnBnClickedStartGrabbingButton() {
    if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing || NULL == m_pcMyCamera) {
        return;
    }

    memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    m_bThreadState = TRUE;

    // start get image and processing thread
    unsigned int nThreadID_0 = 0;
    m_hGrabThread = (void*)_beginthreadex( NULL , 0 , GrabThread , this, 0 , &nThreadID_0 );
    if (NULL == m_hGrabThread) {
        m_bThreadState = FALSE;
        ShowErrorMsg(TEXT("Create Image Processing Thread Fail!"), 0);
        return;
    }
    int nRet = m_pcMyCamera->StartGrabbing();
    if (MV_OK != nRet) {
        m_bThreadState = FALSE;
        ShowErrorMsg(TEXT("Start Grabbing Fail!"), nRet);
        return;
    }

    // start display thread must be after camera start grabbing
    Sleep(200); // 200ms wait for image proceesing thread started, avoid conflict
    unsigned int dThreadID_1 = 1;
    d_hGrabThread = (void*)_beginthreadex(NULL, 0, Display, this, 0, &dThreadID_1);
    if (NULL == d_hGrabThread) {
        m_bThreadState = FALSE;
        ShowErrorMsg(TEXT("Create Display Thread Fail!"), 0);
        return;
    }

    m_bStartGrabbing = TRUE;
    EnableControls(TRUE);
}

// en:Click Stop button
void CBasicDemoDlg::OnBnClickedStopGrabbingButton() {
    if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing || NULL == m_pcMyCamera) {
        return;
    }

    m_bThreadState = FALSE;

    if (m_hGrabThread) {
        //WaitForSingleObject(m_hGrabThread, INFINITE); // bug infinite waiting
        CloseHandle(m_hGrabThread);
        m_hGrabThread = NULL;
    }
    if (d_hGrabThread) {
        //WaitForSingleObject(d_hGrabThread, INFINITE); // bug infinite time waiting
        CloseHandle(d_hGrabThread);
        d_hGrabThread = NULL;
    }

    int nRet = m_pcMyCamera->StopGrabbing();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Stop grabbing fail"), nRet);
        return;
    }
    m_bStartGrabbing = FALSE;
    EnableControls(TRUE);

    // Stop grabbing and stop count but not write to file
    b_start_count = false;
    GetDlgItem(IDC_START_COUNT_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_STOP_COUNT_BUTTON)->EnableWindow(FALSE);
    trackers.clear();
    frameTrackingResult.clear();
    previous_frameTrackingResult.clear();
    current_centers.clear();
    previous_centers.clear();
}

//en:Click Get Parameter button / SDK function
void CBasicDemoDlg::OnBnClickedGetParameterButton() {
    int nRet = GetTriggerMode();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Trigger Mode Fail"), nRet);
    }

    nRet = GetExposureTime();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Exposure Time Fail"), nRet);
    }

    nRet = GetGain();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Gain Fail"), nRet);
    }

    nRet = GetFrameRate();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Frame Rate Fail"), nRet);
    }

    nRet = GetTriggerSource();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Trigger Source Fail"), nRet);
    }

    UpdateData(FALSE);
}

//en:Click Set Parameter button / SDK function
void CBasicDemoDlg::OnBnClickedSetParameterButton() {
    UpdateData(TRUE);

    bool bIsSetSucceed = true;
    int nRet = SetExposureTime();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Exposure Time Fail"), nRet);
    }
    nRet = SetGain();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Gain Fail"), nRet);
    }
    nRet = SetFrameRate();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Frame Rate Fail"), nRet);
    }
    
    if (true == bIsSetSucceed)
    {
        ShowErrorMsg(TEXT("Set Parameter Succeed"), nRet);
    }
}

//en:Click Software button / SDK function
void CBasicDemoDlg::OnBnClickedSoftwareTriggerCheck() {
    UpdateData(TRUE);

    int nRet = SetTriggerSource();
    if (nRet != MV_OK)
    {
        return;
    }
}

// en:Click Execute button / SDK function
void CBasicDemoDlg::OnBnClickedSoftwareOnceButton() {
    if (TRUE != m_bStartGrabbing)
    {
        return;
    }

    m_pcMyCamera->CommandExecute("TriggerSoftware");
}

//en:Click Save BMP button / SDK function
void CBasicDemoDlg::OnBnClickedSaveBmpButton() {
    int nRet = SaveImage(MV_Image_Bmp);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Save bmp fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save bmp succeed"), nRet);
}

//en:Click Save JPG button / SDK function
void CBasicDemoDlg::OnBnClickedSaveJpgButton() {
    int nRet = SaveImage(MV_Image_Jpeg);
    if (MV_OK != nRet) {
        ShowErrorMsg(TEXT("Save jpg fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save jpg succeed"), nRet);
}

void CBasicDemoDlg::OnBnClickedSaveTiffButton() {
    int nRet = SaveImage(MV_Image_Tif);
    if (MV_OK != nRet) {
        ShowErrorMsg(TEXT("Save tiff fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save tiff succeed"), nRet);
}

void CBasicDemoDlg::OnBnClickedSavePngButton() {
    int nRet = SaveImage(MV_Image_Png);
    if (MV_OK != nRet) {
        ShowErrorMsg(TEXT("Save png fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save png succeed"), nRet);
}

// en:Exit from upper right corner / SDK function
void CBasicDemoDlg::OnClose() {
    PostQuitMessage(0);
    CloseDevice();

    DeleteCriticalSection(&m_hSaveImageMux);
    CDialog::OnClose();
}

BOOL CBasicDemoDlg::PreTranslateMessage(MSG* pMsg) {
    if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE) {
        return TRUE;
    }

    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

bool CBasicDemoDlg::RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat) {
     int nResult = enPixelFormat & MV_GVSP_PIX_CUSTOM;
    if(MV_GVSP_PIX_CUSTOM == nResult) {
        return true;
    }
    else {
        return false;
    }
}

// Apply settings from setting window to main window
void CBasicDemoDlg::OnBnClickedSettingButton() {

    Setting_Window* open_setting_windown = new Setting_Window();
    open_setting_windown->DoModal();

    // click ok -> update = true
    if (open_setting_windown->update_setting == true) { // clicked OK
        // get image processing paramerters from setting window
        // segment to binary method
        //int adaptiveThreshold_check = abs(open_setting_windown->setting_adaptiveThreshold_Checked);
        //int bsg_check = abs(open_setting_windown->setting_bsg_Checked);
        // use in grabThreadProcess function
        segment_binary_method = open_setting_windown->setting_segment_binary_method;
        if (segment_binary_method == L"Adaptive Threshold") {
                // adaptive threshold
                adaptiveThreshold_method = open_setting_windown->setting_adaptiveThreshold_method;
                adaptiveThreshold_KSize = open_setting_windown->setting_adaptiveThreshold_KSize;
                adaptiveThreshold_C = open_setting_windown->setting_adaptiveThreshold_C;
                // change adaptive threshod point filter
                Size Adap_size = Size(adaptiveThreshold_KSize, adaptiveThreshold_KSize);
                if (adaptiveThreshold_method == L"MEAN_C") {
                    gpu_adaptiveThreshold_filter = cuda::createBoxFilter(CV_8U, CV_8U, Adap_size);
                }
                else if (adaptiveThreshold_method == L"GAUSSIAN_C") {
                    int sigma = 5;
                    gpu_adaptiveThreshold_filter = cuda::createGaussianFilter(CV_8U, CV_8U, Adap_size, sigma);
                }
                else {
                    AfxMessageBox(L"Adaptive Threshold Setting Error!");
                    delete open_setting_windown;
                    return;
                }
                // for test
                //AfxMessageBox(L"Adaptive Threshold Success!");
        }
        else if (segment_binary_method == L"Background Subtraction") {
                // back ground subtraction
                bgs_method = open_setting_windown->setting_bsg_method;
                bgs_threshold = open_setting_windown->setting_bsg_threshold;
                bgs_shadows = open_setting_windown->setting_bsg_shadow;
                bgs_history = open_setting_windown->setting_bsg_history;
                // change back ground subtraction method
                if (bgs_method == L"MOG2") {
                    if (bgs_shadows == L"True") {
                        pBackSub = cuda::createBackgroundSubtractorMOG2(bgs_history, bgs_threshold, true);
                    }
                    else {
                        pBackSub = cuda::createBackgroundSubtractorMOG2(bgs_history, bgs_threshold, false);
                    }
                }
                else if (bgs_method == L"MOG") {
                    pBackSub = cuda::createBackgroundSubtractorMOG(bgs_history, 5, 0.7);
                }
                else {
                    // for debug
                    AfxMessageBox(L"Backgruond Subtraction Setting Error!");
                    delete open_setting_windown;
                    return;
                }
                // for test
                //AfxMessageBox(L"BSG Success!");
        }
        else {
            // for debug
            AfxMessageBox(L"Segment to binary Setting Error!");
            delete open_setting_windown;
            return;
        }

        // counting method
        //int MyTracking_check = abs(open_setting_windown->setting_MyTracking_Checked); // use abs because -1 after dont check
        //int SORTTracking_check = abs(open_setting_windown->setting_SORTTracking_Checked); // checked = 0
        couting_method = open_setting_windown->setting_counting_method;
        if (couting_method == L"My Simple Tracking") {
            // my simple couting
            tolerance_x = open_setting_windown->setting_tolerance_x;

            // for test
            //AfxMessageBox(L"Simple Success!");
        }
        else if (couting_method == L"SORT") {
            // SORT Tracking + counting
            distance_threshold = open_setting_windown->setting_distance_threshold;
            min_hits = open_setting_windown->setting_min_hits;
            max_age = open_setting_windown->setting_max_age;

            // for test
            //AfxMessageBox(L"SORT Success!");
        }
        else {
            AfxMessageBox(L"Counting Method Setting Error!");
            delete open_setting_windown;
            return;
        }

        // counting parameters
        line_position = open_setting_windown->setting_line_position;
        max_distance = open_setting_windown->setting_max_distance;

        // Detection parameters
        avg_area = open_setting_windown->setting_avg_area;
        min_area = open_setting_windown->setting_min_area;
        max_area = open_setting_windown->setting_max_area;
        min_width = open_setting_windown->setting_min_width;
        max_width = open_setting_windown->setting_max_width;
        min_height = open_setting_windown->setting_min_height;
        max_height = open_setting_windown->setting_max_height;
        
        // cuda blur image
        blur_method = open_setting_windown->setting_blur_method;
        blur_kernel = open_setting_windown->setting_blur_kernel;
        if (blur_kernel != 1) { // blur kernel = 1 dont use blur
            if (blur_method == L"GAUSS") {
                cuda_filter = createGaussianFilter(CV_8U, CV_8U, Size(blur_kernel, blur_kernel), 1);
            }
            else if (blur_method == L"AVG") {
                cuda_filter = cuda::createBoxFilter(CV_8U, CV_8U, Size(blur_kernel, blur_kernel));
            }
            else if (blur_method == L"MEDIAN"){
                cuda_filter = cuda::createMedianFilter(CV_8U, blur_kernel);
            }
            else {
                AfxMessageBox(L"Blur Method Setting Error!");
                delete open_setting_windown;
                return;
            }
        }
        // Morphological filter
        morphological_method = open_setting_windown->setting_morpho_type;
        morphological_kernel = open_setting_windown->setting_morpho_kernel;
        morphological_iterations = open_setting_windown->setting_morpho_iterations;
        if (morphological_kernel != 1) { // = 1 dont use
            mo_kernel = getStructuringElement(MORPH_RECT, Size(morphological_kernel, morphological_kernel));
            if (morphological_method == L"Dilate") {
                mo_filter = cuda::createMorphologyFilter(MORPH_DILATE, CV_8U, mo_kernel, Point(-1, -1), morphological_iterations);
            }
            else if (morphological_method == L"Erode") {
                mo_filter = cuda::createMorphologyFilter(MORPH_ERODE, CV_8U, mo_kernel, Point(-1, -1), morphological_iterations);
            }
            else if (morphological_method == L"Open") {
                mo_filter = cuda::createMorphologyFilter(MORPH_OPEN, CV_8U, mo_kernel, Point(-1, -1), morphological_iterations);
            }
            else if (morphological_method == L"Close") {
                mo_filter = cuda::createMorphologyFilter(MORPH_CLOSE, CV_8U, mo_kernel, Point(-1, -1), morphological_iterations);
            }
            else {
                AfxMessageBox(L"Morphological Method Setting Error!");
                delete open_setting_windown;
                return;
            }
        }
        // All successed!
        AfxMessageBox(L"Updated All Parameters Success!");
        delete open_setting_windown;
        return;
    }
    else {
        //AfxMessageBox(L"Parameters have not been Updated!");
        delete open_setting_windown;
        return;
    }
    //open_setting_windown->DestroyWindow();
}

//Opencv convert data to Mat
bool CBasicDemoDlg::Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData, Mat *srcImage) {
    if (pstImageInfo->enPixelType == PixelType_Gvsp_Mono8) {
        *srcImage = Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1, pData);
    }
    else {
        printf("unsupported pixel format\n");
        return false;
    }

    if (NULL == srcImage) {
        printf("Convert to Mat failed.\n");
        return false;
    }
    return true;
}

//input: Mat_src in gray scale
//output1: vector<Trckingbox> detections
//output2: vector<humoment[7]> HuMoments  // error
//output3: vector<Point> current_centers
//This function run in cuda gpu and contours cpu
void CBasicDemoDlg::ImageProcessing_GPU() {

    frame_count++;
    // for test
    //counter++;

    // check Mat_src input
    if (Mat_src.empty()) return;
    gpu_Mat_src.upload(Mat_src);
    // check Mat input in gray scale, CV_8U?
    // implement in gpu cuda
    if (gpu_Mat_src.channels() == 3) {
        // color image, convert to gray scale image
        cuda::cvtColor(gpu_Mat_src, gpu_Mat_src, COLOR_BGR2GRAY);
    }
    else if (gpu_Mat_src.channels() != 1) {
        AfxMessageBox(L"Source image error in number of channel!");
        return;
    }
    if (gpu_Mat_src.depth() != CV_8U) {
        gpu_Mat_src.convertTo(gpu_Mat_src, CV_8U);
    }

    //cuda blur image
    if (blur_kernel != 1) // kernel == 1 no convolution
        cuda_filter->apply(gpu_Mat_src, gpu_Mat_src);

    // Segmentation to binary image
    // Threshold
    //cuda::threshold(gpu_Mat_src, gpu_Mat_dst, 127, 255, THRESH_BINARY_INV);

    if (segment_binary_method == L"Adaptive Threshold") {
        AdaptiveThreshold_GPU(gpu_Mat_src, gpu_Mat_dst);
    }
    else if (segment_binary_method == L"Background Subtraction") {
        pBackSub->apply(gpu_Mat_src, gpu_Mat_dst);
    }
    else{
        AfxMessageBox(L"Image Processing Error Binary Segment Method!");
        return;
    }
   
    // morphological filter
    if (morphological_kernel != 1) // kernel == 1 no convolution
        mo_filter->apply(gpu_Mat_dst, gpu_Mat_dst);

    gpu_Mat_dst.download(dst);

    // Distance transform
    //distanceTransform(dst, dst, DIST_L2, 3);
    //cv::threshold(dst, dst, 100, 255, THRESH_BINARY);

    // contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    detections.clear();
    current_centers.clear();
    cv::findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area < min_area || area > max_area) continue;

        Rect_<int> boundRect = boundingRect(contours[i]);
        // if width > height (vertical) height <- width, width <-height
        // height is a larger number
        int height_Rect, width_Rect;
        if (boundRect.width > boundRect.height) {
            height_Rect = boundRect.width;
            width_Rect = boundRect.height;
        }
        else {
            height_Rect = boundRect.height;
            width_Rect = boundRect.width;
        }
        if ((width_Rect < min_width) || (width_Rect > max_width) || 
            (height_Rect < min_height) || (height_Rect > max_height)) 
            continue;
       
        // get center point
        Moments M = moments(contours[i]);
        Point2f center_point((M.m10 / M.m00), (M.m01 / M.m00));
        current_centers.push_back(center_point);

        // get tracking center
        TrackingCenter detect_center;
        detect_center.id = -1;
        detect_center.frame = frame_count;
        detect_center.center = center_point;
        detections.push_back(detect_center); // push rectangle box to detection
    }
}

//input: gpu_mat_src in gray scale CV_8U after blur
//output: gpu_mat_dst in binary
void CBasicDemoDlg::AdaptiveThreshold_GPU(GpuMat gsrc, GpuMat &gdst) {

    gpu_adaptiveThreshold_filter->apply(gsrc, gdst);

    cuda::subtract(gdst, adaptiveThreshold_C, gdst);
    cuda::compare(gsrc, gdst, gdst, CMP_LE);
}
// intput: current centers and previous centers
// this function find the point
// below the line in current centers matched with the point
// above the line in previous centers to count
// output: counter
void CBasicDemoDlg::My_Simple_Counting(int tolerance_x, float max_distance) {
    if (current_centers.size() == 0)
        return; // the first frame
    
    // find the same x point
    for (auto current_point = begin(current_centers); current_point != end(current_centers); ++current_point) {
        if (current_point->y >= line_position) { //current point below the line
            vector<Point> matched_points;
            matched_points.clear();
            for (auto pervious_point = begin(previous_centers); pervious_point != end(previous_centers); ++pervious_point) {
                if (pervious_point->y < line_position) { // previous point above the line
                    if ((current_point->x > pervious_point->x - tolerance_x) && (current_point->x < pervious_point->x + tolerance_x)) {
                        float distance_ = GetDistance(*current_point, *pervious_point);
                        if (distance_ < max_distance) { // Check distance
                            Point matched_point = Point(pervious_point->x, pervious_point->y);
                            matched_points.push_back(matched_point);
                        }
                    }
                }
            }
            if (matched_points.size() > 0) { // least one point matched
                counter++;
                // find matched point by sort y
                // sort decrease through y value
                // tested -> ok
                sort(matched_points.begin(), matched_points.end(), sort_point_y); 
                // remove nearest matched point in previous centers
                auto position_ = find(previous_centers.begin(), previous_centers.end(), matched_points[0]);
                //if (position_ != previous_centers.end())
                previous_centers.erase(position_);
                matched_points.clear();
            }
        }
    }
    // copy current centers to previous centers
    previous_centers.clear();
    std::copy(current_centers.begin(), current_centers.end(), back_inserter(previous_centers));
    //clear current centers
    current_centers.clear();
    return;
}

double CBasicDemoDlg::GetDistance(Point2f center_test, Point2f center_gt) {
    float delta_xx = center_test.x - center_gt.x; // center x
    float delta_yy = center_test.y - center_gt.y; // center y
    float un = delta_xx * delta_xx + delta_yy * delta_yy;
    if (un < DBL_EPSILON)
        return 0;
    return sqrt(un);
}
//input: trackers, detection
//output: vector<Trakingbox> frameTrackingResult
void CBasicDemoDlg::SORT(int max_age, int min_hits, double distanceThreshold) {
     
    //KalmanTracker::kf_count = 0; // tracking id relies on this, so we have to reset it in each seq.
    
    if (trackers.size() == 0) { // the first frame
        // initialize kalman trackers using first detections.
        for (unsigned int i = 0; i < detections.size(); i++) {
            KalmanTracker trk = KalmanTracker(detections[i].center); //kf_count++ -> id++
            trackers.push_back(trk);
        }
        frameTrackingResult.clear();
        return;
    }

    // 3.1. get predicted locations from existing trackers.
    vector<Point2f> predictedCenters; //contain predict Centers of Dectections
    for (auto it = trackers.begin(); it != trackers.end();) {
        Point2f pCenter = (*it).predict();
        if (pCenter.y < Image_Height || pCenter.y > 0) {
            if (pCenter.x < 0) pCenter.x = 0;
            if (pCenter.x > Image_Width) pCenter.x = Image_Width;
            predictedCenters.push_back(pCenter);
            it++;

            //save predict center with id:"" frame:"" center:""
            CStdioFile SavePredictFile;
            CFileException exPredict;
            if (SavePredictFile.Open(L"SavePredictFile.txt", CFile::modeNoTruncate | CFile::modeWrite, &exPredict)){
                CString daataPredict;
                daataPredict.Format(_T("id:%lld frame:%lld center:(%f, %f) \n"), (*it).m_id, frame_count, pCenter.x, pCenter.y);
                SavePredictFile.SeekToEnd();
                SavePredictFile.WriteString(daataPredict);
                SavePredictFile.Close();
            }
        }
        else {
            it = trackers.erase(it);
        }
    }

    // 3.2. associate detections to tracked object (both represented as bounding boxes)
    // dets : detFrameData
    unsigned int trkNum = predictedCenters.size();
    unsigned int detNum = detections.size();
    vector<vector<double>> distanceMatrix;
    distanceMatrix.resize(trkNum, vector<double>(detNum, 0));
    for (unsigned int i = 0; i < trkNum; i++) {    // compute distance matrix
        for (unsigned int j = 0; j < detNum; j++) {
            // assign all elements of distance maxtrix
            distanceMatrix[i][j] = GetDistance(predictedCenters[i], detections[j].center);
        }
    }
    // fix bug//note this
    if (distanceMatrix.size() == 0) return;


    HungarianAlgorithm HungAlgo; 
    vector<int> assignment; // the resulting assignment is [track(prediction) : detection], with len=predict Number
    HungAlgo.Solve(distanceMatrix, assignment); // solve the assignment problem using hungarian algorithm.
    

    // find matches, unmatched_detections and unmatched_predictions
    set<int> unmatchedTrajectories;
    set<int> unmatchedDetections;
    set<int> allItems;
    set<int> matchedItems;
    if (detNum > trkNum) { //	there are unmatched detections
        for (unsigned int n = 0; n < detNum; n++)
            allItems.insert(n);
        for (unsigned int i = 0; i < trkNum; ++i)
            matchedItems.insert(assignment[i]);
        set_difference(allItems.begin(), allItems.end(),
            matchedItems.begin(), matchedItems.end(),
            insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin()));
    }
    else if (detNum < trkNum) {  // there are unmatched trajectory/predictions
        for (unsigned int i = 0; i < trkNum; ++i) {
            if (assignment[i] == -1) // unassigned label will be set as -1 in the assignment algorithm
                unmatchedTrajectories.insert(i);
            else
                matchedItems.insert(i);
        }
    }
    else { // detNum = trkNum
        for (unsigned int i = 0; i < trkNum; ++i)
            matchedItems.insert(assignment[i]);
    }
        
    // filter out matched with large distance
    vector<Point> matchedPairs;
    for (unsigned int i = 0; i < trkNum; ++i) {
        if (assignment[i] == -1) // pass over invalid values
            continue;

        // Save matched distance
        CStdioFile SaveDistanceMatched;
        CFileException exDistanceMatched;
        if (SaveDistanceMatched.Open(L"SaveDistanceMatched.txt", CFile::modeNoTruncate | CFile::modeWrite, &exDistanceMatched)) {
            CString daata_;
            daata_.Format(_T("distance:%f \n"), distanceMatrix[i][assignment[i]]);
            SaveDistanceMatched.SeekToEnd();
            SaveDistanceMatched.WriteString(daata_);
            SaveDistanceMatched.Close();
        }

        if (distanceMatrix[i][assignment[i]] > distanceThreshold) {
            unmatchedTrajectories.insert(i);
            unmatchedDetections.insert(assignment[i]);
        }
        else
            matchedPairs.push_back(Point(i, assignment[i]));
    }
    // 3.3. updating trackers
    // update matched trackers with assigned detections.
    // each prediction is corresponding to a tracker
    for (unsigned int i = 0; i < matchedPairs.size(); i++) {
        int trkIdx = matchedPairs[i].x;
        int detIdx = matchedPairs[i].y;
        trackers[trkIdx].update(detections[detIdx].center);
    }

    // create and initialise new trackers for unmatched detections
    for (auto umd : unmatchedDetections) {
        KalmanTracker tracker = KalmanTracker(detections[umd].center);
        trackers.push_back(tracker);
    }

    // get trackers' output
    frameTrackingResult.clear();
    for (auto it = trackers.begin(); it != trackers.end();) {
        if (((*it).m_time_since_update < 1) && // < 1 -> updated
            ((*it).m_hit_streak >= min_hits || frame_count <= min_hits)) { // m_hit_streak time from tracker begin
            TrackingCenter res;
            res.center = (*it).get_state();
            res.id = (*it).m_id + 1; //??? m_id = kf_count
            res.frame = frame_count;
            frameTrackingResult.push_back(res);
            it++;
        }
        else
            it++;
        // remove dead tracklet
        if (it != trackers.end() && (*it).m_time_since_update > max_age)
            it = trackers.erase(it);
    }
}
// counting function
// input: previous_tracking and current_tracking
// ouput: counter
void CBasicDemoDlg::SORT_Counting() {

    if (frameTrackingResult.size() == 0)
        return; // the first frame

    for (int i = 0; i < frameTrackingResult.size(); i++){

        if (frameTrackingResult[i].center.y <= line_position) { // current point above line ->> pass
            continue;
        } // current point bellow line this time
        for (int k = 0; k < previous_frameTrackingResult.size(); k++) {
            if (frameTrackingResult[i].id == previous_frameTrackingResult[k].id) {  //same id
                if (previous_frameTrackingResult[k].center.y > line_position) { // previous point bellow line ->> pass
                    continue;
                } // previous point above the line this time with the same id
                else {
                    float distance_ = GetDistance(frameTrackingResult[i].center, previous_frameTrackingResult[k].center);
                    if (distance_ > max_distance) { // check distance
                        continue;
                    }
                    else {
                        counter++;
                        
                    }
                }
            }
        } 
    }
    // copy frame tracking results to previous tracking result
    previous_frameTrackingResult.clear();
    copy(frameTrackingResult.begin(), frameTrackingResult.end(), back_inserter(previous_frameTrackingResult));
    // clear frame tracking result
    frameTrackingResult.clear();
    return;
}

void CBasicDemoDlg::OnBnClickedStartCountButton() {
    
    b_start_count = true;
    //EnableControls(TRUE);
    // Disable start count button
    GetDlgItem(IDC_START_COUNT_BUTTON)->EnableWindow(FALSE);
    // Enable stop count button
    GetDlgItem(IDC_STOP_COUNT_BUTTON)->EnableWindow(TRUE);
    
    return;
}
void CBasicDemoDlg::OnBnClickedStopCountButton() {
    
    b_start_count = false;
    //EnableControls(TRUE);
    // Enable start count button
    GetDlgItem(IDC_START_COUNT_BUTTON)->EnableWindow(TRUE);
    // Disable stop count button
    GetDlgItem(IDC_STOP_COUNT_BUTTON)->EnableWindow(FALSE);
    // Reset Kalman tracking
    trackers.clear();
    // Reset tracking box
    frameTrackingResult.clear();
    previous_frameTrackingResult.clear();
    // Reset centers
    current_centers.clear();
    previous_centers.clear();

    // write result to file
    // open save file mode write
    CStdioFile StdFile;
    CFileException ex;
    if (!StdFile.Open(global_filename, CFile::modeNoTruncate | CFile::modeWrite, &ex)) {
        CString error;
        error.Format(L"Error when open file to write result!\nCause = %d", ex.m_cause);
        AfxMessageBox(error);
        int message = AfxMessageBox(L"Create new file Result.result?", MB_YESNO);
        if (message == IDYES)
            StdFile.Open(global_filename, CFile::modeCreate | CFile::modeWrite, &ex);
        else 
            return;
    }
    // get date
    CTime time = CTime::GetCurrentTime();
    CString text_date = time.Format("%d/%m/%Y");
    // get time
    CString text_time = time.Format("%H:%M:%S");
    // get number
    CString text_counter;
    text_counter.Format(L"%lld", counter);
    CString text_result = text_date + L" " + text_time + L" " + text_counter + L"\n";
    // set pointer to the end file
    StdFile.SeekToEnd();
    // write result
    StdFile.WriteString(text_result);
    StdFile.Close();
    
    return;
}
void CBasicDemoDlg::OnBnClickedResetNumberButton() {
    
    counter = 0;
    frame_count = 0;

    UpdateData(FALSE);
}

void CBasicDemoDlg::OnBnClickedLogButton() {
    LogHistory* open_logHistory_windown = new LogHistory();
    open_logHistory_windown->DoModal();
    return;
}
