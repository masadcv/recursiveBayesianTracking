#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
void filterImage(const cv::Mat &inImage, cv::Mat &outMask);
cv::Mat segmentImage(const cv::Mat& inImage, int threshold = 10);

int main(int argc, char *argv[])
{
    char ch = ' ';
    int threshVal = 10;
    // open a video stream
    cv::VideoCapture vid;

    if(!vid.open(0))
        return -1;

    while(1)
    {
        cv::Mat inImage;
        vid >> inImage;

        if(inImage.empty())
            break;

        cv::Mat outMask;
        //outMask = segmentImage(inImage, threshVal);
        filterImage(inImage, outMask);
        cv::imshow("Output", outMask);

        ch = cv::waitKey(1);

        if(ch == 27)
            break;

        if(ch =='a')
            threshVal +=10;

        if(ch == 'z')
            threshVal -=10;

    }
    vid.release();
    return 0;
}

void filterImage(const cv::Mat &inImage, cv::Mat &outMask)
{
    //cv::inRange(inImage, cv::Vec3b(0, 0, 0), cv::Vec3b(255, 0,0), outMask);
    cv::Mat hsvImage;
    cv::cvtColor(inImage, hsvImage, CV_BGR2HSV);
    cv::inRange(hsvImage, cv::Scalar(0, 150, 150), cv::Scalar(7, 255, 255), outMask);

}

cv::Mat segmentImage(const cv::Mat& inImage, int threshold)
{
	cv::Mat segImage;
	segImage = cv::Mat::zeros(inImage.rows, inImage.cols, CV_8UC1);

	for(int i = 0; i < inImage.cols; i++)
	{
		for(int j = 0; j < inImage.rows; j++)
		{
			cv::Vec3b colorPixel;
			colorPixel = inImage.at<cv::Vec3b>(j,i);

			// typically opencv images are BGR
			// colorPixel[0] --> B
			// colorPixel[1] --> G
			// colorPixel[2] --> R

			// implementing simple thresholding based segmentation as in
			// Song, Jie, et al. "In-air gestures around unmodified mobile devices."
			// Proceedings of the 27th annual ACM symposium on User interface software and technology. ACM, 2014.
			// equation 1

			bool threshDecision = std::min(colorPixel[2] - colorPixel[1], colorPixel[2] - colorPixel[0]) > threshold;

			segImage.at<uchar>(j, i) = ((uchar)threshDecision)*255;
		}
	}

	return segImage;
}
