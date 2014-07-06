# Makefile server
server:	main.cpp ThreadSafeLogger.cpp OpenCVWarpper.cpp
	g++ -g main.cpp ThreadSafeLogger.cpp OpenCVWarpper.cpp -I/usr/include/ -I/usr/include/opencv2 -L/usr/lib -lopencv_calib3d -lopencv_highgui -lopencv_contrib -lopencv_imgproc -lopencv_core -lopencv_legacy -lopencv_features2d -lopencv_ml -lopencv_flann -lopencv_objdetect -lopencv_gpu -lopencv_video -lpthread -o server

jetson:	main.cpp ThreadSafeLogger.cpp OpenCVWarpper.cpp
	g++ main.cpp ThreadSafeLogger.cpp OpenCVWarpper.cpp . -I/usr/include/ -I/usr/include/opencv2 -L/usr/lib -lopencv_calib3d -lopencv_highgui -lopencv_contrib -lopencv_imgproc -lopencv_core -lopencv_legacy -lopencv_features2d -lopencv_ml -lopencv_flann -lopencv_objdetect -lopencv_gpu -lopencv_video -lpthread -o server
