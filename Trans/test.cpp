#include<time.h>
#include<Trans.h>


int main(){

    Trans trans;

    vector<Mat>input,result;
    vector<cv::String> fn,img_fn;

    Mat dst;
    string p_time,p_temp;
    bool judge = false;
    size_t label_num=0;
    glob(trans.input_filename, fn, false);
    size_t count = fn.size();
    string temp_path = trans.input_filename;
    if(trans.func==8) temp_path = temp_path.replace(temp_path.find("input"),5,"shift");
    else temp_path = temp_path.replace(temp_path.find("input"),5,"trans");

    if(trans.func==8){
        string rm_file = "rm -f "+temp_path+"*";
        system(rm_file.c_str());
    }
    else{
        string rm_file = "rm -f "+temp_path+"*";
        system(rm_file.c_str());
    }

    cout<<"读取数据..."<<endl;
    for (size_t i = 0; i < count; i++){
      dst=imread(fn[i]);

      //图片读取
      if(!dst.empty() && (judge==false || trans.circle==true || trans.misjudgment==true)){
         img_fn.push_back(fn[i]);
         if(trans.circle==false && trans.misjudgment==false && i==count-1) break;
         input.emplace_back(imread(fn[i]));
         label_num+=1;
         judge=true;
      }

      //生成标签文件
      else if(dst.empty() && judge==true){
          cout<<"标签生成中:"<<i*100.0 / (count - 1)<<"%"<<endl;
          string label_path;
          if(trans.func==0 ||trans.func==2 || trans.func==4 || trans.func==6 || trans.func==12)
              trans.label_write(fn[i],p_time);
          else if(trans.func==1 ||trans.func==3 || trans.func==5 || trans.func==7)
              trans.rotate_label_write(fn[i],p_time,input[label_num]);
          else if(trans.func==8) trans.shift_label_write(fn[i],p_time,input[label_num]);
          else if(trans.func==13) trans.trans_label_write(fn[i],p_time,input[label_num]);

          judge=false;
      }
      else if(!dst.empty() && judge==true){
          input.erase(input.end()-1);
          input.emplace_back(dst);
      }
      label_num=0;

    }

    p_temp=p_time;
    //图片载入
    trans.Data_Augmentation(input,result);

    if(trans.add_src && trans.func!=8) {
        string cp_file;
        if (trans.func==8){
            cp_file = "cp "+trans.input_filename+"* "+temp_path;
        }
        else  {
            cp_file = "cp "+trans.input_filename+"* "+temp_path;
        }
        system(cp_file.c_str());
    }

    if(trans.out){
       size_t num = 0;
       for(size_t i=0;i<img_fn.size();i++){
        cout<<"图像生成中:"<<i*100.0 / (result.size() - 1)<<"%"<<endl;
        for(int j = 0; j<trans.picture_num;j++){
            string temp = img_fn[i];
            string out_file = temp.replace(temp.find("Pic"),3,"Pic_Trans_"+to_string(j));
            if(trans.func==8) {
                out_file = out_file.replace(out_file.find("input"),5,"shift");
                imwrite(out_file,result[num]);
            }
            else
            {
                out_file = out_file.replace(out_file.find("input"),5,"trans");
                imwrite(out_file,result[num]);
            }
            num++;
        }
       }
    }

    cout<<"数据增强完成!"<<endl;
    size_t total_num=0;
    if(trans.misjudgment) total_num=count;
    else total_num=count/2;
     cout<<"原图"<<total_num<<"张"<<endl;
    if(trans.add_src && trans.func!=8){
        cout<<"生成图片"<<result.size()<<"张,加上原图共"<<result.size()+total_num<<"张"<<endl;
    }
    else{
        cout<<"生成图片"<<result.size()<<"张"<<endl;
    }
    string rm_cl_file = "rm -f "+temp_path+"/classes.txt";
    system(rm_cl_file.c_str());

    int func = 0;
    if(trans.misjudgment==true) func=1;
    string data_split = "python "+trans.python_path+"data_split.py --path "+trans.project_filename+" --func "+ to_string(func);
    cout<<data_split<<endl;
    system(data_split.c_str());


   waitKey(0);
   return 0;

}
