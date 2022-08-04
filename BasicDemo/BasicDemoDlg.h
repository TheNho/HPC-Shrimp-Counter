// Shrimp Counting Project
// Implement by The Nho 2022
// BasicDemoDlg.h : header file
#pragma once
#include "afxwin.h" 
#include "MvCamera.h"
//opencv core
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
//cuda gpu opencv
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudaimgproc.hpp >
#include <opencv2/cudabgsegm.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/dnn.hpp>
// sort tracking
#include "KalmanTracker.h"
#include "Hungarian.h"
#include <set>
// sub window
#include "Setting_Window.h"
#include "LogHistory.h"

using namespace std;
using namespace cv;
using namespace cv::cuda;
using namespace cuda;

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

/*en:Control corresponding variable*/
private:
    BOOL                    m_bSoftWareTriggerCheck;
    double                  m_dExposureEdit;
    double                  m_dGainEdit;
    double                  m_dFrameRateEdit;   

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
    int GetExposureTime();               // en:Set Exposure Time
    int SetExposureTime(); 
    int GetGain();                       // en:Set Gain
    int SetGain();
    int GetFrameRate();                  //en:Set Frame Rate
    int SetFrameRate();
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

    // for sort y value
    struct _sort_point_y_ {
        bool operator() (Point pt1, Point pt2) { 
            return (pt1.y > pt2.y); }
    } sort_point_y;

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
    afx_msg void OnBnClickedGetParameterButton();       //en:Get Parameter
    afx_msg void OnBnClickedSetParameterButton();       //en:Exit from upper right corner
  
    afx_msg void OnClose();

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    int GrabThreadProcess();
    //opencv convert buffer data to Mat
    bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData, cv::Mat *srcImage);
    void SORT(int max_age, int min_hits, double iouThreshold);
    double GetIOU(Rect_<float> bb_test, Rect_<float> bb_gt);
    void ImageProcessing_GPU();
    void AdaptiveThreshold_GPU(GpuMat gsrc, GpuMat &gdst);
    void My_Simple_Counting(int tolerance_x, float max_square_distance);
    void SORT_Counting();
    void DisplayThread();
    void SettingInitial();
    afx_msg void OnBnClickedSettingButton();

private:
    int real_fps = 0;
    bool b_start_count = false;
    uint64 frame_count = 0;
    uint64 counter = 0;

    // Opencv variables
    Mat Mat_src;
    GpuMat gpu_Mat_src, gpu_Mat_dst;
    // blur
    Ptr<cuda::Filter> cuda_filter;
    // Adaptive threshold
    Ptr<cuda::Filter> gpu_adaptiveThreshold_filter;
    // morphological
    Mat mo_kernel;
    Ptr<cuda::Filter> mo_filter;
    // Backgruond subtraction
    Ptr<BackgroundSubtractor> pBackSub;
    // Detection
    vector<Point> current_centers;
    vector<Point> previous_centers;
    vector<TrackingBox> detections; // detection in one frame
    vector<array<float, 7>> HuMoments; // contain stack [humoment] of 2->4 shirmps in one detection , pop up after classify
    vector<KalmanTracker> trackers;  // reset this in stop count
    vector<TrackingBox> previous_frameTrackingResult;
    vector<TrackingBox> frameTrackingResult; // contain id, box position

public:
    afx_msg void OnBnClickedStartCountButton();
    afx_msg void OnBnClickedStopCountButton();
    afx_msg void OnBnClickedResetNumberButton();
    afx_msg void OnBnClickedLogButton();
public:
    CStdioFile StdFile;

    // define save directory
    CString nFilename = L"Result.result";
};
