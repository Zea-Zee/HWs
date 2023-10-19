import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import cv2
import os
import random
from PIL import Image

def resize_and_crop(image, t_width, t_height):
    c_width = image.shape[1]
    c_height = image.shape[0]
    c_ratio = c_width / c_height
    t_ratio = t_width / t_height
    if c_width >= t_width:
        if c_height >= t_height:
            nimage = image[int((c_height - t_height) / 2):int((c_height + t_height) / 2), : int((c_width - t_width) / 2):int((c_width + t_width) / 2)]
        else:
            image = cv2.resize(image, (int(t_height * c_ratio), t_height))
            c_width = image.shape[1]
            nimage = image[:, int((c_width - t_width) / 2): int((c_width + t_width) / 2)]
    else:
        if c_height >= t_height:
            image = cv2.resize(image, (t_width, int(t_width / c_ratio)))
            c_height = image.shape[0]
            nimage = image[int((c_height - t_height) / 2): int((c_height + t_height) / 2), :]
        else:
            image = cv2.resize(image, (int(t_height * c_ratio), t_height))
            c_width = image.shape[1]
            if c_width >= t_width:
                nimage = image[:, int((c_width - t_width) / 2): int((c_width + t_width) / 2)]
            else:
                image = cv2.resize(image, (int(t_width), int(t_width / c_ratio)))
                c_height = image.shape[0]
                nimage = image[int((c_height - t_height) / 2): int((c_height + t_height) / 2), :]
    return nimage


gwidth, gheight = 860, 920
cv2.namedWindow('image')
cv2.moveWindow('image', 0, 0)
cv2.resizeWindow('image', gwidth, gheight)

cv2.namedWindow('label')
cv2.moveWindow('label', gwidth, 0)
cv2.resizeWindow('label', gwidth, gheight)

def img_loader(imgs_path, num, width, height):
    images = os.listdir(imgs_path)

    def augmentate(img, label, imges_res, labels_res):
        imges_res.append(img)
        # cv2.imshow("tststts", imges_res[0])
        labels_res.append(label)
        rot_counter = 0
        while random.randint(0, 10) < 8 and rot_counter < 5:
            rot_counter += 1
            rot_matrix = cv2.getRotationMatrix2D((width // 2, height // 2), random.randint(0, 360), 1.0)
            imges_res.append(cv2.warpAffine(img, rot_matrix, (width, height)))
            labels_res.append(cv2.warpAffine(label, rot_matrix, (width, height)))
        if random.randint(0, 1) == 1:
            imges_res.append(cv2.flip(img, 0))
            labels_res.append(cv2.flip(label, 0))
        if random.randint(0, 1) == 1:
            imges_res.append(cv2.flip(img, 1))
            labels_res.append(cv2.flip(label, 1))
        blur_counter = 0
        while random.randint(0, 10) < 8 and blur_counter < 5:
            blur_counter += 1
            blur = min(random.randint(1, 100), width, height)
            imges_res.append(cv2.blur(img, (blur, blur)))
            labels_res.append(cv2.blur(label, (blur, blur)))
        cuts_counter = 0
        while random.randint(0, 1) < 1 and cuts_counter < 2:
            cuts_counter += 1
            xs, ys = random.randint(0, width), random.randint(0, height)
            xe, ye = random.randint(max(int(width / 2), xs), width), random.randint(max(int(height / 3), ys), height)
            rected_img = img.copy()
            rected_img = cv2.rectangle(rected_img, (xs, ys), (xe, ye), (0, 0, 0), -1)
            # cv2.imshow("rected", rected_img)
            # cv2.waitKey(0)
            rected_label = label.copy()
            rected_label = cv2.rectangle(rected_label, (xs, ys), (xe, ye), (0, 0, 0), -1)
            imges_res.append(rected_img)
            labels_res.append(rected_label)
        return [imges_res, labels_res]

    def img_mask_gen():
        images_left = images
        random.shuffle(images_left)
        while len(images_left) > 0:
            imges_res = list()
            labels_res = list()
            # for i in range(num):
            ind = num if num < len(images_left) else len(images_left) - 1
            limited_img_lst = images_left[:ind].copy()
            imges = list(map(lambda img: cv2.imread("./nails_dataset/images/" + img), limited_img_lst))
            labels = list(map(lambda img: cv2.imread("./nails_dataset/labels/" + img), limited_img_lst))
            for i in range(len(imges)):
                imges[i] = resize_and_crop(imges[i], width, height)
                labels[i] = resize_and_crop(labels[i], width, height)
            # imges = np.array(imges)
            # labels = np.array(labels)
            for i in range(len(imges)):
                imges_res, labels_res = augmentate(imges[i], labels[i], imges_res, labels_res)

            yield [imges_res, labels_res]
            if len(images_left) - 1 <= ind:
                break
            images_left = images_left[ind:]
            random.shuffle(images_left)
    return img_mask_gen()

pics = img_loader("./nails_dataset/images", 10, gwidth, gheight)
gen_pics_num = 0
for el in pics:
    gen_pics_num += len(el[0])
    for i in range(len(el[0])):
        cv2.imshow("image", el[0][i])
        cv2.imshow("label", el[1][i])
        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        if key == ord('c'):
            cv2.destroyAllWindows()
            exit(0)
cv2.destroyAllWindows()
print("Number of generated pics =", gen_pics_num)