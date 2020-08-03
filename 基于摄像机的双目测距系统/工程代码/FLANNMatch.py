import numpy as np
import cv2
import os

templates = "./data/templates"


def find_matches(frame) -> ():
    """
    遍历templates文件夹，将所有的模板图片与摄像头第一帧作对比，如果找到匹配点则返回，否则返回空值
    :param frame: 摄像头图像
    :return:
    """
    pics = os.listdir(templates)
    for pic in pics:
        result = FLANN_match(frame, cv2.imread(os.path.join(templates, pic)))
        if result.size != 0:
            bbox = (int(result[0][0][0]), int(result[0][0][1]), int(result[2][0][0] - result[0][0][0]),
                    int(result[2][0][1] - result[0][0][1]))
            return bbox
    return ()


def FLANN_match(target, template, min_match_count=10):
    """
    基于FLANN的匹配器(FLANN based Matcher)定位图片
    https://blog.csdn.net/zhuisui_woxin/article/details/84400439
    :param target: 目标图片
    :param template: 模板图片
    :param min_match_count: 最低特征点匹配数量为10
    :return: 目标检测框坐标
    """
    # Initiate SIFT detector创建sift检测器
    sift = cv2.xfeatures2d.SIFT_create()

    # find the keypoints and descriptors with SIFT
    kp2, des2 = sift.detectAndCompute(target, None)
    kp1, des1 = sift.detectAndCompute(template, None)
    # 创建设置FLANN匹配
    FLANN_INDEX_KDTREE = 0
    index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)
    search_params = dict(checks=50)
    flann = cv2.FlannBasedMatcher(index_params, search_params)
    matches = flann.knnMatch(des1, des2, k=2)
    # store all the good matches as per Lowe's ratio test.
    good = []
    # 舍弃大于0.7的匹配
    for m, n in matches:
        if m.distance < 0.7 * n.distance:
            good.append(m)
    if len(good) > min_match_count:
        # 获取关键点的坐标
        src_pts = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1, 1, 2)
        dst_pts = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1, 1, 2)
        # 计算变换矩阵和MASK
        M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
        h, w = template.shape[0], template.shape[1]
        # 使用得到的变换矩阵对原图像的四个角进行变换，获得在目标图像上对应的坐标
        pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
        dst = cv2.perspectiveTransform(pts, M)
        # cv2.polylines(target, [np.int32(dst)], True, 0, 2, cv2.LINE_AA)
        # cv2.imshow("1",target)
        # cv2.waitKey(0)
        return dst
    else:
        print("Not enough matches are found - %d/%d" % (len(good), min_match_count))
        return np.array([])
