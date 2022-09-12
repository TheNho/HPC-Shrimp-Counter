// Shrimps Counting Project
// This code is implemented by The Nho 2022
// Interfacing with Camera HikRobot
// SORT Tracking
// Passing line counting
// BasicDemoDlg.cpp : Implementation file

#include "stdafx.h"
#include "BasicDemo.h"
#include "BasicDemoDlg.h"

// Global variables
// Parameters Setting 
// Using global to load current parameters to Setting window
extern CString flip_image;

extern CString blur_method;
extern int blur_kernel;

extern CString segment_binary_method;

extern CString bgs_method;
extern float bgs_threshold;
extern CString bgs_shadows;
extern int bgs_history;
extern float bsg_learning_rate;

extern CString adaptiveThreshold_method;
extern int adaptiveThreshold_KSize;
extern int adaptiveThreshold_C;

extern CString morphological_method;
extern int morphological_kernel;
extern int morphological_iterations;

extern int line_position;

extern float distance_threshold;
extern int min_hits;
extern int max_age;

extern double min_area;
extern double max_area;
extern int min_width;
extern int min_height;
extern int max_width;
extern int max_height;
extern double image_frame_rate;
extern double image_gain;
extern double image_exposure_time;
extern CString ROI_Point_Left_Above;
extern CString ROI_Point_Left_Below;
extern CString ROI_Point_Right_Above;
extern CString ROI_Point_Right_Below;

//Global filename, used in all windows
extern CString global_filename;

//CAboutDlg dialog used for App About
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

//CBasicDemoDlg dialog
CBasicDemoDlg::CBasicDemoDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CBasicDemoDlg::IDD, pParent)
    , m_pcMyCamera(NULL)
    , m_nDeviceCombo(0)
    , m_bOpenDevice(FALSE)
    , m_bStartGrabbing(FALSE)
    , m_hGrabThread(NULL)
    , m_bThreadState(FALSE)
    , m_nTriggerMode(MV_TRIGGER_MODE_OFF)
    //, m_dExposureEdit(0)
    //, m_dGainEdit(0)
    //, m_dFrameRateEdit(0)
    , m_bSoftWareTriggerCheck(FALSE)
    , m_nTriggerSource(MV_TRIGGER_SOURCE_SOFTWARE)
    , m_pSaveImageBuf(NULL)
    , m_nSaveImageBufSize(0)
    , counter(0)
    , frame_count(0)
    , number_ROI_in_frame(0)
    , number_shrimp_in_frame(0)
    , F1_counter(0)
    , F2_counter(0)
    , F3_counter(0)
    , F4_counter(0)
    , avg_size(0)
{
    // Load icon
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hIcon = AfxGetApp()->LoadIcon(IDI_HPC_ICON);
    std::memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
}

void CBasicDemoDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DEVICE_COMBO, m_ctrlDeviceCombo);
    DDX_CBIndex(pDX, IDC_DEVICE_COMBO, m_nDeviceCombo);
    //DDX_Text(pDX, IDC_EXPOSURE_EDIT, m_dExposureEdit);
    //DDX_Text(pDX, IDC_GAIN_EDIT, m_dGainEdit);
    //DDX_Text(pDX, IDC_FRAME_RATE_EDIT, m_dFrameRateEdit);
    DDX_Check(pDX, IDC_SOFTWARE_TRIGGER_CHECK, m_bSoftWareTriggerCheck);
    DDX_Text(pDX, IDC_SHIRMP_NUMBER_STATIC, counter);
    DDX_Text(pDX, IDC_FRAME_COUNT_EDIT, frame_count);

    DDX_Text(pDX, IDC_ROI_IN_FRAME_EDIT, number_ROI_in_frame);
    DDX_Text(pDX, IDC_SHRIMP_IN_FRAME_EDIT, number_shrimp_in_frame);
    DDX_Text(pDX, IDC_F1_EDIT, F1_counter);
    DDX_Text(pDX, IDC_F2_EDIT, F2_counter);
    DDX_Text(pDX, IDC_F3_EDIT, F3_counter);
    DDX_Text(pDX, IDC_F4_EDIT, F4_counter);
    DDX_Text(pDX, IDC_AVG_SIZE_EDIT, avg_size);
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
   // ON_BN_CLICKED(IDC_GET_PARAMETER_BUTTON, &CBasicDemoDlg::OnBnClickedGetParameterButton)
    //ON_BN_CLICKED(IDC_SET_PARAMETER_BUTTON, &CBasicDemoDlg::OnBnClickedSetParameterButton)
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
        
        return 1;
    }

    return 0;
}

//CBasicDemoDlg message handlers
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
    // Set font for shrimps number in dialog
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
    // Load DefaultSettings file
    // Install the first run variables
    // ROI Mask
    ROI_Point_Left_Above = L"23,0";
    ROI_Point_Left_Below = L"32,480";
    ROI_Point_Right_Above = L"625,0";
    ROI_Point_Right_Below = L"619,480";
    Get_ROI_Mask();
    // Local variables
    // Load SVM
    SVM = ml::SVM::load(SVM_dir);
    // Image detail
    image_frame_rate = 200;
    image_gain = 10;
    image_exposure_time = 2000;
    // global variables
    flip_image = L"Y"; // None;X;Y
    blur_method = L"AVG"; //AVG;GAUSS
    blur_kernel = 1; //1;3;5;7;9
    segment_binary_method = L"Adaptive Threshold"; //Adaptive Threshold;Background Subtraction
    bgs_method = L"MOG2"; //MOG2
    bgs_threshold = 25;
    bgs_shadows = L"False"; //False;True
    bgs_history = 500;
    bsg_learning_rate = -1; // Negative for auto learning rate
    adaptiveThreshold_method = L"MEAN_C"; //MEAN_C;GAUSSIAN_C
    adaptiveThreshold_KSize = 29;
    adaptiveThreshold_C = 20;
    morphological_method = L"Open"; //Open;Close;Erode;Dilate
    morphological_kernel = 5; //1;3;5;7;9
    morphological_iterations = 2;
    line_position = 400;
    distance_threshold = 50; // distance threshold between predict center and current center
    min_hits = 3;
    max_age = 10;
    min_area = 10;
    max_area = 1000;
    min_width = 3;
    min_height = 3;
    max_width = 50;
    max_height = 50;

    // Initialize global directory to save result file
    global_filename = nFilename;

    // Segmentation
    if (segment_binary_method == L"Adaptive Threshold") {
        // do nothing
    }
    else if (segment_binary_method == L"Background Subtraction") {
        if (bgs_method == L"MOG2") {
            if (bgs_shadows == L"True") {
                pBackSub = createBackgroundSubtractorMOG2(bgs_history, bgs_threshold, true);
            }
            else if (bgs_shadows == L"False") {
                pBackSub = createBackgroundSubtractorMOG2(bgs_history, bgs_threshold, false);
            }
            else {
                AfxMessageBox(L"Shadow Error!");
                return;
            }
        }
        else {
            AfxMessageBox(L"Backgruond Subtraction Initail Error!");
            return;
        }
    }
    else {
        AfxMessageBox(L"Segmentation to Binary method Initail Error!");
        return;
    }
    //Blur image
    
    // Morphological filter
    mo_kernel = getStructuringElement(MORPH_RECT, Size(morphological_kernel, morphological_kernel));

    return;

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
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
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

//The system calls this function to obtain the cursor to display while the user drags
//the minimized window.
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
    //GetDlgItem(IDC_EXPOSURE_EDIT)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    //GetDlgItem(IDC_GAIN_EDIT)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    //GetDlgItem(IDC_FRAME_RATE_EDIT)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    //GetDlgItem(IDC_GET_PARAMETER_BUTTON)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
    //GetDlgItem(IDC_SET_PARAMETER_BUTTON)->EnableWindow(m_bOpenDevice && !m_bStartGrabbing ? TRUE : FALSE);
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
        errorMsg = csMessage + L"Error=" + nE + L"\n";
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
    case MV_E_INVALID_ADDRESS:  errorMsg += "Folder not found";                                                break;
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

//en:Set Trigger Mode / SDK function
int CBasicDemoDlg::SetTriggerMode()
{
    return m_pcMyCamera->SetEnumValue("TriggerMode", m_nTriggerMode);
}

//en:Get Exposure Time
double CBasicDemoDlg::GetExposureTime() {
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &stFloatValue);
    if (MV_OK != nRet) {
        return nRet;
    }

    return stFloatValue.fCurValue;;
}

//en:Set Exposure Time 
int CBasicDemoDlg::SetExposureTime(float ExposureTime) {
    // en:Adjust these two exposure mode to allow exposure time effective
    int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
    if (MV_OK != nRet) {
        return nRet;
    }

    m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

    return m_pcMyCamera->SetFloatValue("ExposureTime", ExposureTime);
}

//en:Get Gain 
double CBasicDemoDlg::GetGain() {
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("Gain", &stFloatValue);
    if (MV_OK != nRet) {
        return nRet;
    }

    return stFloatValue.fCurValue;;
}

//en:Set Gain 
int CBasicDemoDlg::SetGain(float Gian) {
    //en:Set Gain after Auto Gain is turned off, this failure does not need to return
    m_pcMyCamera->SetEnumValue("GainAuto", 0);

    return m_pcMyCamera->SetFloatValue("Gain", Gian);
}

//en:Get Frame Rate 
double CBasicDemoDlg::GetFrameRate() {
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("ResultingFrameRate", &stFloatValue);
    if (MV_OK != nRet) {
        return nRet;
    }

    return stFloatValue.fCurValue;;
}

//en:Set Frame Rate
int CBasicDemoDlg::SetFrameRate( float FrameRate) {
    int nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet) {
        return nRet;
    }

    return m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", FrameRate);
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

//en:Save Image
int CBasicDemoDlg::SaveImage(enum MV_SAVE_IAMGE_TYPE enSaveImageType) {
    // check folder Saved_Image existing, if not create folder
    DWORD fileAttr = GetFileAttributes(L"Saved_Image");
    if (fileAttr == INVALID_FILE_ATTRIBUTES && (fileAttr & FILE_ATTRIBUTE_DIRECTORY)) {
        //create folder
         int code = _wmkdir(L"Saved_Image");
         return MV_E_INVALID_ADDRESS;
    }
    MV_SAVE_IMG_TO_FILE_PARAM stSaveFileParam;
    std::memset(&stSaveFileParam, 0, sizeof(MV_SAVE_IMG_TO_FILE_PARAM));

    EnterCriticalSection(&m_hSaveImageMux);
    if (m_pSaveImageBuf == NULL || m_stImageInfo.enPixelType == 0) {
        LeaveCriticalSection(&m_hSaveImageMux);
        return MV_E_NODATA;
    }

    if(RemoveCustomPixelFormats(m_stImageInfo.enPixelType)) {
        LeaveCriticalSection(&m_hSaveImageMux);
        return MV_E_SUPPORT;
    }
    Mat mSave = Mat(m_stImageInfo.nHeight, m_stImageInfo.nWidth, CV_8UC1, m_pSaveImageBuf);
    if (flip_image == L"None") {
        // do nothing
    }
    else if (flip_image == L"X") {
        cv::flip(mSave, mSave, 1);
    }
    else if (flip_image == L"Y") {
        cv::flip(mSave, mSave, 0);
    }
    stSaveFileParam.enImageType = enSaveImageType; // en:Image format to save
    stSaveFileParam.enPixelType = m_stImageInfo.enPixelType;  // en:Camera pixel type
    stSaveFileParam.nWidth      = m_stImageInfo.nWidth;         // en:Width
    stSaveFileParam.nHeight     = m_stImageInfo.nHeight;          //en:Height
    stSaveFileParam.nDataLen    = m_stImageInfo.nFrameLen;
    stSaveFileParam.pData       = mSave.data;
    stSaveFileParam.iMethodValue = 0;

    //en:jpg image nQuality range is (50-99], png image nQuality range is [0-9]
    if (MV_Image_Bmp == stSaveFileParam.enImageType) {
        sprintf_s(stSaveFileParam.pImagePath, 256, "Saved_Image/Image_w%d_h%d_fn%03d.bmp", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Jpeg == stSaveFileParam.enImageType) {
        stSaveFileParam.nQuality = 80;
        sprintf_s(stSaveFileParam.pImagePath, 256, "Saved_Image/Image_w%d_h%d_fn%03d.jpg", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Tif == stSaveFileParam.enImageType) {
        sprintf_s(stSaveFileParam.pImagePath, 256, "Saved_Image/Image_w%d_h%d_fn%03d.tif", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Png == stSaveFileParam.enImageType) {
        stSaveFileParam.nQuality = 8;
        sprintf_s(stSaveFileParam.pImagePath, 256, "Saved_Image/Image_w%d_h%d_fn%03d.png", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }

    int nRet = m_pcMyCamera->SaveImageToFile(&stSaveFileParam);
    LeaveCriticalSection(&m_hSaveImageMux);

    return nRet;
}

// Importance function!
// Grap image from buffer, copy to cv Mat, processing and counting
int CBasicDemoDlg::GrabThreadProcess() {

    //For debug
    //String dir_file = "Data/Data10/";
    //uint idx_img = 0;
    
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
                    return -1;
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
            // Convert data to cvMat, drop roi and flip image
            bool retm = Convert2Mat(&stImageInfo.stFrameInfo, stImageInfo.pBufAddr);
            if (retm == false || Mat_src.empty()) {
                AfxMessageBox(L"Error while convert data to cv Mat!");
                return -1;
            }
            // Image processing
            ImageProcessing();
            /// Press start count?
            if (b_start_count==true) {
                // SORT tracking and counting
                SORT(max_age, min_hits, distance_threshold);
                SORT_Counting();
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
        /*////////////////////////////////////////////////////////////////////////////////////////
        String img_file = dir_file + to_string(idx_img) + ".bmp";
        Mat src_img1 = imread(img_file, 0);
        if (src_img1.empty()) break;
        Rect ROI(35, 0, 570, 480);
        Mat_src = src_img1(ROI);
        idx_img++;
        // image processing
        ImageProcessing();
        /// Press start count?
        if (b_start_count == true) {
            SORT(max_age, min_hits, distance_threshold);
            SORT_Counting();
        }
        // for debug
        imshow("Src Image Before", Mat_src);
        // press space to pause and continue
        if (waitKey(15) == 32)
            waitKey();
        int64 end_time_fps = getTickCount();
        real_fps = int(getTickFrequency() / (end_time_fps - start_time_fps));
        start_time_fps = end_time_fps;
        ////////////////////////////////////////////////////////////////////////////////////*/
    }
    // end thread, destroy all windows
    //cv::destroyAllWindows();
    return MV_OK;
}

//en:Click Find Device button:Enumeration
void CBasicDemoDlg::OnBnClickedEnumButton() {
    // Check if camera driver installed
    bool driver_installed = false;
    LPVOID drivers[ARRAY_SIZE];
    DWORD cbNeeded;
    int cDrivers, i;
    if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers)){
        TCHAR szDriver[ARRAY_SIZE];
        cDrivers = cbNeeded / sizeof(drivers[0]);
        for (i = 0; i < cDrivers; i++) {
            if (GetDeviceDriverBaseName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0]))) {
                if (_tcscmp(szDriver, driver_name) == 0) {
                    driver_installed = true;
                    break;
                }
            }
        }
    }
    if (driver_installed == false) {
        int message = AfxMessageBox(L"Install Camera's Driver???", MB_YESNO);
        if (message == IDYES) {
            system(dir_driver_install); // install driver
            return;
        }
        else
            return;
    }
    
    CString strMsg;

    m_ctrlDeviceCombo.ResetContent();
    std::memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    //en:Enumerate all devices within subnet
    int nRet = CMvCamera::EnumDevices(MV_USB_DEVICE, &m_stDevList);
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
        if (pDeviceInfo->nTLayerType == MV_USB_DEVICE) {
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

//en:Click Open button: Open Device / SDK function
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
    // Set default camera parameters
    // Frame rate
    nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet) {
        AfxMessageBox(L"Cannot set Initail Frame Rate!");
        return;
    }
    m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", image_frame_rate);
    // Gain
    m_pcMyCamera->SetEnumValue("GainAuto", 0);
    m_pcMyCamera->SetFloatValue("Gain", image_gain);
    // Expose time
    nRet = m_pcMyCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
    if (MV_OK != nRet) {
        AfxMessageBox(L"Cannot set Initail Expose Time!");
        return;
    }
    m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    m_pcMyCamera->SetFloatValue("ExposureTime", image_exposure_time);

    m_bOpenDevice = TRUE;
    EnableControls(TRUE);
    //GetImageParameters(); //en:Get Parameter
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

//Thread Display
unsigned int __stdcall Display(void* pUser) {
    if (pUser) {
        CBasicDemoDlg* display_Cam = (CBasicDemoDlg*)pUser;

        display_Cam->DisplayThread();

        return 1;
    }
    return 0;
}
void CBasicDemoDlg::DisplayThread() {

    MV_DISPLAY_FRAME_INFO stDisplayInfo = { 0 };
    Mat Mat_display;

    while (m_bThreadState) {
      
        // wait until Mat_src not NULL
        if (Mat_src.empty()) {
            Sleep(5);
            continue;
        }

        // copy Mat_src and draw
        //Mat_display = dst.clone();
        Mat_display = Mat_src.clone(); 
        // Access are in conflict between 2 theads
        // -> fixed by waiting the first thread started
        putText(Mat_display, "FPS: "+ to_string(real_fps), Point(5, 25), FONT_HERSHEY_COMPLEX, 1, 128, 1, 8);
        //line(Mat_display, Point(0, line_position), Point(640, line_position), 0, 2, 8, 0);
        
        // display frame
        stDisplayInfo.hWnd = m_hwndDisplay;
        stDisplayInfo.pData = Mat_display.data;
        stDisplayInfo.nDataLen = Mat_display.rows * Mat_display.cols;//m_stImageInfo.nFrameLen;
        stDisplayInfo.nWidth = Mat_display.cols;//m_stImageInfo.nWidth;
        stDisplayInfo.nHeight = Mat_display.rows;//m_stImageInfo.nHeight;
        stDisplayInfo.enPixelType = PixelType_Gvsp_Mono8;//m_stImageInfo.enPixelType;
        m_pcMyCamera->DisplayOneFrame(&stDisplayInfo);

        UpdateData(FALSE); // update shrimp number and frame count to window

        // sleep 15ms/60fps
        Sleep(15);
    }
    Mat_display.release();
    return;
}

//en:Click Start button
void CBasicDemoDlg::OnBnClickedStartGrabbingButton() {
    // Set camera
    SetImageParameters(image_frame_rate, image_gain, image_exposure_time);

    if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing || NULL == m_pcMyCamera) {
        return;
    }

    std::memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    m_bThreadState = TRUE;

    // Start getting and processing image thread
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

    // Start display thread must be after camera start grabbing
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

//en:Click Stop button
void CBasicDemoDlg::OnBnClickedStopGrabbingButton() {
    GetImageParameters();
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

    // Stop grabbing and stop counting but not write result to file
    b_start_count = false;
    GetDlgItem(IDC_START_COUNT_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_STOP_COUNT_BUTTON)->EnableWindow(FALSE);
    trackers.clear();
    KalmanTracker::kf_count = 0;
    frameTrackingResult.clear();
    previous_frameTrackingResult.clear();
}

//en:Get Iamge Parameters
void CBasicDemoDlg::GetImageParameters() {
    int nRet = GetTriggerMode();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Trigger Mode Fail"), nRet);
    }

    image_exposure_time = GetExposureTime();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Exposure Time Fail"), nRet);
    }

    image_gain = GetGain();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Gain Fail"), nRet);
    }

    image_frame_rate = GetFrameRate();
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

//en:Set Parameter
void CBasicDemoDlg::SetImageParameters(double setFrameRate, double setGain, double setExposureTine) {
    
    bool bIsSetSucceed = true;
    int nRet = SetExposureTime(setExposureTine);
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Exposure Time Fail"), nRet);
    }
    nRet = SetGain(setGain);
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Gain Fail"), nRet);
    }
    nRet = SetFrameRate(setFrameRate);
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Frame Rate Fail"), nRet);
    }
    
    if (true == bIsSetSucceed)
    {
        //ShowErrorMsg(TEXT("Set Parameter Succeed"), nRet);
    }
}//*/

//en:Click Software button / SDK function
void CBasicDemoDlg::OnBnClickedSoftwareTriggerCheck() {
    UpdateData(TRUE);

    int nRet = SetTriggerSource();
    if (nRet != MV_OK)
    {
        return;
    }
}

//en:Click Execute button / SDK function
void CBasicDemoDlg::OnBnClickedSoftwareOnceButton() {
    if (TRUE != m_bStartGrabbing)
    {
        return;
    }

    m_pcMyCamera->CommandExecute("TriggerSoftware");
}

//en:Click Save BMP button 
void CBasicDemoDlg::OnBnClickedSaveBmpButton() {
    int nRet = SaveImage(MV_Image_Bmp);
    if (nRet == MV_E_INVALID_ADDRESS) {
        ShowErrorMsg(TEXT("Create new folder Saved_Image\n"), nRet);
        return;
    }
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Save bmp fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save bmp succeed"), nRet);
}

void CBasicDemoDlg::OnBnClickedSaveJpgButton() {
    int nRet = SaveImage(MV_Image_Jpeg);
    if (nRet == MV_E_INVALID_ADDRESS) {
        ShowErrorMsg(TEXT("Create new folder Saved_Image\n"), nRet);
        return;
    }
    if (MV_OK != nRet) {
        ShowErrorMsg(TEXT("Save jpg fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save jpg succeed"), nRet);
}

void CBasicDemoDlg::OnBnClickedSaveTiffButton() {
    int nRet = SaveImage(MV_Image_Tif);
    if (nRet == MV_E_INVALID_ADDRESS) {
        ShowErrorMsg(TEXT("Create new folder Saved_Image\n"), nRet);
        return;
    }
    if (MV_OK != nRet) {
        ShowErrorMsg(TEXT("Save tiff fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save tiff succeed"), nRet);
}

void CBasicDemoDlg::OnBnClickedSavePngButton() {
    int nRet = SaveImage(MV_Image_Png);
    if (nRet == MV_E_INVALID_ADDRESS) {
        ShowErrorMsg(TEXT("Create new folder Saved_Image\n"), nRet);
        return;
    }
    if (MV_OK != nRet) {
        ShowErrorMsg(TEXT("Save png fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save png succeed"), nRet);
}

//en:Exit from upper right corner / SDK function
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

// Apply settings from Setting window to Main window
void CBasicDemoDlg::OnBnClickedSettingButton() {
    Setting_Window* open_setting_windown = new Setting_Window();
    open_setting_windown->DoModal();
    // click ok -> update_setting = true
    if (open_setting_windown->update_setting == true) {
        // Reload SVM
        SVM = ml::SVM::load(SVM_dir);
        // get image processing paramerters from setting window
        flip_image = open_setting_windown->setting_flip_image;
        segment_binary_method = open_setting_windown->setting_segment_binary_method;
        if (segment_binary_method == L"Adaptive Threshold") {
                // adaptive threshold
                adaptiveThreshold_method = open_setting_windown->setting_adaptiveThreshold_method;
                adaptiveThreshold_KSize = open_setting_windown->setting_adaptiveThreshold_KSize;
                adaptiveThreshold_C = open_setting_windown->setting_adaptiveThreshold_C;
                // Check again
                if (adaptiveThreshold_method == L"MEAN_C" ||
                    adaptiveThreshold_method == L"GAUSSIAN_C") {
                    // do nothing
                }
                else {
                    AfxMessageBox(L"Adaptive Threshold Setting Error!");
                    delete open_setting_windown;
                    return;
                }
        }
        else if (segment_binary_method == L"Background Subtraction") {
                // back ground subtraction
                bgs_method = open_setting_windown->setting_bsg_method;
                bgs_threshold = open_setting_windown->setting_bsg_threshold;
                bgs_shadows = open_setting_windown->setting_bsg_shadow;
                bgs_history = open_setting_windown->setting_bsg_history;
                bsg_learning_rate = open_setting_windown->setting_bsg_learning_rate;
                // change back ground subtraction method
                if (bgs_method == L"MOG2") {
                    if (bgs_shadows == L"True") {
                        pBackSub = createBackgroundSubtractorMOG2(bgs_history, bgs_threshold, true);
                    }
                    else if (bgs_shadows == L"False"){
                        pBackSub = createBackgroundSubtractorMOG2(bgs_history, bgs_threshold, false);
                    }
                    else {
                        AfxMessageBox(L"Backgruond Subtraction Shadows Setting Error!");
                        delete open_setting_windown;
                        return;
                    }
                }
                else {
                    // for debug
                    AfxMessageBox(L"Backgruond Subtraction Setting Error!");
                    delete open_setting_windown;
                    return;
                }
        }
        else {
            // for debug
            AfxMessageBox(L"Segment to binary Setting Error!");
            delete open_setting_windown;
            return;
        }
        
        // SORT Tracking
        distance_threshold = open_setting_windown->setting_distance_threshold;
        min_hits = open_setting_windown->setting_min_hits;
        max_age = open_setting_windown->setting_max_age;

        // counting parameters
        line_position = open_setting_windown->setting_line_position;

        // Detection parameters
        min_area = open_setting_windown->setting_min_area;
        max_area = open_setting_windown->setting_max_area;
        min_width = open_setting_windown->setting_min_width;
        max_width = open_setting_windown->setting_max_width;
        min_height = open_setting_windown->setting_min_height;
        max_height = open_setting_windown->setting_max_height;
        // Image
        image_frame_rate = open_setting_windown->setting_image_frame_rate;
        image_gain = open_setting_windown->setting_image_gain;
        image_exposure_time = open_setting_windown->setting_image_exposure_time;
        //ROI
        ROI_Point_Left_Above = open_setting_windown->setting_Point_Left_Above;
        ROI_Point_Left_Below = open_setting_windown->setting_Point_Left_Below;
        ROI_Point_Right_Above = open_setting_windown->setting_Point_Right_Above;
        ROI_Point_Right_Below = open_setting_windown->setting_Point_Right_Below;
        Get_ROI_Mask();

        // Blur image
        blur_method = open_setting_windown->setting_blur_method;
        blur_kernel = open_setting_windown->setting_blur_kernel;
        if (blur_kernel != 1) { // blur kernel = 1 dont use blur
            if (blur_method == L"GAUSS" ||
                blur_method == L"AVG") {
                // do nothing
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
            if (morphological_method == L"Dilate" ||
                morphological_method == L"Erode"  ||
                morphological_method == L"Open"   ||
                morphological_method == L"Close") {
                // do nothing
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

//OpenCV convert data to cvMat, drop ROI and flip image
bool CBasicDemoDlg::Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData) {
    if (pstImageInfo->enPixelType == PixelType_Gvsp_Mono8) {
        Mat MatsrcImage = Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1, pData);
        // Flip image
        if (flip_image == L"None") {
            // do nothing
        }
        else if (flip_image == L"X") {
            cv::flip(MatsrcImage, MatsrcImage, 1);
        }
        else if (flip_image == L"Y") {
            cv::flip(MatsrcImage, MatsrcImage, 0);
        }
        // Drop ROI
        add(MatsrcImage, Mask_ROI, MatsrcImage);

        Mat_src = MatsrcImage.clone();
        MatsrcImage.release();
    }
    else {
        printf("unsupported pixel format\n");
        return false;
    }

    if (Mat_src.empty()) {
        printf("Convert to Mat failed.\n");
        return false;
    }
    return true;
}

//Input: Mat_src in gray scale
//Output1: vector<Trackingbox> detections
void CBasicDemoDlg::ImageProcessing() {
    // check Mat_src input
    if (Mat_src.empty()) return;

    frame_count++;
    
    Mat src_processing = Mat_src.clone();

    //Blur image
    if (blur_kernel != 1) { // kernel == 1 dont apply
        if (blur_method == L"AVG")
            blur(src_processing, src_processing, Size(blur_kernel, blur_kernel));
        else if (blur_method == L"GAUSS")
            GaussianBlur(src_processing, src_processing, Size(blur_kernel, blur_kernel), float(blur_kernel / 5.0));
    }
    // Segmentation to binary image
    //threshold(gpu_Mat_src, gpu_Mat_dst, 200, 255, THRESH_BINARY_INV);
    if (segment_binary_method == L"Adaptive Threshold") {
        if (adaptiveThreshold_method == L"MEAN_C")
            adaptiveThreshold(src_processing, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, adaptiveThreshold_KSize, adaptiveThreshold_C);
        else if (adaptiveThreshold_method == L"GAUSSIAN_C")
            adaptiveThreshold(src_processing, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, adaptiveThreshold_KSize, adaptiveThreshold_C);
    }
    else if (segment_binary_method == L"Background Subtraction") {
        pBackSub->apply(src_processing, dst, bsg_learning_rate);
    }
    else {
        AfxMessageBox(L"Image Processing Error Binary Segment Method!");
        return;
    }
    
    // morphological filter
    if (morphological_kernel != 1) {// kernel == 1 dont apply
        if (morphological_method == L"Open")
            morphologyEx(dst, dst, MORPH_OPEN, mo_kernel, Point(-1, -1), morphological_iterations);
        else if (morphological_method == L"Close")
            morphologyEx(dst, dst, MORPH_CLOSE, mo_kernel, Point(-1, -1), morphological_iterations);
        else if (morphological_method == L"Dilate")
            morphologyEx(dst, dst, MORPH_DILATE, mo_kernel, Point(-1, -1), morphological_iterations);
        else if (morphological_method == L"Erode")
            morphologyEx(dst, dst, MORPH_ERODE, mo_kernel, Point(-1, -1), morphological_iterations);
        else {
            AfxMessageBox(L"Image Processing Error Binary Segment Method!");
            return;
        }
    }
    
    // Distance transform
    //distanceTransform(dst, dst, DIST_L2, 3);
    //cv::threshold(dst, dst, 100, 255, THRESH_BINARY);

    // contours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    detections.clear();
    cv::findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    number_ROI_in_frame = 0;
    number_shrimp_in_frame = 0;
    double total_area_in_frame = 0;
    int total_shrimp_in_frame = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area < min_area || area > max_area) continue;

        Rect_<int> boundRect = boundingRect(contours[i]);
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
        M = moments(contours[i]);
        center_point = Point2f((M.m10 / M.m00), (M.m01 / M.m00));
        // get hu-moments
        vector<double> hu;
        cv::HuMoments(M, hu);
        // Log scale hu-moments
        for (int h = 0; h < 7; h++) {
            huMat.at<float>(h) = -1 * copysign(1.0, hu[h]) * log10(abs(hu[h]));
        }
        int response = SVM->predict(huMat);
        number_shrimp_in_frame += response;
        // Get tracking center
        TrackingCenter detect_center;
        detect_center.id = -1;
        detect_center.center = center_point;
        detect_center.svm_respone = response;
        detections.push_back(detect_center);

        total_area_in_frame += area;
        number_ROI_in_frame++;
        total_shrimp_in_frame += response;

        /*// draw contours
        drawContours(Mat_src, contours, i, 127); //*/
    }
    if (total_shrimp_in_frame > 0)
        avg_size = round((avg_size + total_area_in_frame / total_shrimp_in_frame) / 2);
    else
        avg_size = 0;
}

double CBasicDemoDlg::GetDistance(Point2f center_test, Point2f center_gt, float distance_threshold) {
    float delta_xx = center_test.x - center_gt.x; // center x
    float delta_yy = center_test.y - center_gt.y; // center y
    float un = delta_xx * delta_xx + delta_yy * delta_yy;
    float distance = sqrt(un);
    if (distance > distance_threshold)
        return Image_Width;
    return distance;
}
//Input: trackers, detections
//Output: vector<Trackingbox> frameTrackingResult
void CBasicDemoDlg::SORT(int max_age, int min_hits, double distanceThreshold) {

    if (trackers.size() == 0) { // the first frame
        for (uint i = 0; i < detections.size(); i++) {
            KalmanTracker trk = KalmanTracker(detections[i].center);
            trackers.push_back(trk);
        }
        return;
    }

    // 3.1. get predicted locations from existing trackers.
    vector<Point2f> predictedCenters;
    for (auto it = trackers.begin(); it != trackers.end();) {
        Point2f pCenter = (*it).predict();
        if (pCenter.y > 0 && pCenter.y < Image_Height + 20 &&
            pCenter.x > 0 && pCenter.x < Image_Width) {
            predictedCenters.push_back(pCenter);

            /*//Draw predict centers brown to view and debug
            circle(Mat_src, pCenter, 2, 128, 2, 8, 0); //*/

            it++;
        }
        else {
            it = trackers.erase(it);
        }
    }

    // 3.2. associate detections to tracked object
    unsigned int trkNum = predictedCenters.size();
    unsigned int detNum = detections.size();
    vector<vector<double>> distanceMatrix;
    distanceMatrix.resize(trkNum, vector<double>(detNum, 0));
    for (unsigned int i = 0; i < trkNum; i++) { // compute distance matrix
        for (unsigned int j = 0; j < detNum; j++) { 
            distanceMatrix[i][j] = GetDistance(predictedCenters[i], detections[j].center, distanceThreshold);
        }
    }
    // fix bug//note this
    if (distanceMatrix.size() == 0) return;

    HungarianAlgorithm HungAlgo;
    vector<int> assignment; // the resulting assignment is [track(prediction) : detection]
    HungAlgo.Solve(distanceMatrix, assignment); // with len=predict Number

    // find matched, unmatched_detections and unmatched_predictions
    set<int> unmatchedDetections;
    set<int> allItems;
    set<int> matchedItems;
    if (detNum > trkNum) { //there are unmatched detections
        for (unsigned int n = 0; n < detNum; n++) {
            allItems.insert(n);
        }
        for (unsigned int i = 0; i < trkNum; ++i) {
            matchedItems.insert(assignment[i]);
        }
        set_difference(allItems.begin(), allItems.end(), // find unmatched detection
            matchedItems.begin(), matchedItems.end(),
            insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin()));
    }

    // 3.3. update tracker
    for (unsigned int i = 0; i < trkNum; ++i) {
        if (assignment[i] == -1) { // update unmatched tracker
            trackers[i].updateWithPredictedCenter(predictedCenters[i]);
            continue;
        }
        if (distanceMatrix[i][assignment[i]] == Image_Width) { // filter out with large distance
            /*// Stop this time to view 2 points matched with large distance for debug
            cv::waitKey(); //*/
            trackers[i].updateWithPredictedCenter(predictedCenters[i]);
            unmatchedDetections.insert(assignment[i]);
        }
        else { // update tracker with matched point
            trackers[i].updateWithMatchedDetection(detections[assignment[i]], min_hits);
            /*//Draw line between 2 matched points and save distance to file txt
            line(Mat_src, predictedCenters[i], detections[assignment[i]].center, 128, 2, 8, 0);
            /*CStdioFile StdFile;
            CFileException ex;
            StdFile.Open(L"C:/Users/ADMIN/Desktop/SaveDistance.txt", CFile::modeNoTruncate | CFile::modeWrite, &ex);
            CString text;
            text.Format(L"%f", distanceMatrix[i][assignment[i]]);
            text = text + L"\n";
            StdFile.SeekToEnd();
            StdFile.WriteString(text);
            StdFile.Close();//*/
        }
    }
    // Create and initialise new trackers for unmatched detections
    for (auto umd : unmatchedDetections) {
        KalmanTracker tracker = KalmanTracker(detections[umd].center);
        trackers.push_back(tracker);
    }

    // Get trackers' output
    frameTrackingResult.clear();
    for (auto it = trackers.begin(); it != trackers.end();) {
        if (((*it).m_time_since_update == 0) // tracker updated in current frame
          //&&((*it).m_age == 0) // tracker updated with detection in current frame
          &&((*it).confirmed_tracker == true)) { // real tracker
            TrackingCenter res;
            res.center = (*it).get_state();
            res.id = (*it).m_id; // m_id = kf_count
            res.svm_respone = (*it).svm_number;
            frameTrackingResult.push_back(res);
            /*//Draw output Point by white
            circle(Mat_src, res.center, 3, 200, 3, 8, 0);
            putText(Mat_src, to_string(res.id), Point(res.center.x + 20, res.center.y),
                FONT_HERSHEY_COMPLEX_SMALL, 1, 128, 1, 8);
            putText(Mat_src, to_string(res.svm_respone)+":", Point(res.center.x, res.center.y),
                FONT_HERSHEY_COMPLEX_SMALL, 1, 128, 1, 8); //*/
        }
        if (it != trackers.end() && (*it).m_age > max_age) {  // remove dead tracker
            it = trackers.erase(it);
        }
        else {
            it++;
        }
    }
}
// Counting function
// Input: previous_tracking and current_tracking
// Ouput: counter
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
                    if (frameTrackingResult[i].svm_respone == 1)
                        F1_counter++;
                    else if (frameTrackingResult[i].svm_respone == 2)
                        F2_counter++;
                    else if (frameTrackingResult[i].svm_respone == 3)
                        F3_counter++;
                    else if (frameTrackingResult[i].svm_respone == 4)
                        F4_counter++;
                    counter += frameTrackingResult[i].svm_respone;
                }
            }
        } 
    }
    // copy frame tracking results to previous tracking result
    previous_frameTrackingResult.clear();
    std::copy(frameTrackingResult.begin(), frameTrackingResult.end(), back_inserter(previous_frameTrackingResult));
    frameTrackingResult.clear();
    return;
}

void CBasicDemoDlg::OnBnClickedStartCountButton() {
    
    b_start_count = true;
    //EnableControls(TRUE);
    GetDlgItem(IDC_START_COUNT_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_STOP_COUNT_BUTTON)->EnableWindow(TRUE);
    
    return;
}
void CBasicDemoDlg::OnBnClickedStopCountButton() {
    
    b_start_count = false;
    //EnableControls(TRUE);
    GetDlgItem(IDC_START_COUNT_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_STOP_COUNT_BUTTON)->EnableWindow(FALSE);
    // Reset vector
    trackers.clear();
    KalmanTracker::kf_count = 0;
    frameTrackingResult.clear();
    previous_frameTrackingResult.clear();
    // write result to file, open and save file mode write
    CStdioFile StdFile;
    CFileException ex;
    if (!StdFile.Open(global_filename, CFile::modeNoTruncate | CFile::modeWrite, &ex)) {
        CString error;
        error.Format(L"Error when open file to write result!\nCause = %d", ex.m_cause);
        AfxMessageBox(error);
        int message = AfxMessageBox(L"Create new file " + global_filename + L"?", MB_YESNO);
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
    // set pointer to the end of file
    StdFile.SeekToEnd();
    // write result
    StdFile.WriteString(text_result);
    StdFile.Close();
    
    return;
}
void CBasicDemoDlg::OnBnClickedResetNumberButton() {
    // Reset all number in main window
    counter = 0;
    frame_count = 0;
    F1_counter = 0;
    F2_counter = 0;
    F3_counter = 0;
    F4_counter = 0;
    avg_size = 0;
    UpdateData(FALSE);
}

void CBasicDemoDlg::OnBnClickedLogButton() {
    LogHistory* open_logHistory_windown = new LogHistory();
    open_logHistory_windown->DoModal();
    return;
}

bool CBasicDemoDlg::IsLeft(Point2f A, Point2f B, Point2f Check) { // Find x position on the line
    float x_pos = A.x -(B.x - A.x) * (Check.y - A.y) / (A.y - B.y); // from y position
    if (x_pos >= Check.x)
        return true;
    else
        return false;
}
void CBasicDemoDlg::Get_ROI_Mask() { // return Mask_ROI
    CString sTokenX, sTokenY;
    Point2f Left_Above, Left_Below, Right_Above, Right_Below;

    AfxExtractSubString(sTokenX, ROI_Point_Left_Above, 0, ',');
    AfxExtractSubString(sTokenY, ROI_Point_Left_Above, 1, ',');
    Left_Above = Point2f(_ttof(sTokenX), _ttof(sTokenY));

    AfxExtractSubString(sTokenX, ROI_Point_Left_Below, 0, ',');
    AfxExtractSubString(sTokenY, ROI_Point_Left_Below, 1, ',');
    Left_Below = Point2f(_ttof(sTokenX), _ttof(sTokenY));

    AfxExtractSubString(sTokenX, ROI_Point_Right_Above, 0, ',');
    AfxExtractSubString(sTokenY, ROI_Point_Right_Above, 1, ',');
    Right_Above = Point2f(_ttof(sTokenX), _ttof(sTokenY));

    AfxExtractSubString(sTokenX, ROI_Point_Right_Below, 0, ',');
    AfxExtractSubString(sTokenY, ROI_Point_Right_Below, 1, ',');
    Right_Below = Point2f(_ttof(sTokenX), _ttof(sTokenY));

    for (int i = 0; i < Image_Width; i++) {
        for (int j = 0; j < Image_Height; j++) {
            Point2f Check = Point2f(i, j);
            if (!IsLeft(Left_Above, Left_Below, Check) // Right side
             && IsLeft(Right_Above, Right_Below, Check)) // Left side
                Mask_ROI.at<uchar>(j, i) = 0;
            else
                Mask_ROI.at<uchar>(j, i) = 255;
        }
    }
}