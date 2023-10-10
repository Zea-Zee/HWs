import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import cv2
import os
# import simpleaudio as sa
# %matplotlib inline

images = os.listdir("./nails_dataset/images")

# cv2.namedWindow('image, label, image with contours from label')
# cv2.moveWindow('image, label, image with contours from label', 0, 0)
# cv2.resizeWindow('image, label, image with contours from label', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
# for el in images:
#     image = cv2.imread("./nails_dataset/images/" + el)
#     label = cv2.imread("./nails_dataset/labels/" + el)
#     gray = cv2.cvtColor(label, cv2.COLOR_BGR2GRAY)
#     contours, _ = cv2.findContours(gray, 1, 2)
#     cv2.drawContours(image, contours, -1, (0, 0, 0), 0)
#     # concatenated_img = np.concatenate((cv2.imread("./nails_dataset/images/" + el), label, image), axis=1)
#     concatenated_img = cv2.hconcat()
#     concatenated_img = cv2.resize(concatenated_img, (1920, 1080))
#     cv2.imshow("image, label, image with contours from label", concatenated_img)
#     key = cv2.waitKey(0)
#     if key == ord('q'):
#         break
# cv2.destroyWindow("image and label")

# for el in images[0:1]:
#     image = cv2.imread("./nails_dataset/images/" + el)
#     label = cv2.imread("./nails_dataset/labels/" + el)
#     gray = cv2.cvtColor(label, cv2.COLOR_BGR2GRAY)
#     # cv2.imshow("gray", gray)
#     contours, _ = cv2.findContours(gray, 1, 2)
#     cv2.drawContours(image, contours, -1, (0, 0, 0), 0)
#     cv2.imshow("nail with edge", image)
#     key = cv2.waitKey(0)
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows()

cap = cv2.VideoCapture("./video.mp4")
# audio = AudioSegment.from_file(audio_path)
# playback_obj = sa.play_buffer(audio.raw_data, num_channels=audio.channels, bytes_per_sample=audio.sample_width,
#                                    sample_rate=audio.frame_rate)
while True:
    ret, frame = cap.read()
    if not ret:
        break
    cv2.imshow('Video', cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY))
    fps = cap.get(cv2.CAP_PROP_FPS)
    # if cv2.waitKey(int(1000 / fps)) & 0xFF == ord('q'):
    #     break
cap.release()
cv2.destroyAllWindows()