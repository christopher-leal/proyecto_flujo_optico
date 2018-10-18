#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

Mat mascara[3];
Mat transparencia(Mat,Mat,int,int);

int main()
{
    mascara[0] = imread("anbu.png",CV_LOAD_IMAGE_UNCHANGED);
    mascara[1] = imread("obito.png",CV_LOAD_IMAGE_UNCHANGED);
    mascara[2] = imread("rock.png",CV_LOAD_IMAGE_UNCHANGED);

	VideoCapture cap;
	CascadeClassifier detector("haarcascade_frontalface_alt.xml" );

	if(!cap.open(0))
		cout << "No se puede acceder a la webcam." << endl;


	while(true)
	{
		Mat dest, gray, imagen,mascara1,mascara2,mascara3;


		cap >> imagen;
        flip(imagen,imagen,1);
		cvtColor(imagen, gray, CV_BGR2GRAY);
		equalizeHist(gray, dest);

		vector<Rect> rect;
		detector.detectMultiScale(dest, rect, 1.2, 3, 0, Size(60,60),Size(400,500));

		for(Rect rc : rect)
		{
		    if(rect.size()==1){
            resize(mascara[0],mascara1,Size(rect.at(0).width*1.5,rect.at(0).height*1.5));
            imagen=transparencia(mascara1,imagen,rect.at(0).x-rect.at(0).x*.15,rect.at(0).y-rect.at(0).y*.3);
		    }

		    if(rect.size()==2){
            resize(mascara[0],mascara1,Size(rect.at(1).width*1.5,rect.at(1).height*1.5));
            imagen=transparencia(mascara1,imagen,rect.at(1).x-rect.at(1).x*.15,rect.at(1).y-rect.at(1).y*.3);

            resize(mascara[1],mascara2,Size(rect.at(0).width*1.5,rect.at(0).height*1.5));
            imagen=transparencia(mascara2,imagen,rect.at(0).x-rect.at(0).x*.15,rect.at(0).y-rect.at(0).y*.3);
		    }

		    if(rect.size()==3){
            resize(mascara[0],mascara1,Size(rect.at(2).width*1.5,rect.at(2).height*1.5));
            imagen=transparencia(mascara1,imagen,rect.at(2).x-rect.at(2).x*.15,rect.at(2).y-rect.at(2).y*.3);

            resize(mascara[1],mascara2,Size(rect.at(1).width*1.5,rect.at(1).height*1.5));
            imagen=transparencia(mascara2,imagen,rect.at(1).x-rect.at(1).x*.15,rect.at(1).y-rect.at(1).y*.3);

            resize(mascara[2],mascara3,Size(rect.at(0).width*1.5,rect.at(0).height*1.5));
            imagen=transparencia(mascara3,imagen,rect.at(0).x-rect.at(0).x*.15,rect.at(0).y-rect.at(0).y*.3);
		    }
		}

		imshow("Snapchat", imagen);

		if(waitKey(1) >= 0) break;
	}

	return 1;
}

Mat transparencia(Mat img, Mat video,int x, int y){
    Mat trans;

    Mat canales[4],canal[3];
    split(img, canales);
    split(video, canal);

    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if((canales[3].at<uchar>(i,j))==255){
                canal[0].at<uchar>(i+y,j+x)=canales[0].at<uchar>(i,j);
                canal[1].at<uchar>(i+y,j+x)=canales[1].at<uchar>(i,j);
                canal[2].at<uchar>(i+y,j+x)=canales[2].at<uchar>(i,j);
            }
        }
    }
    merge(canal,3,trans);
    return trans;
}
