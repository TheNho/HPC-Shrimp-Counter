// KalmanTracker.h: KalmanTracker Class Declaration
// Code edited by The Nho 2022

#ifndef KALMAN_H
#define KALMAN_H 2

//#include "opencv2/video/tracking.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

#define StateType Point2f

typedef struct _TrackingCenter
{
	int64 id;
	Point2f center;
	double contours_area;
	int svm_respone;
}TrackingCenter;

// This class represents the internel state of individual tracked objects observed as bounding box.
class KalmanTracker
{
public:
	KalmanTracker(StateType initial_center)
	{
		init_kf(initial_center);
		m_time_since_update = 0; // number of predict after update
		m_hits = 0; // number of continuous hit with detections
		m_hit_streak = 0; // number of update of after predict
		m_age = 0;  // number of update after confirm real tracker
		confirmed_tracker = false; // real track or fake track
		m_id = kf_count; // id
		svm_number = 1; // number of shirmps in contour
		area = 0;
		kf_count++;
	}

	~KalmanTracker()
	{
		//m_history.clear();
	}

	StateType predict();
	void updateWithMatchedDetection(TrackingCenter detection, int min_hits);
	void updateWithPredictedCenter(StateType stateMat);
	
	StateType get_state();

	static uint64 kf_count;
	bool confirmed_tracker;
	int m_time_since_update;
	int m_hits;
	int m_hit_streak;
	int m_age;
	uint64 m_id;
	int svm_number;
	double area;
private:
	void init_kf(StateType stateMat);

	cv::KalmanFilter kf;
	cv::Mat measurement;

	//std::vector<StateType> m_history;
};

#endif