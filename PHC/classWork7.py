import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import cv2
import os
import random
from PIL import Image

# import simpleaudio as sa
# %matplotlib inline

# def img_mask_gen(num):
#     images_left = images
#     while len(images_left) > 0:
#         # for i in range(num):
#         i = num if num < len(images_left) else len(images_left) - 1
#         limited_img_lst = images_left[:i].copy()
#         yield list(map(lambda img: "./nails_dataset/images" + img, limited_img_lst)), list(
#             map(lambda img: "./nails_dataset/labels" + img, limited_img_lst))
#         if i == len(images_left) - 1:
#             break
#         images_left = images_left[i:]
#         random.shuffle(images_left)
#
#
# return img_mask_gen(2)

width, height = 600, 600


def img_loader(imgs_path, num, width, height):
    images = os.listdir(imgs_path)
    # imges_res = list()
    # labels_res = list()

    def augmentate(img, label, imges_res, labels_res):
        imges_res.append(img)
        labels_res.append(label)
        # if random.randint(0, 10) == 1:
        #     rot_matrix = cv2.getRotationMatrix2D((width // 2, height // 2), random.randint(0, 360), 1.0)
        #     rot_img = cv2.warpAffine(img, rot_matrix, (width, height))
        #     rot_label = cv2.warpAffine(label, rot_matrix, (width, height))
        #     augmentate(rot_img, rot_label)

    def img_mask_gen():
        images_left = images
        while len(images_left) > 0:
            imges_res = list()
            labels_res = list()
            # for i in range(num):
            i = num if num < len(images_left) else len(images_left) - 1
            limited_img_lst = images_left[:i].copy()
            imges = list(map(lambda img: np.array((Image.open("./nails_dataset/images/" + img)).thumbnail((width, height), Image.Resampling.LANCZOS)), limited_img_lst))
            labels = list(map(lambda img: np.array((Image.open("./nails_dataset/labels/" + img)).thumbnail((width, height), Image.Resampling.LANCZOS)), limited_img_lst))
            for i in range(len(imges)):
                augmentate(imges[i], labels[i], imges_res, labels_res)

            yield [imges_res, labels_res]
            if i == len(images_left) - 1:
                break
            images_left = images_left[i:]
            random.shuffle(images_left)

    return img_mask_gen()

pics = img_loader("./nails_dataset/images", 1, width, height)
for el in pics:
    for i in range(len(el[0])):
        cv2.imshow("img", el[0][i])
        cv2.imshow("label", el[1][i])


