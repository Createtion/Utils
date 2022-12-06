#ifndef TRANS_H
#define TRANS_H

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;


class Trans{

   private:

    //数据增强参数
    int Min_Alpha                = 8;              //亮度阈值
    int Max_Alpha                = 10;

    double Min_size              = 0.8;          //缩放阈值
    double Max_size              = 1.2;

    int rotate_num               = 1;             //旋转参数
    int brightness_num           = 3;         //亮度参数
    int size_num                 = 2;               //缩放参数
    int translation_num          = 2;        //平移参数
    int shift_num                = 1;              //分割参数
    bool shift_only              = false;
    int shift_only_label         = 7;

   public:
    int func=6;
    //0:no; 1:angle; 2:bright; 3:a+b; 4:size; 5:a+s 6:b+s; 7:a+b; 8:shift; 9:merge; 10:Mask; 11:channl_6_size; 12:b+c 13:translation
    bool circle                  = false;
    bool misjudgment             = true;
    bool add_src                 = true;
    bool out                     = true;
    bool data_spilt              = false;
    string pic_type              = ".bmp";
    string project_filename      = "wazhou_outplane";
    string input_filename        = "/home/luoziyi/Trans/data/chejiagong_endplane/input/";
    string python_path           = "/home/luoziyi/Trans/";
    //fangchengai_trans,203_outplane,mifengquan_trans,ccwq_out,203_innerplane,mifengquan_wyj

    int label_num                = 0;
    int picture_num              = size_num*brightness_num*rotate_num;

    Trans();
    virtual ~Trans();

    void label_write(String fn,String p_time);
    void rotate_label_write(String fn,String p_time,Mat src);
    void shift_label_write(String fn,String p_time,Mat src);
    void trans_label_write(String fn,String p_time,Mat src);
    vector<Mat> Data_Augmentation(vector<Mat> src,vector<Mat> &resault);
    virtual vector<Mat> circle_detect(vector<Mat> src);
    vector<Mat> angle_trans(vector<Mat> circle_image);
    vector<Mat> brightness_trans_1(vector<Mat> angle_image);
    Mat brightness_trans_2(Mat angle_image,Mat brightness_image,float alpha);
    vector<Mat> size_trans(vector<Mat> size_image);
    vector<Mat> shift_trans(vector<Mat> src);
    vector<Mat> merge(vector<Mat> src);
    vector<Mat> channl_6_size(vector<Mat> src);
    vector<Mat> Mask(vector<Mat> src);
    vector<Mat> translation(vector<Mat> src);

    void removeEdgeNoise(cv::Mat &image);
    bool fitCircle(const vector<cv::Point> &points, cv::Point2f &center, float &radius);
    void getOuterCircle(cv::Mat binary_img, cv::Point2f &center, float &radius);
    void pointPolar2Cart2(const cv::Point2f &pt, cv::Point & pt_result,const cv::Point2f &center, const float &radius);

};

#endif
