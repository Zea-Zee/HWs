import cv2
import numpy as np
import math
from PIL import Image


x_coeff = 56
x_shift = 0
y_coeff = 56
y_shift = 22
# rgb_color = np.uint8([[[255, 255, 0]]]) # -> 30, 255, 255
# rgb_color = np.uint8([[[255, 0, 0]]]) # -> 0, 255, 255
# rgb_color = np.uint8([[[255, 255, 255]]]) # -> 0, 255, 255
# hsv_color = cv2.cvtColor(rgb_color, cv2.COLOR_RGB2HSV)
#
# rgb_white = np.uint8([[[255, 255, 255]]]) # -> 0, 255, 255
# hsv_white = cv2.cvtColor(rgb_color, cv2.COLOR_RGB2HSV)
#
# print("HSV цвет:", hsv_color)


FIG_SIZE = 1000
scale = 2
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
    M = cv2.getRotationMatrix2D((int(w / 2), int(h / 2)), angle, 1)
    res = cv2.warpAffine(res, M, (w, h))
    res = cut_corners(res)
    res = cv2.resize(res, (FIG_SIZE - 1, FIG_SIZE - 1))
    # print("new shape", res.shape)
    # cv2.imshow("rot", res)
    # cv2.waitKey(0)
    return res


def create_regular_polygon(num_vertices, is_rhombus=False):
    center = (int(FIG_SIZE / 2), int(FIG_SIZE / 2))  # Центр изображения
    image = np.zeros((FIG_SIZE, FIG_SIZE), dtype=np.uint8)  # Прозрачный фон (4 канала: BGR + Альфа)

    vertices = []
    for i in range(num_vertices):
        angle_rad = 2 * math.pi * i / num_vertices
        x = int(center[0] + int(FIG_SIZE / 2 * math.cos(angle_rad)))
        y = int(center[1] + int(FIG_SIZE / 2 * math.sin(angle_rad)))
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
    "yellow": [np.array([25, 150, 150]), np.array([35, 255, 255])],
    "red": [np.array([0, 200, 200]), np.array([10, 255, 255])],
    # "white": [np.array(hsv_white[0, 0, :] - 10), np.array(hsv_white[0, 0, :] + 10)],
    "white": [np.array([0,0,200]), np.array([5,5,255])]
}

colors = {
    "blue": (255, 100, 100),
    "green": (100, 255, 100),
    "yellow": (100, 255, 255),
    "red": (100, 100, 255),
    "white": (200, 200, 200)
}

fig_extend = {}
objects = {}

def get_colored_imges(img, color_range, color, figures, new_img):
    # print(color_range, color_range[0], color_range[1])
    mask = cv2.inRange(img, color_range[0], color_range[1])
    # mask = cv2.inRange(img, np.array([110, 50, 50]), np.array([130, 255, 255]))
    img = cv2.cvtColor(img, cv2.COLOR_HSV2BGR)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    if color[1] == "white":
        _, mask = cv2.threshold(img, 250, 255, cv2.THRESH_BINARY)
    # cv2.imshow("mask", mask)
    # cv2.waitKey(0)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours = [cnt for cnt in contours if cv2.contourArea(cnt) >= (int(FIG_SIZE / 3)) ** 2]
    ret = []
    # Итерация по всем контурам
    for contour in contours:
        tst = mask.copy()
        tst = cv2.cvtColor(tst, cv2.COLOR_GRAY2BGR)
        cv2.drawContours(tst, [contour], -1, (0, 255, 0), 5)
        cv2.imshow("contour", tst)
        # cv2.waitKey()
        x, y, w, h = cv2.boundingRect(contour)
        roi = mask[y:y + h, x:x + w]
        # cv2.imshow("roi", roi)
        # cv2.waitKey(0)
        best_name = "nothing"
        best_score = 0.1
        # best_loc = -1
        for key in figures[color[1]]:
            res = cv2.matchTemplate(roi, figures[color[1]][key], cv2.TM_CCOEFF_NORMED)
            minval, maxval, minloc, maxloc = cv2.minMaxLoc(res)
            # print(col, key, maxval, maxloc)
            # print(maxval, key)
            cv2.imshow("roi", roi)
            cv2.imshow("fig", figures[color[1]][key])
            # cv2.waitKey(0)
            # cv2.imshow(key, figures[color[1]][key])
            if maxval > best_score:
                best_score = maxval
                best_name = key
                # best_loc = maxloc
        if best_name == "nothing":
            continue
        # print(f"new fig is {best_name} with score {best_score}")
        res = cv2.matchTemplate(mask, figures[color[1]][best_name], cv2.TM_CCOEFF_NORMED)
        minval, maxval, minloc, max_loc = cv2.minMaxLoc(res)
        if (best_name == "square"):
            cv2.destroyAllWindows()
            cv2.imshow("mask", mask)
            cv2.imshow("fig", figures[color[1]][best_name])
            cv2.waitKey(0)
        # print(f"it was added with coords {maxloc}")
        ret.append((best_name, max_loc))

        cv2.drawContours(new_img, [contour], -1, color[0], 2)
        shape_label = best_name + ", " + color[1]
        cv2.putText(new_img, shape_label, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color[0], 2)
        objects[color[1] + " " + best_name] = max_loc
        # cv2.imshow("with new fig", new_img)
        # cv2.waitKey(0)
    return ret, new_img


def draw_imges_in_topic(img):
    new_img = glob_img
    for key in color_ranges:
        fig_lst, new_img = get_colored_imges(img.copy(), color_ranges[key], (colors[key], key), fig_extend, new_img)
        # print("ADDED", fig_lst)
        # cv2.waitKey(0)
    cv2.destroyAllWindows()
    cv2.imshow("recognized figures", new_img)
    cv2.waitKey(0)


img = cv2.imread("./aruco_cam.jpg")
h, w, c = img.shape
h, w = h * scale, w * scale
img = cv2.resize(img, (w, h))
FIG_SIZE = int(w / 12)
hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
cv2.imshow("img", img)
glob_img = img.copy()

# figures["triangle"] = create_regular_polygon(num_vertices=3)
# figures["square"] = rot_img(create_regular_polygon(num_vertices=4), 45)
figures["square"] = np.ones((FIG_SIZE - 1, FIG_SIZE - 1), dtype=np.uint8) * 255
figures["rhombus"] = create_regular_polygon(num_vertices=4)
figures["pentagon"] = rot_img(create_regular_polygon(num_vertices=5), 180)
figures["hexagon"] = create_regular_polygon(num_vertices=6)
figures["circle"] = create_regular_polygon(num_vertices=32)

fig_extend = {
    "green": {
        "pentagon": figures["pentagon"],
        "square": figures["square"],
    },
    "blue": {
        "hexagon": figures["hexagon"],
        "rhombus": figures["rhombus"],
    },
    "red": {
        "square": figures["square"],
        "pentagon": figures["pentagon"],
    },
    "yellow": {
        "circle": figures["circle"],
        "rhombus": figures["rhombus"],
    },
    "white": {
        "circle": figures["circle"],
    }
}

# for key in figures:
#     print(key, figures[key].shape)
#     cv2.imshow("fig", figures[key])
#     cv2.waitKey(0)

draw_imges_in_topic(hsv_img)
for dot in objects:
    objects[dot] = tuple(int(element / scale) for element in objects[dot])
    print(dot, objects[dot])
    print(objects[dot][0] / 60, objects[dot][1] / 60, "in coords")

# cv2.waitKey(0)
cv2.destroyAllWindows()