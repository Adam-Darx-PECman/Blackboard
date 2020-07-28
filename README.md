# Blackboard

#### 介绍
一定程度上简化费曼图的绘制与计算

#### 软件架构
软件架构说明
采用Qt(C++)开发，主要分为两个模块:PaintArea和ComputeModel.自动生成对应的Mathematica代码(需要安装包FeynCalc)

#### 安装教程

1.  将代码文件放到一个文件夹下，使用Qt Creator打开.pro文件

#### 使用说明

1.  目前版本(1.0.0)默认时间方向水平从左到右
2.  务必保证每一步都画对，顶角处采用"磁贴"设计，自动吸附15像素半径内线的端点.须保证没有多余/错置顶点.
3.  生成的Mathematica代码需要包FeynCalc,详见[FeynCalc](https://feyncalc.github.io/)

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx / Fix_xxx 分支
3.  提交代码
4.  新建 Pull Request
