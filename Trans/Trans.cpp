#include <Trans.h>


Trans::Trans(){
    input_filename.replace(input_filename.find("chejiagong_endplane"),19,project_filename);
}

Trans::~Trans(){

}

vector<Mat> Trans::Data_Augmentation(vector<Mat> src,vector<Mat> &result){

    vector<Mat> dst,size_img,rotate_img,bright_img;
    if(circle) dst=circle_detect(src);
    else dst=src;

    switch (func) {      //angle,size,bright

    case 0:
        result=dst;
        break;

    case 1:
        result=angle_trans(dst);
        break;

    case 2:
        result=brightness_trans_1(dst);
        break;

    case 3:
        rotate_img=angle_trans(dst);
        result=brightness_trans_1(rotate_img);
        break;

    case 4:
        result=size_trans(dst);
        break;

    case 5:
        rotate_img=angle_trans(dst);
        result=size_trans(rotate_img);
        break;

    case 6:
        bright_img=brightness_trans_1(dst);
        result=size_trans(bright_img);
        break;

    case 7:
        rotate_img=angle_trans(dst);
        bright_img=brightness_trans_1(rotate_img);
        result=size_trans(bright_img);
        break;

    case 8:
        result=shift_trans(dst);
        break;

    case 9:
        result=merge(dst);
        break;

    case 10:
        result=Mask(dst);
        break;

    case 11:
        result=channl_6_size(dst);
        break;

    case 12:
        bright_img=brightness_trans_1(dst);
        result=channl_6_size(bright_img);
        break;

    case 13:
        result=translation(dst);
        break;
    }

    return result;
}
vector<Mat> Trans::translation(vector<Mat> src){

    vector<Mat>hImgs,translation_image;

       Mat result;

       for(size_t i=0;i<src.size();i++){

           cout<<"数据增强-平移部分:"<<i*100.0 / (src.size() - 1)<<"% "<<endl;

           translation_image.push_back(src[i]);
           Mat input=src[i];
           input =input(Rect(200,0,input.cols-400,input.rows));
           Mat dst;

           for(int j=1;j<translation_num;j++){
               Mat polar_image_1=input(Rect(input.cols-(input.cols*j/translation_num),0,(input.cols*j/translation_num),input.rows));
               Mat polar_image_2=input(Rect(0,0,input.cols-(input.cols*j/translation_num),input.rows));
               hImgs.push_back(polar_image_1);
               hImgs.push_back(polar_image_2);
               hconcat(hImgs, result);
               copyMakeBorder(result,result,0,0,200,200,BORDER_WRAP);

               translation_image.push_back(result.clone());
               hImgs.clear();
            }
       }
       return translation_image;

}

vector<Mat> Trans::channl_6_size(vector<Mat> src){
    vector<Mat> result_img;
    for(size_t i=0;i<src.size();i++){
        cout<<"6通道缩放部分:"<<i*100.0 / (src.size() - 1)<<"% "<<endl;
        cvtColor(src[i],src[i],CV_BGR2GRAY);
        resize(src[i],src[i],Size(640,3732),0,0,INTER_LINEAR);
        result_img.push_back(src[i]);
    }
    return result_img;

}

vector<Mat> Trans::Mask(vector<Mat> src){
    vector<Mat> result_img;
    for(size_t i=0;i<src.size();i++){
        cout<<"防尘盖Mask部分:"<<i*100.0 / (src.size() - 1)<<"% "<<endl;
        cv::Mat mask = cv::Mat::zeros(src[i].rows,src[i].cols,CV_8UC1);
        cv::circle(mask,Point2d(src[i].rows/2,src[i].cols/2),src[i].rows/2-100,Scalar(255),-1);
        Mat re;
        src[i].copyTo(re,mask);
        re = re(Rect(100,100,re.cols-200,re.rows-200));
        result_img.push_back(re);
    }
    return result_img;

}

vector<Mat> Trans::merge(vector<Mat> src){
    vector<Mat> temp_img,result_img;
    Mat result,temp,warp;
    int num=0;
    for(size_t i =0;i<src.size();i++){
        cout<<"图像合并部分:"<<i*100.0 / (src.size() - 1)<<"% "<<endl;
        temp_img.push_back(src[i].clone());
        num++;
        if(num==6){
            vconcat(temp_img,result);
            result_img.push_back(result.clone());
            temp_img.clear();
            num=0;
        }
    }
    return result_img;

}

vector<Mat> Trans::shift_trans(vector<Mat> src){
    vector<Mat> temp_img,result_img;
    Mat result,temp,warp;

    for(size_t i =0;i<src.size();i++){
        cout<<"图像变换部分:"<<i*100.0 / (src.size() - 1)<<"% "<<endl;
        int add_shift=src[i].cols/20;
        int shift_col = src[i].cols/shift_num;
        for(int j=0;j<shift_num;j++){
            temp = src[i](Rect(shift_col*j,0,shift_col,src[i].rows));
            if(j<shift_num-1 && j!=0) copyMakeBorder(temp,warp,0,0,int(add_shift),int(add_shift),BORDER_WRAP);
            else if(j==shift_num-1) copyMakeBorder(temp,warp,0,0,int(add_shift)*2,0,BORDER_WRAP);
            else copyMakeBorder(temp,warp,0,0,0,int(add_shift)*2,BORDER_WRAP);
            temp_img.push_back(warp.clone());
        }
        vconcat(temp_img,result);
        result_img.push_back(result.clone());
        temp_img.clear();
    }
    return result_img;

}

vector<Mat> Trans::size_trans(vector<Mat> size_image){

    int size=1;

    vector<Mat>  size_dst;
    Mat size_img;

    for(size_t i=0;i<size_image.size();i++){

        int Max_pic_size_cols=int(size_image[i].cols*Max_size);
        int Max_pic_size_rows=int(size_image[i].rows*Max_size);
        int Min_pic_size=int(size_image[i].cols*Min_size);

        int step_cols=int((size_image[i].cols*(Max_size-Min_size))/(size_num-0.5));
        int step_rows=int((size_image[i].rows*(Max_size-Min_size))/(size_num-0.5));

        cout<<"数据增强-缩放部分:"<<i*100.0 / (size_image.size() - 1)<<"%"<<endl;

        for(int r_size=Max_pic_size_cols;r_size>Min_pic_size;r_size-=step_cols){

              int prop=Max_pic_size_cols/r_size;
              Max_pic_size_rows=Max_pic_size_rows/prop;

              resize(size_image[i],size_img,Size(r_size,Max_pic_size_rows),0,0,INTER_LINEAR);

              size_dst.push_back(size_img.clone());
              Max_pic_size_rows-=step_rows;
              size+=1;

        }
    }

    return size_dst;
}


Mat Trans::brightness_trans_2(Mat angle_image,Mat brightness_image,float alpha){

    alpha/=10;
    float beta =32;

    for (int row = 0; row < angle_image.rows; row++) {
        for (int col = 0; col < angle_image.cols; col++) {
            if (angle_image.channels() == 3) {
                float b = angle_image.at<Vec3b>(row, col)[0];
                float g = angle_image.at<Vec3b>(row, col)[1];
                float r = angle_image.at<Vec3b>(row, col)[2];

                brightness_image.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(b * alpha + beta);
                brightness_image.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(g * alpha + beta);
                brightness_image.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(r * alpha + beta);

            }

            else if (angle_image.channels() == 1) {
                float v_gray = angle_image.at<uchar>(row, col);
                brightness_image.at<uchar>(row, col) = saturate_cast<uchar>(v_gray * alpha + beta);
            }
        }
    }

    //imwrite(output_circle_filename+"img_"+to_string(brightness)+".bmp",brightness_image);
    return brightness_image;
}


vector<Mat> Trans::brightness_trans_1(vector<Mat> angle_image){

    vector<Mat> brightness_image;

    Mat brightness_img;
    int brightness=0;
    double Alpha=Max_Alpha-Min_Alpha;

      for(size_t i=0;i<angle_image.size();i++){
        cout<<"数据增强-亮度部分:"<<i*100.0 / (angle_image.size() - 1)<<"% "<<endl;
        brightness_img = Mat::zeros(angle_image[i].size(), angle_image[i].type());

        for(float alpha=Min_Alpha;alpha<Max_Alpha;alpha+=float(Alpha/(brightness_num-0.5))){
            brightness_img=brightness_trans_2(angle_image[i],brightness_img,alpha);
            brightness_image.push_back(brightness_img.clone());
            brightness+=1;
        }

      }

        return brightness_image;

}


vector<Mat> Trans::angle_trans(vector<Mat> angle_image){

    vector<Mat>vImgs,rotate_image;

       Mat result,polar_image;

       for(size_t j=0;j<angle_image.size();j++){

           cout<<"数据增强-旋转部分:"<<j*100.0 / (angle_image.size() - 1)<<"% "<<endl;

           Point2d center=Point2d(angle_image[j].cols/2,angle_image[j].rows/2);

           rotate_image.push_back(angle_image[j]);
           Mat src=angle_image[j];
           Mat dst;

           for(int i=1;i<rotate_num;i++){

               warpPolar(src,polar_image, Size(int(src.cols/1),int(src.rows*2*CV_PI)), center, center.x, INTER_LINEAR + WARP_POLAR_LINEAR);
               Mat polar_image_1=polar_image(Rect(0,polar_image.rows-(polar_image.rows*i/rotate_num),polar_image.cols,(polar_image.rows*i/rotate_num)));
               Mat polar_image_2=polar_image(Rect(0,0,polar_image.cols,polar_image.rows-(polar_image.rows*i/rotate_num)));
               vImgs.push_back(polar_image_1);
               vImgs.push_back(polar_image_2);
               vconcat(vImgs, result);

               warpPolar(result, dst, Size(src.rows,src.cols), center, center.x, INTER_LINEAR + WARP_POLAR_LINEAR + WARP_INVERSE_MAP);

               rotate_image.push_back(dst);

               vImgs.clear();
            }
       }

       return polar_image;

}


vector<Mat> Trans::circle_detect(vector<Mat> src){

    vector<Mat> cart_image;
    for(size_t i=0;i<src.size();i++){
        cv::Mat binary_image = src[i] > 160;
        cv::morphologyEx(binary_image, binary_image, cv::MORPH_CLOSE, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        cv::Point2f center = cv::Point2f(0,0);
        float outer_radius = 0;
        removeEdgeNoise(binary_image);
        getOuterCircle(binary_image,center,outer_radius);
        cart_image.push_back(src[i](Rect(center.x-outer_radius/2,center.y-outer_radius/2,center.x+outer_radius/2,center.y+outer_radius/2)));
    }
    return cart_image;
}


void Trans::label_write(String fn,String p_time){

    ifstream infile;
    ofstream outfile;
    string temp;
    for(int k=0;k<picture_num;k++){
      string path_temp = fn;
      string out_file = path_temp.replace(path_temp.find("Pic"),3,"Pic_Trans_"+to_string(k));
      out_file = out_file.replace(out_file.find("input"),5,"trans");
      infile.open(fn, ios::in);
      outfile.open(out_file,ios::app);
      for(int i=0;getline(infile, temp);i++){
        outfile << temp << endl;
      }
      label_num++;
      infile.close();
      outfile.close();
   }
}

void Trans::rotate_label_write(String fn,String p_time,Mat src){

    ifstream infile;
    ofstream outfile;
    string temp;
    int rows=0;

    Point2d center,center_trans;
    vector<Point2d> vec_trans,vec_corner;

    for(int k=0;k<rotate_num;k++){
       for(int bright_size=0;bright_size<brightness_num*size_num;bright_size++){
          string path_temp = fn;
          string out_file = path_temp.replace(path_temp.find("index2"),6,"Trans_"+to_string(k)+"-index2");
          out_file = out_file.replace(out_file.find("input"),5,"trans");
          infile.open(fn, ios::in);
          outfile.open(out_file,ios::app);
          for(int i=0;getline(infile, temp);i++) rows+=1;

          double data[rows][5];
          fstream in(fn);
          cin.rdbuf(in.rdbuf());

           for(int i=0; i<rows;i++){
              for(int j=0;j<5;j++){
                  cin>>data[i][j];
                  if(j==0) continue;
                  else if(j==1 || j==3) data[i][j]*=src.cols;
                  else if(j==2 || j==4) data[i][j]*=src.rows;
              }

              vec_trans.push_back(Point2d(data[i][1],data[i][2]));
              vec_trans.push_back(Point2d(data[i][1]+data[i][3],data[i][2]));
              vec_trans.push_back(Point2d(data[i][1],data[i][2]+data[i][4]));
              vec_trans.push_back(Point2d(data[i][1]+data[i][3],data[i][2]+data[i][4]));

             for(size_t i=0;i<vec_trans.size();i++){
                center.x=vec_trans[i].x;
                center.y=vec_trans[i].y;
                center_trans.x= (center.x - src.cols/2)*cos(CV_PI*2/rotate_num*(k)) - (center.y - src.rows/2)*sin(CV_PI*2/rotate_num*(k)) + src.cols/2 ;
                center_trans.y= (center.x - src.cols/2)*sin(CV_PI*2/rotate_num*(k)) + (center.y - src.rows/2)*cos(CV_PI*2/rotate_num*(k)) + src.rows/2 ;
                vec_corner.push_back(Point2d(center_trans.x,center_trans.y));

             }

             double min_x=1000,max_x=0,min_y=1000,max_y=0;
             for(size_t i=0;i<vec_corner.size();i++){
                 if(vec_corner[i].x>max_x) max_x=vec_corner[i].x;
                 if(vec_corner[i].x<min_x) min_x=vec_corner[i].x;
                 if(vec_corner[i].y>max_y) max_y=vec_corner[i].y;
                 if(vec_corner[i].y<min_y) min_y=vec_corner[i].y;
             }

             double x=vec_corner[0].x/src.cols;
             double y=vec_corner[0].y/src.rows;
             double width=(max_x-min_x)/src.cols;
             double height=(max_y-min_y)/src.rows;
             if(x<0) x=0;
             if(y<0) y=0;

             String result = to_string(int(data[i][0]))+" "+to_string(x)+" "+to_string(y)+" "+to_string(width)+" "+to_string(height);
             outfile << result << endl;

             vec_trans.clear();
             vec_corner.clear();

          }

          label_num++;
          rows=0;
          infile.close();
          outfile.close();
       }

    }

}

void Trans::trans_label_write(String fn,String p_time,Mat src){

    ifstream infile;
    ofstream outfile;
    string temp;
    int rows=0;
    int x_min = 1000;
    int mark_num =0;
    int cols = src.cols-400;
    bool seg =false;
    Point2d center,center_trans;
    vector<Point2d> vec_trans,vec_corner;

    for(int k=0;k<translation_num;k++){
       //for(int bright_size=0;bright_size<brightness_num*size_num;bright_size++){
          string path_temp = fn;
          string out_file = path_temp.replace(path_temp.find("index2"),6,"Trans_"+to_string(k)+"-index2");
          out_file = out_file.replace(out_file.find("input"),5,"trans");
          infile.open(fn, ios::in);
          outfile.open(out_file,ios::app);
          for(int i=0;getline(infile, temp);i++){
              if(k==0){
                    outfile << temp << endl;
              }
            rows+=1;
          }

          double data[rows][5];
          fstream in(fn);
          cin.rdbuf(in.rdbuf());
          bool x_in_200 = false;
          if(k!=0){
              for(int i=0;i<rows;i++){
                  for(int j=0;j<5;j++){
                      cin>>data[i][j];
                      if(j==1 || j==3) data[i][j]*=src.cols;
                   }
                  double x = data[i][1] - data[i][3]/2 - 200;
                  if (x<0){
                     x = data[i][1] - data[i][3]/2;
                     x_in_200 = true;
                  }

                  double width = data[i][3];
                  for(int num=0;num<translation_num;num++){
                      if(x-cols*num/translation_num<x_min && x-cols*num/translation_num>0){
                          x_min = x-cols*num/translation_num;
                          mark_num = num;
                      }
                  }
                  x = x_min+cols*(k+mark_num)/translation_num;
                  double x_1,width_1;
                  double x_temp = x;

                  if (x+width>cols+200 && x+width<src.cols && x<cols){
                      width_1=x+width-cols+200;
                      x_1=0;
                      seg=true;
                  }
                  if (x+width<src.cols && x>cols){
                      width_1=data[i][3];
                      x_1=x - cols;
                      seg=true;
                  }
                  if (x<0 && x+width<800){
                      x_temp = abs(x);
                      width_1=width;
                      x_1=cols-x_temp;
                      x_1 = (x_1+width_1/2+200)/src.cols;
                      seg=true;
                  }
                  if (x>0 && x<400 && x+width<cols/2){
                      if (x+width<400) width_1 = width;
                      else width_1 = 400-x;
                      x_1=src.cols-width_1;
                      x_1 = (x_1+width_1/2+200)/src.cols;
                      seg=true;
                  }
                  x = max(0,int(x));
                  //x = min(int(x),src.cols);
                  if(x+width>src.cols /*&& x>src.cols*/){
                      x = x-src.cols+400;
                  }
                  if(x_in_200) x=x+width/2;
                  else x = x+width/2+200;
                  x = x/src.cols;
                  width/=src.cols;


                  String result = to_string(int(data[i][0]))+" "+to_string(x)+" "+to_string(data[i][2])+" "+to_string(width)+" "+to_string(data[i][4]);
                  outfile << result << endl;
                  if(seg){
                      width_1/=src.cols;
                      String result_1 = to_string(int(data[i][0]))+" "+to_string(x_1)+" "+to_string(data[i][2])+" "+to_string(width_1)+" "+to_string(data[i][4]);
                      outfile << result_1 << endl;
                      seg = false;
                  }
//                  String result_2 = to_string(int(data[i][0]))+" "+to_string(0.5)+" "+to_string(0.5)+" "+to_string(0.9017)+" "+to_string(1);
//                  outfile << result_2 << endl;
//                  String result_3 = to_string(int(data[i][0]))+" "+to_string(0.5)+" "+to_string(0.5)+" "+to_string(0.8033)+" "+to_string(1);
//                  outfile << result_3 << endl;
              }

          }

          label_num++;
          rows=0;
          infile.close();
          outfile.close();
       //}

    }

}

void Trans::shift_label_write(String fn,String p_time,Mat src){

    ifstream infile;
    ofstream outfile;
    string temp;
    int rows=0;
    double temp_dis=0;
    double x=0.0,y=0.0,width=0.0,height=0.0;
    double x_1=0.0,y_1=0.0,width_1=0.0,height_1=0.0;
    double x_corner=0.0,total_width=0.0;

    Point2d center,center_trans;
    vector<Point2d> vec_trans,vec_corner;
    bool seg=false;
    bool add_label=false;

    //for(int bright_size=0;bright_size<brightness_num*size_num;bright_size++){
    string path_temp = fn;
    string out_file = path_temp.replace(path_temp.find("index2"),6,"Trans_"+to_string(0)+"-index2");
    out_file = out_file.replace(out_file.find("input"),5,"shift");
    infile.open(fn, ios::in);
    outfile.open(out_file,ios::app);
    for(int i=0;getline(infile, temp);i++) rows+=1;

    double data[rows][5];
    fstream in(fn);
    cin.rdbuf(in.rdbuf());

    for(int i=0; i<rows;i++){
       for(int j=0;j<5;j++){
           cin>>data[i][j];
           if(j==0) continue;
           else if(j==1 || j==3) data[i][j]*=src.cols;
           else if(j==2 || j==4) data[i][j]*=src.rows;
       }
     if(int(data[i][0])!=shift_only_label && shift_only) continue;

       vec_trans.push_back(Point2d(data[i][1],data[i][2]));
       int shift_cols=src.cols/shift_num;
       int add_shift=src.cols/20;
       for(int j=0;j<shift_num;j++){
           if(vec_trans[0].x-data[i][3]/2>shift_cols*j && vec_trans[0].x-data[i][3]/2<shift_cols*(j+1)){
               if(vec_trans[0].x+data[i][3]/2>shift_cols*(j+1)+add_shift*2){
                   if(j==0){
                       total_width=data[i][3]+add_shift*3;
                       x_corner=vec_trans[0].x-data[i][3]/2;
                       width=shift_cols-x_corner+add_shift*2;
                   }
                   else {
                       if(j==shift_num-2) total_width=data[i][3]+add_shift*3;
                       else total_width=data[i][3]+add_shift*2;
                       x_corner=vec_trans[0].x-shift_cols*j+add_shift-data[i][3]/2;
                       width=shift_cols-x_corner+add_shift*2;
                   }
                   //double x_corner=vec_trans[0].x-data[0][3]/2-shift_cols*j;

                   height=data[i][4];
                   x=x_corner+width/2;
                   y=vec_trans[0].y+src.rows*j;
                   double width_temp=width;

                   width_1=total_width-width_temp;
                   height_1=data[i][4];
                   x_1=width_1/2;
                   y_1=vec_trans[0].y+src.rows*(j+1);


                   shift_cols+=add_shift*2;
                   data[i][3]+=add_shift*2;

                   x=x/(shift_cols);
                   y=y/(src.rows*shift_num);
                   width=width/(shift_cols);
                   height=height/(src.rows*shift_num);

                   x_1=x_1/(shift_cols);
                   y_1=y_1/(src.rows*shift_num);
                   width_1=width_1/(shift_cols);
                   height_1=height_1/(src.rows*shift_num);

                   seg=true;
               }
               else{
                   if(j==0) x=vec_trans[0].x-shift_cols*j;
                   else if(j==shift_num-1) x=vec_trans[0].x-shift_cols*j+add_shift*2;
                   else x=vec_trans[0].x-shift_cols*j+add_shift;
                   y=vec_trans[0].y+src.rows*j;
                   width=data[i][3];
                   height=data[i][4];
                   x_corner=x-data[i][3]/2;
                   bool width_change=false;

                   double x_end=x_corner+data[i][3];
                   if(j==0) {
                       if( x_corner>shift_cols-add_shift){
                           temp_dis = x_corner-(shift_cols-add_shift);
                           y_1=vec_trans[0].y+src.rows*(j+1);
                       }
                       else if(x_corner>0 && x_corner<400){
                           temp_dis = x_corner+(shift_cols-(200-add_shift)*2);
                           y_1=vec_trans[0].y+src.rows*(shift_num-1);
                       }
                       else if(x_end>shift_cols-add_shift){
                           if (x_corner<shift_cols-add_shift*3) temp_dis = 0;   //temp_dis=0;
                           else temp_dis=x_corner-(shift_cols-add_shift*3);
                           y_1=vec_trans[0].y+src.rows*(j+1);
                           width_1=x_end-(shift_cols-add_shift);
                           width_change=true;
                       }
                   }
                   else if(j==1){
                       if( x_corner>shift_cols){
                           temp_dis = x_corner-shift_cols;
                           y_1=vec_trans[0].y+src.rows*(j+1);
                       }
                       else if(x_corner>0 && x_corner<add_shift*3){
                           temp_dis = x_corner+(shift_cols-add_shift);
                           y_1=vec_trans[0].y+src.rows*(j-1);
                       }
                       else if(x_end>shift_cols){
                           if (x_corner<shift_cols-add_shift) temp_dis = 0;
                           else temp_dis=x_corner-(shift_cols-add_shift);
                           y_1=vec_trans[0].y+src.rows*(j+1);
                           width_1=x_end-shift_cols;
                           width_change=true;
                       }
                   }
                   else if(j==2){
                       if( x_corner>shift_cols-add_shift){
                           temp_dis = x_corner-(shift_cols-add_shift);
                           y_1=vec_trans[0].y+src.rows*(j+1);
                       }
                       else if(x_corner>0 && x_corner<add_shift*2){
                           temp_dis = x_corner+shift_cols;
                           y_1=vec_trans[0].y+src.rows*(j-1);
                       }
                       else if(x_end>shift_cols-add_shift){
                           if (x_corner<shift_cols-add_shift) temp_dis = 0;
                           else temp_dis=x_corner-(shift_cols-add_shift);
                           y_1=vec_trans[0].y+src.rows*(j+1);
                           width_1=x_end-(shift_cols-add_shift);
                           width_change=true;
                       }
                   }
                   else if(j==3){
                       if( x_corner>shift_cols-(200-add_shift)*2){
                           temp_dis = x_corner-(shift_cols-(200-add_shift)*2);
                           y_1=vec_trans[0].y;
                       }
                       else if(x_corner>0 && x_corner<add_shift*3){
                           temp_dis = x_corner+shift_cols-add_shift+data[i][3];
                           y_1=vec_trans[0].y+src.rows*(j-1);
                       }
                       else if(x_end>shift_cols-(200-add_shift)*2){
                           if (x_corner<shift_cols-400)temp_dis = 0;
                           else temp_dis=x_corner-(shift_cols-400);
                           y_1=vec_trans[0].y;
                           width_1=x_end-(shift_cols-(200-add_shift)*2);
                           width_change=true;
                       }
                   }
                   if(!width_change)x_1=temp_dis+data[i][3]/2;
                   else x_1=temp_dis+width_1/2;

                   if(y_1!=0.0) add_label=true;
                   else add_label=false;

                   shift_cols+=int(add_shift)*2;
                   x=x/shift_cols;
                   y=y/(src.rows*shift_num);
                   width=width/shift_cols;
                   height=height/(src.rows*shift_num);

                   //shift_cols-=int(add_shift)*2;
                   if(!width_change) width_1=data[i][3];
                   height_1=data[i][4];
                   if(width_1<40) add_label=false;
                   if(add_label){
                       x_1=std::min(int(x_1),shift_cols);
                       width_1=std::min(int(x_1+width_1/2),shift_cols)-(x_1-width_1/2);
                       x_1=x_1/shift_cols;
                       y_1=y_1/(src.rows*shift_num);
                       width_1=width_1/shift_cols;
                       height_1=height_1/(src.rows*shift_num);

                   }
                   seg=false;
               }

           }

       }
       if(seg){
           String result = to_string(int(data[i][0]))+" "+to_string(x)+" "+to_string(y)+" "+to_string(width)+" "+to_string(height);
           String result_1 = to_string(int(data[i][0]))+" "+to_string(x_1)+" "+to_string(y_1)+" "+to_string(width_1)+" "+to_string(height_1);
           outfile << result << endl;
           outfile << result_1 << endl;
       }
       else{
          String result = to_string(int(data[i][0]))+" "+to_string(x)+" "+to_string(y)+" "+to_string(width)+" "+to_string(height);
          String result_1 = to_string(7)+" "+to_string(x_1)+" "+to_string(y_1)+" "+to_string(width_1)+" "+to_string(height_1);
          outfile << result << endl;
          if(add_label) outfile<<result_1<<endl;
       }
//       String result_2 = to_string(int(data[i][0]))+" "+to_string(0.5)+" "+to_string(0.5)+" "+to_string(0.9)+" "+to_string(1);
//       outfile << result_2 << endl;
//       String result_3 = to_string(int(data[i][0]))+" "+to_string(0.5)+" "+to_string(0.5)+" "+to_string(0.8)+" "+to_string(1);
//       outfile << result_3 << endl;

       vec_trans.clear();
       vec_corner.clear();

    }


    label_num++;
    rows=0;
    infile.close();
    outfile.close();
 //}

}

void Trans::getOuterCircle(cv::Mat binary_img, cv::Point2f &center, float &radius)
{
    vector<vector<cv::Point>> contours;
    cv::findContours(binary_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//    cv::Mat img_bgr;
//    cv::cvtColor(binary_img, img_bgr, cv::COLOR_GRAY2BGR);
//    drawContours(img_bgr, contours, -1, cv::Scalar(0, 255, 0));
    if(contours.empty())
        return;
    size_t outer_index = 0;
    double max_size = cv::contourArea(contours[0]);
    for(size_t i = 1; i < contours.size(); i++)
    {
        double temparea = cv::contourArea(contours[i]);
        if( temparea > max_size)
        {
            outer_index = i;
            max_size = temparea;
        }
    }
    fitCircle(contours[outer_index], center, radius);
    return;
}

bool Trans::fitCircle(const vector<cv::Point> &points, cv::Point2f &center, float &radius)
{
    radius = 0.0f;
    float sum_x = 0.0f, sum_y = 0.0f;
    float sum_x2 = 0.0f, sum_y2 = 0.0f;
    float sum_x3 = 0.0f, sum_y3 = 0.0f;
    float sum_xy = 0.0f, sum_x1y2 = 0.0f, sum_x2y1 = 0.0f;
    unsigned int N = static_cast<unsigned int>(points.size());
    for (unsigned int i = 0; i < N; i++)
    {
        float x = points[i].x;
        float y = points[i].y;
        float x2 = x*x;
        float y2 = y*y;
        sum_x += x;
        sum_y += y;
        sum_x2 += x2;
        sum_y2 += y2;
        sum_x3 += x2*x;
        sum_y3 += y2*y;
        sum_xy += x*y;
        sum_x1y2 += x*y2;
        sum_x2y1 += x2*y;
    }
    float C, D, E, G, H;
    float a, b, c;
    C = N*sum_x2 - sum_x*sum_x;
    D = N*sum_xy - sum_x*sum_y;
    E = N*sum_x3 + N*sum_x1y2 - (sum_x2 + sum_y2)*sum_x;
    G = N*sum_y2 - sum_y*sum_y;
    H = N*sum_x2y1 + N*sum_y3 - (sum_x2 + sum_y2)*sum_y;
    a = (H*D - E*G) / (C*G - D*D);
    b = (H*C - E*D) / (D*D - G*C);
    c = -(a*sum_x + b*sum_y + sum_x2 + sum_y2) / N;
    center.x = static_cast<int>(a / (-2));
    center.y = static_cast<int>(b / (-2));
    if(a*a + b*b - 4 * c <= 0)
    {
        return false;
    }
    radius = sqrt(a*a + b*b - 4 * c) / 2;
    return true;
}

void Trans::removeEdgeNoise(cv::Mat &image)
{
    cv::Mat labels, stats, centeroids;
    int nccomps = cv::connectedComponentsWithStats(image, labels, stats, centeroids);

    for (int num = 1; num < nccomps; num++)
    {
        if (stats.at<int>(num, cv::CC_STAT_LEFT) == 0||stats.at<int>(num,cv::CC_STAT_TOP) == 0
                ||stats.at<int>(num,cv::CC_STAT_TOP)+stats.at<int>(num,cv::CC_STAT_HEIGHT)==image.rows||stats.at<int>(num,cv::CC_STAT_LEFT)+stats.at<int>(num,cv::CC_STAT_WIDTH)==image.cols)
        {
            int x = stats.at<int>(num, cv::CC_STAT_LEFT);
            int y = stats.at<int>(num, cv::CC_STAT_TOP);
            int w = stats.at<int>(num, cv::CC_STAT_WIDTH);
            int h = stats.at<int>(num, cv::CC_STAT_HEIGHT);

            cv::Rect rect(x, y, w, h);
            cv::Mat temp_image = image(rect);
            cv::Mat temp_labels = labels(rect);

            cv::parallel_for_(cv::Range(0, temp_image.rows * temp_image.cols), [&](const cv::Range& range){
                for (int r = range.start; r < range.end; r++)
                {
                    int i = r / temp_image.cols;
                    int j = r % temp_image.cols;
                    if (temp_labels.ptr<int>(i)[j] == num)
                    {
                        temp_image.at<uchar>(i, j) = 0;
                    }
                }
            });
        }
        else
        {
        }
    }

}







