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
	int64 frame;
	int64 id;
	Point2f center;
}TrackingCenter;

// This class represents the internel state of individual tracked objects observed as bounding box.
class KalmanTracker
{
public:
	KalmanTracker(StateType initCenter)
	{
		init_kf(initCenter);
		m_time_since_update = 0;
		m_hits = 0;
		m_hit_streak = 0;
		m_age = 0;
		m_id = kf_count;
		kf_count++;
	}

	~KalmanTracker()
	{
		m_history.clear();
	}

	StateType predict();
	void update(StateType stateMat);
	
	StateType get_state();

	static uint64 kf_count;

	int m_time_since_update;
	int m_hits;
	int m_hit_streak;
	int m_age;
	uint64 m_id;

private:
	void init_kf(StateType stateMat);

	cv::KalmanFilter kf;
	cv::Mat measurement;

	std::vector<StateType> m_history;
};

#endif