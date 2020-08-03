import cv2
import os
import sys
import time
import numpy as np
import FLANNMatch
from cv2.xfeatures2d import matchGMS
import camera_config
from enum import Enum
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import QTimer, Qt
from PyQt5.QtGui import QIcon, QPixmap, QImage
from PyQt5.QtWidgets import QApplication, QMainWindow, QInputDialog, QMessageBox, QLineEdit,QFileDialog

folder_prefix = "data"
folder_path = os.path.join(folder_prefix,'static_measure')
folder_path_dynamic = os.path.join(folder_prefix,'dynamic_measure')
folder_name = ""
folder_name_dynamic = ""
window_width = 800  # 窗口宽
window_height = 600  # 窗口高
img_height = 480
img_width = 640


# write simple callback function to pass trackbar position as *arg
def callback(x):
    pass


# 主页窗口
class UiMainBox(object):
    def __init__(self, measure_box):
        measure_box.setObjectName("MeasureBox")
        measure_box.resize(window_width, window_height)
        self.centralwidget = QtWidgets.QWidget(measure_box)

        self.centralwidget.setObjectName("centralwidget")
        self.home_pic_label = QtWidgets.QLabel(self.centralwidget)
        self.home_pic_label.setGeometry(QtCore.QRect(-40, 0, 841, 361))
        self.home_pic_label.setText("")
        self.home_pic_label.setPixmap(QtGui.QPixmap("home_pic.jpg"))
        self.home_pic_label.setObjectName("label")
        self.create_task_Button = QtWidgets.QPushButton(self.centralwidget)
        self.create_task_Button.setGeometry(QtCore.QRect(300, 400, 251, 71))

        font = QtGui.QFont()
        font.setFamily("Adobe Arabic")
        font.setPointSize(14)
        self.create_task_Button.setFont(font)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("new_task.jpg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.create_task_Button.setIcon(icon)
        self.create_task_Button.setObjectName("pushButton")

        measure_box.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(measure_box)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 23))
        self.menubar.setObjectName("menubar")
        self.menuNew = QtWidgets.QMenu(self.menubar)
        self.menuNew.setObjectName("menuNew")
        self.menuHelp = QtWidgets.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        measure_box.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(measure_box)
        self.statusbar.setObjectName("statusbar")
        measure_box.setStatusBar(self.statusbar)
        self.actionMeasure_Task = QtWidgets.QAction(measure_box)
        self.actionMeasure_Task.setObjectName("actionMeasure_Task")
        self.actionView_Data = QtWidgets.QAction(measure_box)
        self.actionView_Data.setObjectName("actionView_Data")
        self.actionExit = QtWidgets.QAction(measure_box)
        self.actionExit.setObjectName("actionExit")
        self.actionVersion = QtWidgets.QAction(measure_box)
        self.actionVersion.setObjectName("actionVersion")
        self.actionDocuments = QtWidgets.QAction(measure_box)
        self.actionDocuments.setObjectName("actionDocuments")

        self.menuNew.addAction(self.actionMeasure_Task)
        self.menuNew.addAction(self.actionView_Data)
        self.menuNew.addAction(self.actionExit)
        self.menuHelp.addAction(self.actionVersion)
        self.menuHelp.addAction(self.actionDocuments)
        self.menubar.addAction(self.menuNew.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.create_task_Button.clicked.connect(self.hide_home_show_measure)
        self.actionMeasure_Task.triggered.connect(self.hide_home_show_measure)
        self.actionExit.triggered.connect(QApplication.quit)
        self.actionVersion.triggered.connect(self.hide_home_show_version)
        self.actionView_Data.triggered.connect(self.hide_home_show_view)

        self.retranslateUi(measure_box)
        QtCore.QMetaObject.connectSlotsByName(measure_box)

    def hide_home_show_measure(self):
        clear_measure_window()
        show_hide_window(MainWindow,MeasureUiWindow)

    def hide_home_show_view(self):
        view_data()
        show_hide_window(MainWindow,ViewWindow)

    def hide_home_show_version(self):
        show_hide_window(MainWindow, VersionWindow)

    def retranslateUi(self, MeasureBox):
        _translate = QtCore.QCoreApplication.translate
        MeasureBox.setWindowTitle(_translate("MeasureBox", "MainWindow"))
        self.create_task_Button.setText(_translate("MeasureBox", "Create New Task"))
        self.menuNew.setTitle(_translate("MeasureBox", "New"))
        self.menuHelp.setTitle(_translate("MeasureBox", "Help"))
        self.actionMeasure_Task.setText(_translate("MeasureBox", "Measure Task"))
        self.actionView_Data.setText(_translate("MeasureBox", "View Data"))
        self.actionExit.setText(_translate("MeasureBox", "Exit"))
        self.actionVersion.setText(_translate("MeasureBox", "Version"))
        self.actionDocuments.setText(_translate("MeasureBox", "Documents"))


class UiMeasureWindow(object):
    def __init__(self, Measure_Window):
        self.camera_opened = False

        Measure_Window.setObjectName("Measure_Window")
        Measure_Window.resize(window_width, window_height)
        self.centralwidget = QtWidgets.QWidget(Measure_Window)
        self.centralwidget.setObjectName("centralwidget")
        self.frame = QtWidgets.QFrame(self.centralwidget)
        self.frame.setGeometry(QtCore.QRect(10, 0, 781, 571))
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.textBrowser_4 = QtWidgets.QTextBrowser(self.frame)
        self.textBrowser_4.setGeometry(QtCore.QRect(530, 260, 241, 151))
        self.textBrowser_4.setObjectName("textBrowser_4")

        self.button_save_pic = QtWidgets.QPushButton(self.frame)
        self.button_save_pic.setGeometry(QtCore.QRect(400, 210, 111, 41))
        font = QtGui.QFont()
        font.setFamily("Adobe Arabic")
        font.setPointSize(10)
        font.setBold(True)
        font.setWeight(75)
        self.button_save_pic.setFont(font)
        self.button_save_pic.setObjectName("pushButton_9")

        self.label_2 = QtWidgets.QLabel(self.frame)
        self.label_2.setGeometry(QtCore.QRect(70, 0, 301, 41))
        font = QtGui.QFont()
        font.setFamily("Adobe Arabic")
        font.setPointSize(10)
        font.setBold(True)
        font.setWeight(75)
        self.label_2.setFont(font)
        self.label_2.setObjectName("label_2")

        self.button_open_camera = QtWidgets.QPushButton(self.frame)
        self.button_open_camera.setGeometry(QtCore.QRect(400, 50, 111, 41))
        font = QtGui.QFont()
        font.setFamily("Adobe Arabic")
        font.setPointSize(10)
        font.setBold(True)
        font.setWeight(75)
        self.button_open_camera.setFont(font)
        self.button_open_camera.setObjectName("pushButton_10")

        self.button_hide_measure_show_home = QtWidgets.QPushButton(self.frame)
        self.button_hide_measure_show_home.setGeometry(QtCore.QRect(600, 440, 111, 41))

        font = QtGui.QFont()
        font.setFamily("Adobe Arabic")
        font.setPointSize(10)
        font.setBold(True)
        font.setWeight(75)
        self.button_hide_measure_show_home.setFont(font)
        self.button_hide_measure_show_home.setObjectName("pushButton_11")

        self.button_take_pic = QtWidgets.QPushButton(self.frame)
        self.button_take_pic.setGeometry(QtCore.QRect(400, 130, 111, 41))
        font = QtGui.QFont()
        font.setFamily("Adobe Arabic")
        font.setPointSize(10)
        font.setBold(True)
        font.setWeight(75)
        self.button_take_pic.setFont(font)
        self.button_take_pic.setObjectName("pushButton_13")

        self.button_compute_distance = QtWidgets.QPushButton(self.frame)
        self.button_compute_distance.setGeometry(QtCore.QRect(400, 290, 111, 41))
        font = QtGui.QFont()
        font.setFamily("Adobe Arabic")
        font.setPointSize(8)
        font.setBold(True)
        font.setWeight(75)
        self.button_compute_distance.setFont(font)
        self.button_compute_distance.setObjectName("button_compute_distance")

        self.label = QtWidgets.QLabel(self.frame)
        self.label.setGeometry(QtCore.QRect(110, 230, 54, 12))
        self.label.setText("")
        self.label.setObjectName("label")

        self.left_cam_label = QtWidgets.QLabel(self.frame)
        self.left_cam_label.setGeometry(QtCore.QRect(30, 50, 351, 211))
        self.left_cam_label.setText("")
        self.left_cam_label.setObjectName("left_cam_label")
        self.right_cam_label = QtWidgets.QLabel(self.frame)
        self.right_cam_label.setGeometry(QtCore.QRect(30, 260, 351, 231))
        self.right_cam_label.setText("")
        self.right_cam_label.setObjectName("right_cam_label")

        self.label_5 = QtWidgets.QLabel(self.frame)
        self.label_5.setGeometry(QtCore.QRect(550, 50, 211, 191))
        self.label_5.setText("")
        self.label_5.setObjectName("label_5")

        self.button_auto_measure = QtWidgets.QPushButton(self.frame)
        self.button_auto_measure.setGeometry(QtCore.QRect(130, 500, 161, 41))
        font = QtGui.QFont()
        font.setFamily("Adobe Arabic")
        font.setPointSize(10)
        font.setBold(True)
        font.setWeight(75)
        self.button_auto_measure.setFont(font)
        self.button_auto_measure.setObjectName("button_auto_measure")

        Measure_Window.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(Measure_Window)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 23))
        self.menubar.setObjectName("menubar")
        self.menuNew = QtWidgets.QMenu(self.menubar)
        self.menuNew.setObjectName("menuNew")
        self.menuHelp = QtWidgets.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        Measure_Window.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(Measure_Window)
        self.statusbar.setObjectName("statusbar")
        Measure_Window.setStatusBar(self.statusbar)
        self.actionMeasure_Task = QtWidgets.QAction(Measure_Window)
        self.actionMeasure_Task.setObjectName("actionMeasure_Task")
        self.actionView_Data = QtWidgets.QAction(Measure_Window)
        self.actionView_Data.setObjectName("actionView_Data")
        self.actionExit = QtWidgets.QAction(Measure_Window)
        self.actionExit.setObjectName("actionExit")
        self.actionVersion = QtWidgets.QAction(Measure_Window)
        self.actionVersion.setObjectName("actionVersion")
        self.actionDocuments = QtWidgets.QAction(Measure_Window)
        self.actionDocuments.setObjectName("actionDocuments")
        self.menuNew.addAction(self.actionMeasure_Task)
        self.menuNew.addAction(self.actionView_Data)
        self.menuNew.addAction(self.actionExit)
        self.menuHelp.addAction(self.actionVersion)
        self.menuHelp.addAction(self.actionDocuments)
        self.menubar.addAction(self.menuNew.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.button_hide_measure_show_home.clicked.connect(self.hide_measure_show_home)
        self.button_open_camera.clicked.connect(self.open_camera)
        self.button_take_pic.clicked.connect(self.take_pic)
        self.button_compute_distance.clicked.connect(self.compute_distance)
        self.button_auto_measure.clicked.connect(self.auto_measure)
        self.actionMeasure_Task.triggered.connect(self.hide_measure_show_home)
        self.actionView_Data.triggered.connect(self.hide_measure_show_view)
        self.actionExit.triggered.connect(self.quit_application)
        self.actionVersion.triggered.connect(self.hide_measure_show_version)

        self.retranslate_ui(Measure_Window)
        QtCore.QMetaObject.connectSlotsByName(Measure_Window)

    def quit_application(self):
        self.close_camera()
        QApplication.quit()

    def hide_measure_show_home(self):
        show_hide_window(MeasureUiWindow, MainWindow)
        cv2.destroyAllWindows()

    def hide_measure_show_view(self):
        view_data()
        show_hide_window(MeasureUiWindow, ViewWindow)

    def hide_measure_show_version(self):
        show_hide_window(MeasureUiWindow, VersionWindow)

    def open_camera(self):
        if not self.camera_opened:
            self.camera_opened = True
            cam_left.label = measure_ui.left_cam_label
            cam_left.timer.start(200)
            cam_right.label = measure_ui.right_cam_label
            cam_right.timer.start(200)

    def close_camera(self):
        if self.camera_opened:
            cam_left.stop_camera()
            cam_right.stop_camera()
            cam_left.label.clear()
            cam_right.label.clear()
            self.camera_opened = False

    def take_pic(self):
        """
        需要同时拍两张照
        """
        # 如果没有打开摄像头就take picture
        self.open_camera()
        create_folder_dynamic_static(False)
        cam_left.get_pic(os.path.join(folder_path, folder_name, "left.jpg"))
        cam_right.get_pic(os.path.join(folder_path, folder_name, "right.jpg"))

    def auto_measure(self):
        self.open_camera()
        create_folder_dynamic_static(True)

        tracker_type = "KCF"
        tracker = cv2.TrackerKCF_create()
        win_name = 'Tracking'

        left_ok, frame = cam_left.cam.read()
        bbox: tuple = FLANNMatch.find_matches(frame)
        # Uncomment the line below to select a different bounding box
        if bbox == ():  # 如果返回空值就手动选择匹配框
            QMessageBox.critical(None, '请选择想要测量的对象',
                                 "自动查找测量对象失败，请手动选择，按空格键结束选择")
            bbox = cv2.selectROI(win_name, frame, False)
        # Initialize tracker with first frame and bounding box
        tracker.init(frame, bbox)

        cv2.namedWindow(win_name)
        temp_dist = 0
        while True:
            # Read a new frame
            left_ok, frame = cam_left.cam.read()

            if not left_ok:
                print('Cannot read left video file')
                break
            find_box, bbox = tracker.update(frame)
            if find_box:
                right_ok, right_frame = cam_right.cam.read()
                if not right_ok:
                    print("Cannot read right video file")
                    break

                # 根据打开的图像和相机参数更正 map 对图片进行重构
                img_left = cv2.remap(frame, camera_config.left_map1, camera_config.left_map2, cv2.INTER_LINEAR)
                img_right = cv2.remap(right_frame, camera_config.right_map1, camera_config.right_map2, cv2.INTER_LINEAR)

                # 显示极线矫正后的图片
                # cv2.imshow("hstack", np.hstack([img_left, img_right]))

                x_start = int(bbox[0])
                x_end = int(bbox[0] + bbox[2])
                y_start = int(bbox[1])
                y_end = int(bbox[1] + bbox[3])

                template_img = img_left[y_start:y_end, x_start:x_end]
                img_left_clicked_size = (x_start, x_end, y_start, y_end)

                # height, width = y_end - y_start, x_end - x_start
                # result = FLANNMatch.FLANN_match(img_right, template_img, 5)
                # img_right_clicked_size = (
                #     max(int(result[0][0][0]), 0), min(int(result[0][0][0]) + width, img_right.shape[1]),
                #     max(int(result[0][0][1]), 0), min(int(result[0][0][1]) + height, img_right.shape[0]))
                # print(img_left_clicked_size, img_right_clicked_size)
                img_right_clicked_size = find_left_in_right(template_img, img_right)

                orb_num = 500
                dist = compute_distence(img_left, img_right, orb_num, img_left_clicked_size, img_right_clicked_size,
                                        True)
                if dist == -1:
                    dist = temp_dist
                else:
                    temp_dist = dist
                # 矩形的一个顶点，左上角
                p1 = (x_start, y_start)
                #  矩阵对角线上的另一个顶点，右下角
                p2 = (x_end, y_end)
                cv2.rectangle(frame, p1, p2, (255, 0, 0), 2, 1)
                cv2.putText(frame, f"distance: {round(dist, 2)}", (x_start, y_start), cv2.FONT_HERSHEY_SIMPLEX, 0.75,
                            (0, 0, 255), 2)
            else:
                # Tracking failure
                cv2.putText(frame, "Tracking failure detected", (100, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255),
                            2)
            cv2.imshow(win_name, frame)

            k = cv2.waitKey(1) & 0xff
            if k == ord("q"):
                cv2.destroyAllWindows()
                break
            elif k == ord("c") or k == ord("C"):
                # 重新选择跟踪对象
                tracker = cv2.TrackerKCF_create()
                bbox = cv2.selectROI(win_name, frame, False)
                tracker.init(frame, bbox)
            elif k == ord("s") or k == ord("S"):
                folder = os.path.join(folder_path_dynamic, folder_name_dynamic,
                                      time.strftime('%H-%M-%S', time.localtime(time.time())))
                os.mkdir(folder)
                cv2.imwrite(os.path.join(folder, "left.jpg"), img_left)
                cv2.imwrite(os.path.join(folder, "right.jpg"), img_right)
                show_depth(img_left, img_right, folder, True)
                cv2.imwrite(os.path.join(folder, "object.jpg"), frame)
            elif k == ord("f") or k == ord("F"):
                try:
                    tracker = cv2.TrackerKCF_create()
                    bbox: tuple = FLANNMatch.find_matches(frame)
                    tracker.init(frame, bbox)
                except:
                    pass

    def compute_distance(self):
        # self.open_camera()  # 先打开摄像头
        pic_left = os.path.join(folder_path, folder_name, 'left.jpg')
        pic_right = os.path.join(folder_path, folder_name, 'right.jpg')
        window_name = 'distance'
        cv2.namedWindow(window_name)
        if os.path.exists(pic_left) and os.path.exists(pic_right):  # 左右两个图片都存在才进行下一步
            frame1 = cv2.imread(pic_left)
            frame2 = cv2.imread(pic_right)
            # 根据打开的图像和相机参数更正 map 对图片进行重构
            img_left = cv2.remap(frame1, camera_config.left_map1, camera_config.left_map2, cv2.INTER_LINEAR)
            img_right = cv2.remap(frame2, camera_config.right_map1, camera_config.right_map2, cv2.INTER_LINEAR)
            #imgs = np.hstack([img_left, img_right])
            # 展示深度图像
            show_depth(img_left, img_right,os.path.join(folder_path,folder_name), False)
            cv2.imshow(window_name, img_left)

            def callbackFunc(e, x, y, p, f):
                if e == cv2.EVENT_LBUTTONDBLCLK:
                    # 通过点击来选取已以点击中心100个像素的部分进行匹配找出左右图像中目标的位置进行视差计算
                    template_img = img_left[y - 65:y + 65, x - 65:x + 65]
                    img_left_clicked_size = (x - 65, x + 65, y - 65, y + 65)
                    img_right_clicked_size = find_left_in_right(template_img, img_right)
                    orb_num = 200
                    dist = compute_distence(img_left, img_right, orb_num, img_left_clicked_size, img_right_clicked_size,
                                           False)
                    cv2.rectangle(img_left, (x - 65, y - 65), (x + 65, y + 65), (0, 0, 255), 2)
                    cv2.putText(img_left, f"distance: {round(dist, 2)}", (x - 70, y - 70), cv2.FONT_HERSHEY_SIMPLEX, 0.75,
                                (0, 0, 255),2)
                    cv2.imwrite(os.path.join(folder_path,folder_name,'object.jpg'), img_left)
                    #print(round(dist, 2))
                    self.textBrowser_4.setText("distance:" + str(round(dist, 2)))

            while True:
                key = cv2.waitKey(1)
                if key == ord("q"):
                    cv2.destroyWindow(window_name)
                    break
                cv2.setMouseCallback(window_name, callbackFunc, None)
        else:
            QMessageBox.critical(None, 'Can not find images', "please take photos first!")

    def retranslate_ui(self, measure_window):
        _translate = QtCore.QCoreApplication.translate
        measure_window.setWindowTitle(_translate("Measure_Window", "MainWindow"))
        self.button_save_pic.setText(_translate("Measure_Window", "Save Picture"))
        self.label_2.setText(_translate("Measure_Window", "Image acquisition area"))
        self.button_open_camera.setText(_translate("Measure_Window", "Open Camera"))
        self.button_hide_measure_show_home.setText(_translate("Measure_Window", "Exit"))
        self.button_take_pic.setText(_translate("Measure_Window", "Take Picture"))
        self.button_compute_distance.setText(_translate("Measure_Window", "Compute Distance"))
        self.button_auto_measure.setText(_translate("Measure_Window", "Auto measure"))
        self.menuNew.setTitle(_translate("Measure_Window", "New"))
        self.menuHelp.setTitle(_translate("Measure_Window", "Help"))
        self.actionMeasure_Task.setText(_translate("Measure_Window", "Measure Task"))
        self.actionView_Data.setText(_translate("Measure_Window", "View Data"))
        self.actionExit.setText(_translate("Measure_Window", "Exit"))
        self.actionVersion.setText(_translate("Measure_Window", "Version"))
        self.actionDocuments.setText(_translate("Measure_Window", "Documents"))

#标定窗口即为查询窗口
class Ui_View_Data_Window(object):
    def __init__(self, View_Window):
        View_Window.setObjectName("View_Window")
        View_Window.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(View_Window)
        self.centralwidget.setObjectName("centralwidget")
        self.frame = QtWidgets.QFrame(self.centralwidget)
        self.frame.setGeometry(QtCore.QRect(10, 0, 781, 571))
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.label_2 = QtWidgets.QLabel(self.frame)
        self.label_2.setGeometry(QtCore.QRect(20, 20, 361, 251))
        self.label_2.setText("")
        self.label_2.setObjectName("label_2")
        self.label_3 = QtWidgets.QLabel(self.frame)
        self.label_3.setGeometry(QtCore.QRect(20, 290, 361, 251))
        self.label_3.setText("")
        self.label_3.setObjectName("label_3")
        self.label_4 = QtWidgets.QLabel(self.frame)
        self.label_4.setGeometry(QtCore.QRect(400, 20, 361, 251))
        self.label_4.setText("")
        self.label_4.setObjectName("label_4")
        self.label_5 = QtWidgets.QLabel(self.frame)
        self.label_5.setGeometry(QtCore.QRect(400, 290, 361, 251))
        self.label_5.setText("")
        self.label_5.setObjectName("label_5")
        View_Window.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(View_Window)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 23))
        self.menubar.setObjectName("menubar")
        self.menuNew = QtWidgets.QMenu(self.menubar)
        self.menuNew.setObjectName("menuNew")
        self.menuHelp = QtWidgets.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        View_Window.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(View_Window)
        self.statusbar.setObjectName("statusbar")
        View_Window.setStatusBar(self.statusbar)
        self.actionMeasure_Task = QtWidgets.QAction(View_Window)
        self.actionMeasure_Task.setObjectName("actionMeasure_Task")
        self.actionSystem_Calibration = QtWidgets.QAction(View_Window)
        self.actionSystem_Calibration.setObjectName("actionSystem_Calibration")
        self.actionView_Data = QtWidgets.QAction(View_Window)
        self.actionView_Data.setObjectName("actionView_Data")
        self.actionVersion = QtWidgets.QAction(View_Window)
        self.actionVersion.setObjectName("actionVersion")
        self.actionDocuments = QtWidgets.QAction(View_Window)
        self.actionDocuments.setObjectName("actionDocuments")
        self.actionExit = QtWidgets.QAction(View_Window)
        self.actionExit.setObjectName("actionExit")
        self.menuNew.addAction(self.actionMeasure_Task)
        self.menuNew.addAction(self.actionView_Data)
        self.menuNew.addAction(self.actionExit)
        self.menuHelp.addAction(self.actionVersion)
        self.menuHelp.addAction(self.actionDocuments)
        self.menubar.addAction(self.menuNew.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.actionMeasure_Task.triggered.connect(self.show_measure_hide_view)
        self.actionVersion.triggered.connect(self.show_version_hide_view)
        self.actionView_Data.triggered.connect(view_data)
        self.actionExit.triggered.connect(QApplication.quit)



        self.retranslateUi(View_Window)
        QtCore.QMetaObject.connectSlotsByName(View_Window)

    def show_measure_hide_view(self):
        clear_measure_window()
        show_hide_window(ViewWindow, MeasureUiWindow)
    def show_version_hide_view(self):
        show_hide_window(ViewWindow, VersionWindow)

    def retranslateUi(self, View_Window):
        _translate = QtCore.QCoreApplication.translate
        View_Window.setWindowTitle(_translate("View_Window", "MainWindow"))
        self.menuNew.setTitle(_translate("View_Window", "New"))
        self.menuHelp.setTitle(_translate("View_Window", "Help"))
        self.actionMeasure_Task.setText(_translate("View_Window", "Measure Task"))
        self.actionSystem_Calibration.setText(_translate("View_Window", "System Calibration"))
        self.actionView_Data.setText(_translate("View_Window", "View Data"))
        self.actionVersion.setText(_translate("View_Window", "Version"))
        self.actionDocuments.setText(_translate("View_Window", "Documents"))
        self.actionExit.setText(_translate("View_Window", "Exit"))

# 版本信息窗口
class UiVersionWindow(object):
    def __init__(self, version_window):
        version_window.setObjectName("Version_Window")
        version_window.resize(window_width, window_height)
        self.central_widget = QtWidgets.QWidget(version_window)
        self.central_widget.setObjectName("centralwidget")
        self.frame = QtWidgets.QFrame(self.central_widget)
        self.frame.setGeometry(QtCore.QRect(10, 0, 751, 541))
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.label = QtWidgets.QLabel(self.frame)
        self.label.setGeometry(QtCore.QRect(120, 10, 551, 201))
        font = QtGui.QFont()
        font.setPointSize(22)
        self.label.setFont(font)
        self.label.setObjectName("label")
        version_window.setCentralWidget(self.central_widget)
        self.menubar = QtWidgets.QMenuBar(version_window)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 23))
        self.menubar.setObjectName("menubar")
        self.menuNew = QtWidgets.QMenu(self.menubar)
        self.menuNew.setObjectName("menuNew")
        self.menuHelp = QtWidgets.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        version_window.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(version_window)
        self.statusbar.setObjectName("statusbar")
        version_window.setStatusBar(self.statusbar)
        self.actionMeasure_Task = QtWidgets.QAction(version_window)
        self.actionMeasure_Task.setObjectName("actionMeasure_Task")
        self.actionView_Data = QtWidgets.QAction(version_window)
        self.actionView_Data.setObjectName("actionView_Data")
        self.actionExit = QtWidgets.QAction(version_window)
        self.actionExit.setObjectName("actionExit")
        self.actionVersion = QtWidgets.QAction(version_window)
        self.actionVersion.setObjectName("actionVersion")
        self.actionDocuments = QtWidgets.QAction(version_window)
        self.actionDocuments.setObjectName("actionDocuments")
        self.menuNew.addAction(self.actionMeasure_Task)
        self.menuNew.addAction(self.actionView_Data)
        self.menuNew.addAction(self.actionExit)
        self.menuHelp.addAction(self.actionVersion)
        self.menuHelp.addAction(self.actionDocuments)
        self.menubar.addAction(self.menuNew.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.actionMeasure_Task.triggered.connect(main_ui.hide_home_show_measure)
        self.actionView_Data.triggered.connect(self.hide_version_show_view)
        self.actionExit.triggered.connect(QApplication.quit)

        self.retranslateUi(version_window)
        QtCore.QMetaObject.connectSlotsByName(version_window)

    def hide_version_show_measure(self):
        clear_measure_window()
        show_hide_window(VersionWindow, MainWindow)

    def hide_version_show_view(self):
        view_data()
        show_hide_window(VersionWindow, ViewWindow)

    def retranslateUi(self, Version_Window):
        _translate = QtCore.QCoreApplication.translate
        Version_Window.setWindowTitle(_translate("Version_Window", "MainWindow"))
        self.label.setText(_translate("Version_Window", "School:SouthWest JiaoTong University\n"
                                                        "Author:Fu Tangshuai,Zeng Lihu\n"
                                                        "ID:和尚洗头用清扬\n"
                                                        "Date:2020-2021\n"
                                                        "Version1.0"))
        self.menuNew.setTitle(_translate("Version_Window", "New"))
        self.menuHelp.setTitle(_translate("Version_Window", "Help"))
        self.actionMeasure_Task.setText(_translate("Version_Window", "Measure Task"))
        self.actionView_Data.setText(_translate("Version_Window", "View Data"))
        self.actionExit.setText(_translate("Version_Window", "Exit"))
        self.actionVersion.setText(_translate("Version_Window", "Version"))
        self.actionDocuments.setText(_translate("Version_Window", "Documents"))


# 定义一个实时视频类
class Video:
    def __init__(self, cam):
        super().__init__()
        self.cam = cam
        self.cam.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        self.cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        self._timer = QTimer()
        self.label = QtWidgets.QLabel()
        self._timer.timeout.connect(self.play)

    def play(self):
        ret, frame = self.cam.read()
        if ret:
            show_pic(frame, self.label)

    def stop_camera(self):
        if self._timer.isActive():
            self._timer.stop()
        if self.cam.isOpened:
            self.cam.release()

    @property
    def timer(self):
        return self._timer

    def get_pic(self, file_path):
        ret, frame = self.cam.read()
        if ret:
            cv2.imwrite(file_path, frame)


def show_hide_window(hide_window, show_window):
    hide_window.hide()
    show_window.show()


def clear_measure_window():
    measure_ui.label_5.clear()
    measure_ui.textBrowser_4.clear()

#用于静态测量和动态测量的创建文件夹操作
def create_folder_dynamic_static(dynamic_flag):
    path_dialogue = QInputDialog()
    while True:
        if dynamic_flag:
            folder_path_temp = folder_path_dynamic
        else:
            folder_path_temp = folder_path
        global folder_name
        folder_name, ok = path_dialogue.getText(None, 'Create the folder', 'Input Folder name:',
                                                QLineEdit.Normal, '')
        folder_time = time.strftime('%Y-%m-%d', time.localtime(time.time()))
        folder_name = folder_name + '_' + folder_time
        if not ok:  # 取消操作
            break
        elif folder_name in os.listdir(folder_path_temp):
            # finished 可能要打开以前创建的文件夹 跳转view data
            reply = QMessageBox.question(None, "Folder existed!",
                                         "Folder existed! Do you want to open the folder to view data?")
            # print(reply)
            if reply == 16384:
                ok = True
                break
            # QMessageBox.critical(None, 'Invalid Input', "The folder has existed!")
        else:
            break
    if ok and folder_name != '':
        if not os.path.exists(os.path.join(folder_path_temp, folder_name)):
            os.makedirs(os.path.join(folder_path_temp, folder_name))
    if dynamic_flag:
        global folder_name_dynamic
        folder_name_dynamic = folder_name

def show_pic(pic_show, show_location):
    rgb_image = cv2.cvtColor(pic_show, cv2.COLOR_BGR2RGB)
    rgb_image = cv2.resize(rgb_image, (show_location.width(), show_location.height()), interpolation=cv2.INTER_NEAREST)
    h, w, ch = rgb_image.shape
    bytes_per_line = ch * w
    image = QPixmap.fromImage(QImage(rgb_image.data, w, h, bytes_per_line,
                                     QImage.Format_RGB888))
    show_location.setPixmap(image)

def view_data():
    open_folder = QFileDialog.getExistingDirectory(None, "Choose the file",'./data')

    if 'dynamic' in open_folder:
        open_folder = os.path.join(folder_path_dynamic, open_folder.split('/')[-2], open_folder.split('/')[-1])
    else:
        open_folder = os.path.join(folder_path, open_folder.split('/')[-1])
    print(open_folder)
    left_img = cv2.imread(os.path.join(open_folder, 'left.jpg'))
    show_pic(left_img, view_ui.label_2)
    right_img = cv2.imread(os.path.join(open_folder, 'right.jpg'))
    show_pic(right_img, view_ui.label_3)
    color_img = cv2.imread(os.path.join(open_folder, 'color.jpg'))
    show_pic(color_img, view_ui.label_4)
    object_img = cv2.imread(os.path.join(open_folder, 'object.jpg'))
    show_pic(object_img, view_ui.label_5)



# 显示深度图以及写深度图
def show_depth(img_left, img_right,folder,dynamic_flag):
    min_disp = 2
    num = 2
    block_size = 5
    param = {'minDisparity': min_disp,
             'numDisparities': num * 16,
             'blockSize': block_size,
             'P1': 8 * 3 * block_size ** 2,
             'P2': 32 * 3 * block_size ** 2,
             'disp12MaxDiff': 154,
             'preFilterCap': 5,
             'uniquenessRatio': 6,
             'speckleWindowSize': 60,
             'speckleRange': 2,
             'mode': cv2.STEREO_SGBM_MODE_SGBM_3WAY
             }
    stereo = cv2.StereoSGBM_create(**param)
    stereoR = cv2.ximgproc.createRightMatcher(stereo)
    # 使用WLS滤波器进行滤波，去除噪声
    lmbda = 80000
    sigma = 1.8
    wls_filter = cv2.ximgproc.createDisparityWLSFilter(matcher_left=stereo)
    wls_filter.setLambda(lmbda)
    wls_filter.setSigmaColor(sigma)
    disparity = stereo.compute(img_left, img_right)  # .astype(np.float32)/ 16
    dispL = disparity

    dispR = stereoR.compute(img_right, img_left)
    dispL = np.int16(dispL)
    dispR = np.int16(dispR)
    filteredImg = wls_filter.filter(dispL, img_left, None, dispR)

    filteredImg = cv2.normalize(src=filteredImg, dst=filteredImg, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX,
                                dtype=cv2.CV_8U)
    filt_color = cv2.applyColorMap(filteredImg, cv2.COLORMAP_OCEAN)
    cv2.imwrite(os.path.join(folder, 'color.jpg'), filt_color)
    if not dynamic_flag:
        show_pic(filt_color,measure_ui.label_5)

class parallax():
    def __init__(self, leftx, rightx, paravalue):
        self.leftX = leftx
        self.rightX = rightx
        self.paraValue = paravalue


class DrawingType(Enum):
    ONLY_LINES = 1
    LINES_AND_POINTS = 2
    COLOR_CODED_POINTS_X = 3
    COLOR_CODED_POINTS_Y = 4
    COLOR_CODED_POINTS_XpY = 5


def draw_matches(src1, src2, kp1, kp2, matches, drawing_type):
    height = max(src1.shape[0], src2.shape[0])
    width = src1.shape[1] + src2.shape[1]
    output = np.zeros((height, width, 3), dtype=np.uint8)
    output[0:src1.shape[0], 0:src1.shape[1]] = src1
    output[0:src2.shape[0], src1.shape[1]:] = src2[:]

    if drawing_type == DrawingType.ONLY_LINES:
        for i in range(len(matches)):
            left = kp1[matches[i].queryIdx].pt
            right = tuple(sum(x) for x in zip(kp2[matches[i].trainIdx].pt, (src1.shape[1], 0)))
            cv2.line(output, tuple(map(int, left)), tuple(map(int, right)), (0, 255, 255))

    elif drawing_type == DrawingType.LINES_AND_POINTS:
        for i in range(len(matches)):
            left = kp1[matches[i].queryIdx].pt
            right = tuple(sum(x) for x in zip(kp2[matches[i].trainIdx].pt, (src1.shape[1], 0)))
            cv2.line(output, tuple(map(int, left)), tuple(map(int, right)), (255, 0, 0))

        for i in range(len(matches)):
            left = kp1[matches[i].queryIdx].pt
            right = tuple(sum(x) for x in zip(kp2[matches[i].trainIdx].pt, (src1.shape[1], 0)))
            cv2.circle(output, tuple(map(int, left)), 1, (0, 255, 255), 2)
            cv2.circle(output, tuple(map(int, right)), 1, (0, 255, 0), 2)

    elif drawing_type == DrawingType.COLOR_CODED_POINTS_X or drawing_type == DrawingType.COLOR_CODED_POINTS_Y or drawing_type == DrawingType.COLOR_CODED_POINTS_XpY:
        _1_255 = np.expand_dims(np.array(range(0, 256), dtype='uint8'), 1)
        _colormap = cv2.applyColorMap(_1_255, cv2.COLORMAP_HSV)

        for i in range(len(matches)):
            left = kp1[matches[i].queryIdx].pt
            right = tuple(sum(x) for x in zip(kp2[matches[i].trainIdx].pt, (src1.shape[1], 0)))

            if drawing_type == DrawingType.COLOR_CODED_POINTS_X:
                colormap_idx = int(left[0] * 256. / src1.shape[1])  # x-gradient
            if drawing_type == DrawingType.COLOR_CODED_POINTS_Y:
                colormap_idx = int(left[1] * 256. / src1.shape[0])  # y-gradient
            if drawing_type == DrawingType.COLOR_CODED_POINTS_XpY:
                colormap_idx = int((left[0] - src1.shape[1] * .5 + left[1] - src1.shape[0] * .5) * 256. / (
                        src1.shape[0] * .5 + src1.shape[1] * .5))  # manhattan gradient

            color = tuple(map(int, _colormap[colormap_idx, 0, :]))
            cv2.circle(output, tuple(map(int, left)), 1, color, 2)
            cv2.circle(output, tuple(map(int, right)), 1, color, 2)
    return output


def compute_distence(img_left, img_right, orb_num, img_left_clicked_size,
                     img_right_clicked_size, auto_compute: bool):
    """
    :param img_left:
    :param img_right:
    :param orb_num:
    :param img_left_clicked_size:
    :param img_right_clicked_size:
    :param auto_compute: 是否是自动计算模式，若是，则不弹窗提醒
    """
    flag, paraMean = compute_paraValue(img_left, img_right, orb_num, img_left_clicked_size,
                                       img_right_clicked_size)
    find_start_time = time.time()
    if not auto_compute:
        while not flag:
            orb_num += 100
            flag, paraMean = compute_paraValue(img_left, img_right, orb_num, img_left_clicked_size,
                                               img_right_clicked_size)
            if time.time() - find_start_time > 4:
                QMessageBox.critical(None, 'cannot find',
                                     "The point cannot calculate,Please select another point!")
                break
    else:  # 如果是auto measure 则直接返回-1，防止多次计算导致卡顿
        if not flag:
            return -1.
    if paraMean < 0 or paraMean > 400:
        y = 0
    if paraMean > 107:
        y = -0.0000028541  * paraMean ** 3 + 0.0026632801 * paraMean ** 2 - 0.8807328385 * paraMean + 122.24
    if 46 < paraMean < 107:
        y = -0.0002617619  * paraMean ** 3 + 0.0754835287 * paraMean ** 2 - 7.9610891045 * paraMean + 363.93
    if paraMean < 46:
        y = -0.0015684441  * paraMean ** 3 + 0.2947637774 * paraMean ** 2 - 19.9026825376 * paraMean + 580.1
    if 70< y < 90:
        y = int((y-70)/10 + 0.5) + y
    if y > 90:
        y = int((y -90)/20 + 1.5) + y
    return y


def compute_paraValue(left_img, right_img, orb_num, left_img_size, right_img_size):
    """
    :param left_img:
    :param right_img:
    :param orb_num: 特征点数
    :param left_img_size: 代表目标所在的区域，是一个元组，0，1为宽，2，3为高
    :param right_img_size:
    :return:
    """
    orb = cv2.ORB_create(orb_num)  # 建立一个特征点集
    orb.setFastThreshold(0)
    flag = 0  # 标志是否找到特征点
    left_img = left_img[left_img_size[2]:left_img_size[3], left_img_size[0]:left_img_size[1]]
    right_img = right_img[right_img_size[2]:right_img_size[3], right_img_size[0]:right_img_size[1]]
    kp1, des1 = orb.detectAndCompute(left_img, None)  # 计算特征点集
    kp2, des2 = orb.detectAndCompute(right_img, None)
    matcher = cv2.BFMatcher(cv2.NORM_HAMMING)  # 提供对象实现匹配
    matches_all = matcher.match(des1, des2)  # 匹配特征点
    matches_gms = matchGMS(left_img.shape[:2], right_img.shape[:2], kp1, kp2, matches_all, withScale=False,
                           withRotation=False,
                           thresholdFactor=5)
    if matches_gms:
        # 创建新的视差对象列表
        flag = 1
        parallax_list = []
        for i in range(len(matches_gms)):
            leftX = kp1[matches_gms[i].queryIdx].pt[0] + left_img_size[0]
            rightX = kp2[matches_gms[i].trainIdx].pt[0] + right_img_size[0]
            para_temp = parallax(leftX, rightX, leftX - rightX)
            parallax_list.append(para_temp)
        parallax_list.sort(key=lambda x: x.paraValue)
        # 筛选匹配点
        idxMedian = int(len(parallax_list) / 2)
        paraMedian = parallax_list[idxMedian].paraValue

        errorRange = 0.05
        delete_index = []
        for it in parallax_list:
            if it.paraValue < (1 - errorRange) * paraMedian or it.paraValue > (1 + errorRange) * paraMedian:
                parallax_list.remove(it)
        # 将匹配结果写入文件夹
        output = draw_matches(left_img, right_img, kp1, kp2, matches_gms, DrawingType.LINES_AND_POINTS)
        paraSum = 0
        # 计算平均视差
        for it in parallax_list:
            paraSum += it.paraValue
        paraMean = paraSum / len(parallax_list)
        return flag, paraMean
    else:
        return flag, 0


# 匹配左图像在右图像中的位置
def find_left_in_right(img_left, img_right):
    img_right_gray = cv2.cvtColor(img_right, cv2.COLOR_BGR2GRAY)
    template_img_gray = cv2.cvtColor(img_left, cv2.COLOR_BGR2GRAY)
    height, width = template_img_gray.shape[:2]
    res = cv2.matchTemplate(img_right_gray, template_img_gray, cv2.TM_CCOEFF)
    min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
    left_top = max_loc
    return left_top[0], left_top[0] + width, left_top[1], left_top[1] + height


def creat_folder():
    """
    folder_path 有可能不存在，程序启动时判断一下
    :return: None
    """
    if not os.path.exists(folder_prefix):
        os.mkdir(folder_prefix)
        os.mkdir(folder_path)
        os.mkdir(folder_path_dynamic)


if __name__ == '__main__':
    creat_folder()
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)  # 让字体适应高分辨率屏幕 （hi-dpi）
    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon("icon.jpg"))

    MainWindow = QMainWindow()
    main_ui = UiMainBox(MainWindow)
    MainWindow.show()

    MeasureUiWindow = QMainWindow()
    measure_ui = UiMeasureWindow(MeasureUiWindow)


    ViewWindow = QMainWindow()
    view_ui = Ui_View_Data_Window(ViewWindow)

    VersionWindow = QMainWindow()
    version_ui = UiVersionWindow(VersionWindow)

    cam_left = Video(cv2.VideoCapture(0))
    cam_right = Video(cv2.VideoCapture(1))

    sys.exit(app.exec_())
