// KalmanTracker.cpp: KalmanTracker Class Implementation Declaration
// Using Kalman Tracker OpenCV
// This code is implemented by The Nho 2022

#include "KalmanTracker.h"

uint64 KalmanTracker::kf_count = 0;

// Initialize Kalman filter with the first center
void KalmanTracker::init_kf(StateType initial_center)
{
	int stateNum = 4;
	int measureNum = 2;
	kf = KalmanFilter(stateNum, measureNum, 0);

	measurement = Mat::zeros(measureNum, 1, CV_32F);

	kf.transitionMatrix = (Mat_<float>(stateNum, stateNum) <<
		1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1); // Ma trận chuyển đổi trạng thái A

	setIdentity(kf.measurementMatrix); //measurement/observation matrix (H)
	setIdentity(kf.processNoiseCov, Scalar::all(1e-2));
	setIdentity(kf.measurementNoiseCov, Scalar::all(1e-1)); //measurement noise covariance matrix (R)
	setIdentity(kf.errorCovPost, Scalar::all(1));

	//install the first point
	kf.statePost.at<float>(0, 0) = initial_center.x;
	kf.statePost.at<float>(1, 0) = initial_center.y;
	//initial the first velocity
	//kf.statePost.at<float>
	//kf.statePost.at<float>
}

// Predict the estimated Center.
StateType KalmanTracker::predict(){
	// predict
	Mat p = kf.predict();
	m_hit_streak = 0;
	m_time_since_update += 1;

	StateType predictCenter = Point2f(p.at<float>(0, 0), p.at<float>(1, 0));
	// for view results
	//m_history.push_back(predictCenter);
	return predictCenter;
}

// Update the state vector with observed Center.
void KalmanTracker::updateWithMatchedDetection(TrackingCenter detection, int min_hits){
	//m_history.clear();
	m_time_since_update = 0; 
	m_hit_streak += 1; 
	m_hits += 1;
	svm_number = detection.svm_respone; // update number
	area = detection.contours_area;
	if (m_hits > min_hits) { // hit in many continuous frame -> confirm real tracker
		m_age = 0; //reset age=0 to avoid delete
		confirmed_tracker = true;
	}
	// measurement is a cv::Mat, update measurement
	measurement.at<float>(0, 0) = detection.center.x;
	measurement.at<float>(1, 0) = detection.center.y;
	// corect measurement
	kf.correct(measurement);
}
void KalmanTracker::updateWithPredictedCenter(StateType stateMat) {
	//m_history.clear();
	m_time_since_update = 0;
	m_hit_streak += 1;
	m_age += 1;
	m_hits = 0; // reset number hit if dont match with anny detection
	
	measurement.at<float>(0, 0) = stateMat.x;
	measurement.at<float>(1, 0) = stateMat.y;
	
	kf.correct(measurement);
}
// Return the current state vector
StateType KalmanTracker::get_state()
{
	Mat s = kf.statePost;
	return Point2f(s.at<float>(0, 0), s.at<float>(1, 0));
}


/*
// --------------------------------------------------------------------
// Kalman Filter Demonstrating, a 2-d ball demo
// --------------------------------------------------------------------

const int winHeight = 600;
const int winWidth = 800;

Point mousePosition = Point(winWidth >> 1, winHeight >> 1);

// mouse event callback
void mouseEvent(int event, int x, int y, int flags, void *param)
{
	if (event == CV_EVENT_MOUSEMOVE) {
		mousePosition = Point(x, y);
	}
}

void TestKF();

void main()
{
	TestKF();
}


void TestKF()
{
	int stateNum = 4;
	int measureNum = 2;
	KalmanFilter kf = KalmanFilter(stateNum, measureNum, 0);

	// initialization
	Mat processNoise(stateNum, 1, CV_32F);
	Mat measurement = Mat::zeros(measureNum, 1, CV_32F);

	kf.transitionMatrix = *(Mat_<float>(stateNum, stateNum) <<
		1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1);

	setIdentity(kf.measurementMatrix);
	setIdentity(kf.processNoiseCov, Scalar::all(1e-2));
	setIdentity(kf.measurementNoiseCov, Scalar::all(1e-1));
	setIdentity(kf.errorCovPost, Scalar::all(1));

	randn(kf.statePost, Scalar::all(0), Scalar::all(winHeight));

	namedWindow("Kalman");
	setMouseCallback("Kalman", mouseEvent);
	Mat img(winHeight, winWidth, CV_8UC3);

	while (1)
	{
		// predict
		Mat prediction = kf.predict();
		Point predictPt = Point(prediction.at<float>(0, 0), prediction.at<float>(1, 0));

		// generate measurement
		Point statePt = mousePosition;
		measurement.at<float>(0, 0) = statePt.x;
		measurement.at<float>(1, 0) = statePt.y;

		// update
		kf.correct(measurement);

		// visualization
		img.setTo(Scalar(255, 255, 255));
		circle(img, predictPt, 8, CV_RGB(0, 255, 0), -1); // predicted point as green
		circle(img, statePt, 8, CV_RGB(255, 0, 0), -1); // current position as red

		imshow("Kalman", img);
		char code = (char)waitKey(100);
		if (code == 27 || code == 'q' || code == 'Q')
			break;
	}
	destroyWindow("Kalman");
}
*/
