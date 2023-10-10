import time
import numpy as np
# import asyncio
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import cv2
import os

# async def change_state(time, state)

cap = cv2.VideoCapture(0)

cv2.namedWindow('res')                      #window for main lab's task
cv2.moveWindow('res', 0, 400)
cv2.namedWindow('diff')                     #window for differences between frames
cv2.moveWindow('diff', 900, -50)
cv2.namedWindow('thres_diff')
cv2.moveWindow('thres_diff', 900, 400)    #window for pixel, which were assumed as moved
cv2.namedWindow('map')
cv2.moveWindow('map', 0, 0)    #window for pixel, which were assumed as moved

ret, frame_a = cap.read()
height, width, channels = frame_a.shape
frames_num = 0
is_danger = False
change_state_time = np.random.randint(1, 10) + time.time()

while ret:
    if time.time() > change_state_time:
        is_danger = not is_danger
        change_state_time = np.random.randint(1, 10) + time.time()
    ret, frame_b = cap.read()
    diffFrame = cv2.absdiff(cv2.cvtColor(frame_a, cv2.COLOR_BGR2GRAY), cv2.cvtColor(frame_b, cv2.COLOR_BGR2GRAY))
    _, thresholded_diff = cv2.threshold(diffFrame, 50, 255, cv2.THRESH_BINARY)
    output = frame_b.copy()
    empty_img = np.zeros((height, width, 3), dtype=np.uint8)
    if is_danger:
        if thresholded_diff.sum() > 1000:
            contours, _ = cv2.findContours(thresholded_diff, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            merged_contours = []
            for contour in contours:
                merged_contours.extend(contour)
            hull = cv2.convexHull(np.array(merged_contours))
            cv2.drawContours(output, [hull], -1, (0, 0, 255), 5)
            cv2.drawContours(empty_img, [hull], -1, (0, 0, 255), 5)
            cv2.putText(output, "STOP!!!", (100, 100), cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 255), 5)
    else:
        cv2.putText(output, "YOU CAN MOVE", (100, 100), cv2.FONT_HERSHEY_PLAIN, 2, (0, 255, 0), 5)
    cv2.imshow("map", thresholded_diff)
    cv2.imshow("diff", diffFrame)
    cv2.imshow("thres_diff", empty_img)
    cv2.imshow("res", output)
    if cv2.waitKey(100) == ord('q'):
        break
    frame_a = frame_b
cv2.destroyAllWindows()