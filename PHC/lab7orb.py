import cv2
import numpy as np


def findGhost(ghost, image, output, color):
    sift = cv2.SIFT_create()

    keypoints_1, descriptors_1 = sift.detectAndCompute(ghost, None)
    keypoints_2, descriptors_2 = sift.detectAndCompute(image, None)

    bf = cv2.BFMatcher(cv2.NORM_L1, crossCheck=True)
    matches = bf.match(descriptors_1, descriptors_2)

    src_pts = np.float32([keypoints_1[m.queryIdx].pt for m in matches]).reshape(-1, 1, 2)
    dst_pts = np.float32([keypoints_2[m.trainIdx].pt for m in matches]).reshape(-1, 1, 2)

    M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 15.0)
    matchesMask = mask.ravel().tolist()

    if (sum(matchesMask) < 10): return False

    h, w, _ = ghost.shape
    pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
    dst = cv2.perspectiveTransform(pts, M)

    output = cv2.polylines(output, [np.int32(dst)], True, color, 3, cv2.LINE_AA)
    # image = cv2.fillPoly(image, [np.int32(dst)], (0, 0, 0))

    cv2.imshow("a", output)
    cv2.imshow("b", image)
    cv2.waitKey(0)
    return True

ret = True
img = cv2.imread("./Halloween/lab7.png")
while(ret):
    ret = findGhost(cv2.imread("./Halloween/pampkin_ghost.png"), img, cv2.imread("./Halloween/lab7.png"), (255, 0, 0))
ret = True
while (ret):
    ret = findGhost(cv2.imread("./Halloween/scary_ghost.png"), img, cv2.imread("./Halloween/lab7.png"), (0, 0, 255))
ret = True
while (ret):
    ret = findGhost(cv2.imread("./Halloween/candy_ghost.png"), img, cv2.imread("./Halloween/lab7.png"), (0, 255, 0))