import cv2
import numpy as np
import math
from PIL import Image


FIG_SIZE = 1000
glob_img = 0
figures = {}

def cut_corners(img):
    im = Image.fromarray(img)
    m = img == 255  # Проверка на черный цвет
    coords = np.column_stack(np.where(m))
    y0, x0, y1, x1 = *np.min(coords, axis=0), *np.max(coords, axis=0)
    im = im.crop((x0, y0, x1+1, y1+1))
    ret = np.array(im)
    # print("cutted size", ret.shape)
    return ret

def rot_img(img, angle):
    h, w = img.shape
    res = img.copy()
    M = cv2.getRotationMatrix2D((w // 2, h // 2), angle, 1)
    res = cv2.warpAffine(res, M, (w, h))
    res = cut_corners(res)
    res = cv2.resize(res, (FIG_SIZE - 1, FIG_SIZE - 1))
    # print("new shape", res.shape)
    # cv2.imshow("rot", res)
    # cv2.waitKey(0)
    return res


def create_regular_polygon(num_vertices, is_rhombus=False):
    center = (FIG_SIZE // 2, FIG_SIZE // 2)  # Центр изображения
    image = np.zeros((FIG_SIZE, FIG_SIZE), dtype=np.uint8)  # Прозрачный фон (4 канала: BGR + Альфа)

    vertices = []
    for i in range(num_vertices):
        angle_rad = 2 * math.pi * i / num_vertices
        x = int(center[0] + FIG_SIZE // 2 * math.cos(angle_rad))
        y = int(center[1] + FIG_SIZE // 2 * math.sin(angle_rad))
        vertices.append((x, y))

        # Отрисовка многоугольника
    vertices = np.array(vertices, dtype=np.int32)
    vertices = vertices.reshape((-1, 1, 2))
    cv2.fillPoly(image, [vertices], 255)  # Белый цвет
    croped_image = cut_corners(image)

    h, w = croped_image.shape
    if h < w:
        croped_image = cv2.resize(croped_image, (FIG_SIZE - 1, int(FIG_SIZE / w * h) - 1))
    else:
        croped_image = cv2.resize(croped_image, (int(FIG_SIZE / h * w) - 1, FIG_SIZE - 1))
    # print("returned size", croped_image.shape)
    # cv2.imshow("new fig", croped_image)
    # cv2.waitKey(0)
    return croped_image

color_ranges = {
    "blue": [np.array([110, 50, 50]), np.array([130, 255, 255])],
    "green": [np.array([40, 50, 50]), np.array([80, 255, 255])],
    "yellow": [np.array([20, 50, 50]), np.array([40, 255, 255])],
    "red": [np.array([0, 50, 50]), np.array([10, 255, 255])]
}

colors = {
    "blue": (255, 100, 100),
    "green": (0, 255, 0),
    "yellow": (0, 255, 255),
    "red": (0, 0, 255),
}

def get_colored_imges(img, color_range, color, figures, new_img):
    print(color_range, color_range[0], color_range[1])
    mask = cv2.inRange(img, color_range[0], color_range[1])
    # mask = cv2.inRange(img, np.array([110, 50, 50]), np.array([130, 255, 255]))
    cv2.imshow("mask", mask)
    cv2.waitKey(0)
    img = cv2.cvtColor(img, cv2.COLOR_HSV2BGR)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)


    ret = []

    # Итерация по всем контурам
    for contour in contours:
        tst = mask.copy()
        cv2.drawContours(tst, [contour], -1, color[0], 2)
        cv2.imshow("contour", tst)
        x, y, w, h = cv2.boundingRect(contour)
        roi = img[y:y + h, x:x + w]

        # Преобразование подизображения в оттенки серого
        _, thresh_roi = cv2.threshold(roi, 127, 255, cv2.THRESH_BINARY)
        best_name = "nothing"
        best_score = float('-inf')
        for key in figures:
            res = cv2.matchTemplate(roi, figures[key], cv2.TM_CCOEFF_NORMED)
            minval, maxval, minloc, maxloc = cv2.minMaxLoc(res)
            if maxval > best_score:
                best_score = maxval
                best_name = key
        print(f"new fig is {best_name} with score {best_score}")
        res = cv2.matchTemplate(img, figures[best_name], cv2.TM_CCOEFF_NORMED)
        minval, maxval, minloc, maxloc = cv2.minMaxLoc(res)
        print(f"it was added with coords {maxloc}")
        ret.append((best_name, maxloc))

        cv2.drawContours(new_img, [contour], -1, color[0], 2)
        shape_label = best_name + ", " + color[1]
        cv2.putText(new_img, shape_label, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color[0], 2)
        cv2.imshow("with new fig", new_img)
        cv2.waitKey(0)
    return ret, new_img

def draw_imges_in_topic(img):
    new_img = glob_img
    for key in color_ranges:
        fig_lst, new_img = get_colored_imges(img.copy(), color_ranges[key], (colors[key], key), figures, new_img)
        print(fig_lst)
        cv2.imshow("recognized figures", new_img)
        cv2.waitKey(0)


img = cv2.imread("./aruco_cam.jpg")
h, w, c = img.shape
h, w = h * 2, w * 2
img = cv2.resize(img, (w, h))
FIG_SIZE = w // 12
hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
cv2.imshow("img", img)
glob_img = img.copy()

# figures["triangle"] = create_regular_polygon(num_vertices=3)
figures["square"] = rot_img(create_regular_polygon(num_vertices=4), 45)
figures["rhombus"] = create_regular_polygon(num_vertices=4)
figures["pentagon"] = create_regular_polygon(num_vertices=5)
figures["hexagon"] = create_regular_polygon(num_vertices=6)
figures["circle"] = create_regular_polygon(num_vertices=32)

for el in figures.items():
    cv2.imshow("fig")

draw_imges_in_topic(hsv_img)

# cv2.waitKey(0)
cv2.destroyAllWindows()