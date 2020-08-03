import cv2
import numpy as np

left_camera_matrix = np.array([[1067.03623, 0., 368.49844],
                               [0., 1057.15582, 321.48917],
                               [0., 0., 1.]])
left_distortion = np.array([[-0.45072, -0.66407, -0.01660, -0.00068, 0.00000]])

right_camera_matrix = np.array([[1067.75963, 0., 379.90827],
                                [0., 1065.91723, 269.39259],
                                [0., 0., 1.]])
right_distortion = np.array([[-0.50619, 0.43593, -0.00656, -0.00792, 0.00000]])

om = np.array([-0.04121, -0.00376, 0.00283])  # 旋转关系向量
R = cv2.Rodrigues(om)[0]  # 使用Rodrigues变换将om变换为R
T = np.array([-50.41560, 0.84743, 14.72770])  # 平移关系向量

size = (640, 480)  # 图像尺寸

# 进行立体更正
R1, R2, P1, P2, Q, validPixROI1, validPixROI2 = cv2.stereoRectify(left_camera_matrix, left_distortion,
                                                                  right_camera_matrix, right_distortion, size, R, T)
# 计算更正map
left_map1, left_map2 = cv2.initUndistortRectifyMap(left_camera_matrix, left_distortion, R1, P1, size, cv2.CV_16SC2)
right_map1, right_map2 = cv2.initUndistortRectifyMap(right_camera_matrix, right_distortion, R2, P2, size, cv2.CV_16SC2)
