#!/usr/bin/python
# -*- coding:utf-8 -*-
#coding :UTF-8

import os
import shutil
import random
import math
import random

'''
# 自动拆分误判数据到训练集与验证集
'''
def WrongDetectDataSplit(src_path,out_path):
    path = out_path
    images_path = os.path.join(path,'images')
    train_dst = os.path.join(images_path,'train')
    val_dst = os.path.join(images_path,'val')
    if os.path.exists(path):
        shutil.rmtree(path)
    filelist = os.listdir(src_path)
    val_len = (len(filelist)) // 10
    os.makedirs(train_dst)
    os.makedirs(val_dst)
    print(val_len)
    val_list = []
    while val_len > 0:
        file = random.choice(filelist)
        src_name = os.path.join(src_path, file)
        dst_name = os.path.join(val_dst, file)
        shutil.copyfile(src_name, dst_name)
        val_len -= 1
        val_list.append(file)
    print('finish val')
    for file in filelist:
        if file not in val_list:
            src_name = os.path.join(src_path, file)
            dst_name = os.path.join(train_dst, file)
            shutil.copyfile(src_name, dst_name)

'''
修改label标签
'''
def ModifyLabels(path):
    filelist = os.listdir(path)
    for file in filelist:
        filename = os.path.join(path, file)
        if not filename.endswith('txt'):
            continue
        flag = False
        with open(filename, 'r', encoding="utf-8") as f:
            data = []
            for idx, line in enumerate(f):
                temp = list(line)
                if temp[0] == '3':
                    temp[0] = '2'
                    flag = True
                temp_str = ''.join(temp)
                data.append(temp_str)
        if not flag:
            continue
        with open(filename, 'w', encoding="utf-8") as f:
            for temp in data:
                f.write(temp)

'''
# 带标签数据分成两类
'''
def NornamlDataSplit(src_path,out_path,img_type='.bmp'):
    # dst_path = os.path.join(src_path,'split')
    dst_path = out_path
    if os.path.exists(dst_path):
        shutil.rmtree(dst_path)
    img_train_dst_path = os.path.join(dst_path,'images','train')
    label_train_dst_path = os.path.join(dst_path,'labels','train')
    img_val_dst_path = os.path.join(dst_path,'images','val')
    label_val_dst_path = os.path.join(dst_path,'labels','val')
    os.makedirs(img_train_dst_path)
    os.makedirs(label_train_dst_path)
    os.makedirs(img_val_dst_path)
    os.makedirs(label_val_dst_path)

    # img_src_path = os.path.join(src_path,'image')
    # label_src_path = os.path.join(src_path,'labels')
    img_src_path = src_path
    label_src_path = src_path
    img_names = os.listdir(img_src_path)
    img_names = [img_name for img_name in img_names if img_type in img_name]
    random.shuffle(img_names)
    val_len = len(img_names) // 10
    for i in range(0,val_len):
        img_name = img_names[i]
        label_name = img_name.replace(img_type,'.txt')
        shutil.copy(os.path.join(img_src_path,img_name),os.path.join(img_val_dst_path,img_name))
        if os.path.exists(os.path.join(label_src_path,label_name)):
            shutil.copy(os.path.join(label_src_path,label_name),os.path.join(label_val_dst_path,label_name))

    for i in range(val_len,len(img_names)):
        img_name = img_names[i]
        label_name = img_name.replace(img_type,'.txt')
        shutil.copy(os.path.join(img_src_path,img_name),os.path.join(img_train_dst_path,img_name))
        if os.path.exists(os.path.join(label_src_path,label_name)):
            shutil.copy(os.path.join(label_src_path,label_name),os.path.join(label_train_dst_path,label_name))


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(
        description=__doc__)
    parser.add_argument('--path',default='chejiagong_endplane')
    parser.add_argument('--func',default="0")
    args = parser.parse_args()
    # # 误判数据自动拆分训练集与验证集
    src_path = '/home/luoziyi/Trans/data/chejiagong_endplane/trans/'
    out_path = '/home/luoziyi/Trans/transport/chejiagong_endplane/'
    src_path = src_path.replace('chejiagong_endplane', args.path)
    out_path = out_path.replace('chejiagong_endplane', args.path)
    # WrongDetectDataSplit(src_path)
    # fangchengai_trans,203_outplane,mifengquan_trans,ccwq_out,203_innerplane,chejiagong_innerplane

    #带标签拆分数据集
    if args.func=="0":
       NornamlDataSplit(src_path,out_path,img_type='.bmp')
    else:
       WrongDetectDataSplit(src_path,out_path)
    #ModifyLabels(src_path)


