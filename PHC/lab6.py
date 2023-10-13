import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import cv2
import os

cap = cv2.VideoCapture("./video.mp4")

ret, frame_a = cap.read()
cv2.namedWindow('diffplusorig')
cv2.moveWindow('diffplusorig', 0, 0)
cv2.namedWindow('diff')
cv2.moveWindow('diff', 855, 0)
while ret:
    ret, frame_b = cap.read()
    frame_ag = cv2.cvtColor(frame_a, cv2.COLOR_BGR2GRAY)
    frame_bg = cv2.cvtColor(frame_b, cv2.COLOR_BGR2GRAY)
    diffFrame = cv2.absdiff(frame_ag, frame_bg)
    # diffFrame = cv2.cvtColor(diffFrame, cv2.COLOR_BGR2GRAY)
    _, thresholded_diff = cv2.threshold(diffFrame, 30, 255, cv2.THRESH_BINARY)
    contours, _ = cv2.findContours(thresholded_diff, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cv2.drawContours(frame_b, contours, -1, (0, 255, 0), 10)
    cv2.imshow("diffplusorig", thresholded_diff)
    cv2.imshow("diff", diffFrame)
    # cap.read()
    # cap.read()
    frame_a = frame_b#cap.read()
    if cv2.waitKey(100) == ord('q'):
        break
