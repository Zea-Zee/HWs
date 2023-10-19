import numpy as np
import cv2

cv2.namedWindow('image')
cv2.moveWindow('image', 300, 0)
# cv2.resizeWindow('image', 500, 500)

def find_obj(img, template, res_img, color):
    match_num = 0
    w, h = template.shape[1], template.shape[0]
    def find_and_rot(obj):
        for i in range(0, 360, 40):
            temp_w, temp_h = obj.shape[1], obj.shape[0]
            loc_match_num = 0
            rot_matrix = cv2.getRotationMatrix2D((temp_w // 2, temp_h // 2), i, 1.0)
            temp = cv2.warpAffine(obj, rot_matrix, (temp_w, temp_h))
            res = cv2.matchTemplate(img, temp,
                                    cv2.TM_CCOEFF_NORMED)  # 3rd -  Нормализованный коэффициент корреляции
            min_val, max_val, min_pos, max_pos = cv2.minMaxLoc(res)
            print(i, max_val, loc_match_num, max_pos)
            cv2.imshow("obj", temp)
            if max_val < 0.7:
                continue
            i -= 40
            top_left = max_pos
            loc_match_num += 1
            bottom_right = (top_left[0] + temp_w, top_left[1] + temp_h)
            cv2.rectangle(res_img, top_left, bottom_right, color, 2)
            cv2.rectangle(img, top_left, bottom_right, (0, 0, 0), -1)
            cv2.imshow("image", res_img)
            # cv2.imshow("bg", img)
            key = cv2.waitKey(1)
            if key == ord('q'):
                break
            if key == ord('c'):
                cv2.destroyAllWindows()
                # print("Number of generated pics =", gen_pics_num)
                exit(0)
        return loc_match_num

    find_and_rot(template)
    find_and_rot(cv2.resize(template, (int(w / 1.5), int(h // 1.5))))
    find_and_rot(cv2.resize(template, (int(w / 2), int(h // 2))))
    find_and_rot(cv2.resize(template, (int(w / 3), int(h // 3))))
    find_and_rot(cv2.resize(template, (int(w / 4), int(h // 4))))


background = cv2.imread("./Halloween/lab7.png")
background_cpy = background.copy()
ghost_1 = cv2.imread("./Halloween/candy_ghost.png")
ghost_2 = cv2.imread("./Halloween/scary_ghost.png")
ghost_3 = cv2.imread("./Halloween/pampkin_ghost.png")

find_obj(background, ghost_1, background_cpy, (0, 255, 0))
find_obj(background, cv2.flip(ghost_1, 0),background_cpy,  (0, 255, 0))
find_obj(background, cv2.flip(ghost_1, 1),background_cpy,  (0, 255, 0))
find_obj(background, ghost_2, background_cpy, (255, 0, 0))
find_obj(background, cv2.flip(ghost_2, 0),background_cpy,  (255, 0, 0))
find_obj(background, cv2.flip(ghost_2, 1),background_cpy,  (255, 0, 0))
find_obj(background, ghost_3, background_cpy, (0, 0, 255))
find_obj(background, cv2.flip(ghost_3, 0),background_cpy,  (0, 0, 255))
find_obj(background, cv2.flip(ghost_3, 1),background_cpy,  (0, 0, 255))
cv2.waitKey(0)

# cv2.imshow("tst", cv2.matchTemplate(background, ghost_1, cv2.TM_CCORR_NORMED))
# cv2.waitKey(0)




# # Произведите сопоставление шаблона
# result = cv2.matchTemplate(background, ghost_1, cv2.TM_CCOEFF_NORMED)
#
# # Найдите точку с максимальной корреляцией
# min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(result)
#
# # Получите размеры шаблона
# template_width, template_height = ghost_1.shape[1], ghost_1.shape[0]
#
# # Нарисуйте прямоугольник вокруг найденного объекта
# top_left = max_loc
# bottom_right = (top_left[0] + template_width, top_left[1] + template_height)
# cv2.rectangle(background, top_left, bottom_right, (0, 255, 0), 2)
#
# # Отобразите результат
# cv2.imshow('Result', background)
# cv2.waitKey(0)
# cv2.destroyAllWindows()