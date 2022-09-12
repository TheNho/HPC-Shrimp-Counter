// Shrimps Counting Project
// Implement by The Nho 2022
// BasicDemoDlg.h : header file
#pragma once
#include "afxwin.h" 
#include "MvCamera.h"
//opencv core
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include "KalmanTracker.h"
#include "Hungarian.h"
#include <set>
// sub window
#include "Setting_Window.h"
#include "LogHistory.h"
#include <psapi.h> // for check if camera driver installed

#define ARRAY_SIZE 1024
#define PSAPI_VERSION 1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;

// CBasicDemoDlg dialog
class CBasicDemoDlg : public CDialog
{
// Construction
public:
	CBasicDemoDlg(CWnd* pParent = NULL);	// Standard constructor

    // Dialog Data
	enum { IDD = IDD_BasicDemo_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//en:Control corresponding variable*/
private:
    BOOL                    m_bSoftWareTriggerCheck;
    //double                  m_dExposureEdit;
    //double                  m_dGainEdit;
    //double                  m_dFrameRateEdit;   

    CComboBox               m_ctrlDeviceCombo;                // en:Enumerated device
    int                     m_nDeviceCombo;
    CFont                   Font_big;
private:
    //en:Window initialization
    void DisplayWindowInitial();

    void EnableControls(BOOL bIsCameraReady);
    void ShowErrorMsg(CString csMessage, int nErrorNum);

    int SetTriggerMode();                // en:Set Trigger Mode
    int GetTriggerMode();
    double GetExposureTime();               // en:Set Exposure Time
    int SetExposureTime(float ExposureTime);
    double GetGain();                       // en:Set Gain
    int SetGain(float Gian);
    double GetFrameRate();                  //en:Set Frame Rate
    int SetFrameRate(float FrameRate);
    int GetTriggerSource();              //en:Set Trigger Source
    int SetTriggerSource();

    int CloseDevice();                   //en:Close Device
    int SaveImage(enum MV_SAVE_IAMGE_TYPE enSaveImageType);                     //en:Save Image

    //en:Remove custom pixel formats
    bool RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat);

private:
    BOOL                    m_bOpenDevice;         //en:Whether to open device
    BOOL                    m_bStartGrabbing;      //en:Whether to start grabbing
    int                     m_nTriggerMode;        //en:Trigger Mode
    int                     m_nTriggerSource;      //en:Trigger Source

    CMvCamera*              m_pcMyCamera;          //en:CMyCamera packed commonly used interface
    HWND                    m_hwndDisplay;         //en:Display Handle
    MV_CC_DEVICE_INFO_LIST  m_stDevList;         

    CRITICAL_SECTION        m_hSaveImageMux;
    unsigned char*          m_pSaveImageBuf;
    unsigned int            m_nSaveImageBufSize;
    MV_FRAME_OUT_INFO_EX    m_stImageInfo;

    void*                   m_hGrabThread;           // en:Grab thread handle, thread 1, image processing
    void*                   d_hGrabThread;           // display thread handle, to call thread 2 display
    BOOL                    m_bThreadState;

public:
    /*en:Initialization*/
    afx_msg void OnBnClickedEnumButton();               //en:Find Devices
    afx_msg void OnBnClickedOpenButton();               //en:Open Devices
    afx_msg void OnBnClickedCloseButton();              //en:Close Devices
   
    /*en:Image Acquisition*/
    afx_msg void OnBnClickedContinusModeRadio();        //en:Continus Mode
    afx_msg void OnBnClickedTriggerModeRadio();         //en:Trigger Mode
    afx_msg void OnBnClickedStartGrabbingButton();      //en:Start Grabbing
    afx_msg void OnBnClickedStopGrabbingButton();       //en:Stop Grabbing
    afx_msg void OnBnClickedSoftwareTriggerCheck();     //en:Software Trigger
    afx_msg void OnBnClickedSoftwareOnceButton();       // en:Software Trigger Execute Once
  
    /*en:Image Save*/
    afx_msg void OnBnClickedSaveBmpButton();            //en:Save bmp
    afx_msg void OnBnClickedSaveJpgButton();            //en:Save jpg
    afx_msg void OnBnClickedSaveTiffButton();
    afx_msg void OnBnClickedSavePngButton();
  
    /*en:Parameters Get and Set*/
    void GetImageParameters();       //en:Get Parameter
    void SetImageParameters(double setFrameRate, double setGain, double setExposureTine);       //en:Exit from upper right corner
  
    afx_msg void OnClose();

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    int GrabThreadProcess();
    //opencv convert buffer data to Mat
    bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData);
    void SORT(int max_age, int min_hits, double iouThreshold);
    double GetDistance(Point2f center_test, Point2f center_gt, float distance_threshold);
    void ImageProcessing();
    void SORT_Counting();
    void DisplayThread();
    void SettingInitial();
    afx_msg void OnBnClickedSettingButton();
    bool IsLeft(Point2f A, Point2f B, Point2f Check);
    void Get_ROI_Mask();

private:
    int real_fps = 0;
    bool b_start_count = false;
    uint64 frame_count = 0;
    uint64 counter = 0;
    uint64 F1_counter = 0;
    uint64 F2_counter = 0;
    uint64 F3_counter = 0;
    uint64 F4_counter = 0;
    double avg_size = 0;
    // Opencv variables
    Mat Mat_src;
    Mat dst; // dst is a binary image
    Mat mo_kernel;
    Ptr<BackgroundSubtractor> pBackSub;
    // ROI
    Mat Mask_ROI = Mat::zeros(Image_Height, Image_Width, CV_8UC1);
    // Detection
    Mat huMat = Mat::zeros(1, 7, CV_32F);
    Moments M;
    Point2f center_point;
    Ptr<ml::SVM> SVM;
    String SVM_dir = "SVM.xml";
    vector<TrackingCenter> detections; // detection in one frame
    vector<array<float, 7>> HuMoments; // contain stack [humoment] of 2->4 shirmps in one detection , pop up after classify
    vector<KalmanTracker> trackers;  // reset this in stop count
    vector<TrackingCenter> previous_frameTrackingResult;
    vector<TrackingCenter> frameTrackingResult; // contain id, box position

public:
    afx_msg void OnBnClickedStartCountButton();
    afx_msg void OnBnClickedStopCountButton();
    afx_msg void OnBnClickedResetNumberButton();
    afx_msg void OnBnClickedLogButton();
public:
    // define save directory
    CString nFilename = L"Result.result";
private:
    //char dir_save_image[ARRAY_SIZE] = "Saved_Image/";
    // variable of driver installing
    char dir_driver_install[ARRAY_SIZE] = "Driver\\install.bat";
    TCHAR driver_name[ARRAY_SIZE] = TEXT("mvu3v.sys");
    int number_ROI_in_frame;
    int number_shrimp_in_frame;
    
};
