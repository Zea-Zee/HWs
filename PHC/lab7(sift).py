import numpy as np
import cv2
import matplotlib.pyplot as plt

def find_obj(img, templates):
    sift = cv2.SIFT_create()
    # background_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    for template in templates:
        # Convert images to grayscale

        # template_gray = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)

        # Detect keypoints and descriptors
        kp_background, desc_background = sift.detectAndCompute(img, None)
        kp_template, desc_template = sift.detectAndCompute(template, None)

        # Feature matching
        bf = cv2.BFMatcher()
        matches = bf.knnMatch(desc_template, desc_background, k=2)

        # Filter matches
        good_matches = []
        for m, n in matches:
            if m.distance < 0.75 * n.distance:
                good_matches.append(m)

        # Homography estimation
        src_pts = np.float32([kp_template[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
        dst_pts = np.float32([kp_background[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)

        M, _ = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)

        # Warp template image
        h, w = template.shape[:2]
        warped_template = cv2.warpPerspective(template, M, (w, h))

        # Convert to grayscale and apply contour detection
        warped_template_gray = warped_template#cv2.cvtColor(warped_template, cv2.COLOR_BGR2GRAY)
        contours, _ = cv2.findContours(warped_template_gray, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # Draw contours on background
        cv2.drawContours(img, contours, -1, (0, 255, 0), 2)

    # Display the result
    cv2.imshow('Result', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


background = cv2.cvtColor(cv2.imread("./Halloween/lab7.png"), cv2.COLOR_BGR2GRAY)
# background = cv2.imread("./Halloween/lab7.png")
res = background.copy()
ghost_1 = cv2.cvtColor(cv2.imread("./Halloween/candy_ghost.png"), cv2.COLOR_BGR2GRAY)
# ghost_1 = cv2.imread("./Halloween/candy_ghost.png")
ghost_2 = cv2.cvtColor(cv2.imread("./Halloween/scary_ghost.png"), cv2.COLOR_BGR2GRAY)
ghost_3 = cv2.cvtColor(cv2.imread("./Halloween/pampkin_ghost.png"), cv2.COLOR_BGR2GRAY)

find_obj(background, [ghost_1, ghost_2, ghost_3])

