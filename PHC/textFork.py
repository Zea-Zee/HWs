import cv2
import numpy as np
import requests
from PIL import Image

HEIGHT = 54

def wait():
    key = cv2.waitKey(0)
    if key == ord('q'):
        exit(0)
    cv2.destroyAllWindows()

def cut_corners(img):
    color_converted = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)
    im = Image.fromarray(color_converted)
    a = np.array(im)[:,:,:3]  # keep RGB only
    m = np.any(a != [255, 255, 255], axis=2)
    coords = np.argwhere(m)
    y0, x0, y1, x1 = *np.min(coords, axis=0), *np.max(coords, axis=0)
    im = im.crop((x0, y0, x1+1, y1+1))
    ret = np.array(im)
    ret = cv2.cvtColor(ret, cv2.COLOR_BGR2GRAY)
    return ret

def match_letter(letter, alphabet):
    # scores = {}
    # for char in range(ord('A'), ord('Z') + 1):
    #     scores[char] = 0
    #     # scores.append(0)
    # for char in range(ord('1'), ord('9') + 1):
    #     scores[char] = 0
    #     # scores.append(0)
    # # print(scores)
    # cv2.imshow("let", alphabet[0])
    # wait()
    biggest_index = 0
    biggest_val = 0

    for i in range(len(alphabet)):
        feat = cv2.ORB_create()
        kp1, des1 = feat.detectAndCompute(alphabet[i], None)
        kp2, des2 = feat.detectAndCompute(letter, None)

        bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
        matches = bf.knnMatch(des1, des2)
        count = 0
        matched_image = cv2.drawMatchesKnn(alphabet[i],kp1, letter, kp2, matches, None,matchColor=(0, 255, 0), matchesMask=None,singlePointColor=(255, 0, 0), flags=0)
        # cv2.imshow("matches", matched_image)
        # wait()
        for m, n in matches:
            if m.distance < 0.9 * n.distance:
                count += 1
        if count > biggest_index:
            biggest_index = count
            biggest_val = i
    if biggest_index == 0:
        biggest_index = 78
    elif biggest_index < 13:
        biggest_index += 66
    elif biggest_index >= 13 and biggest_index < 26:
        biggest_index += 65
    else:
        biggest_index += 23
    return chr(biggest_index)


def match_letter_templater(letter, alphabet):
    biggest_index = 0
    biggest_val = 0
    lh, lw = letter.shape
    letter = cv2.resize(letter, (int(lw / lh * HEIGHT), HEIGHT))

    for i in range(len(alphabet)):
        ah, aw = alphabet[i].shape
        alphabet[i] = cv2.resize(alphabet[i], (int(aw / ah * HEIGHT), HEIGHT))
        res = cv2.matchTemplate(alphabet[i], letter, cv2.TM_CCOEFF_NORMED)
        minval, maxval, minloc, maxloc = cv2.minMaxLoc(res)
        # print(maxval)
        if maxval > biggest_val:
            biggest_index = i
            biggest_val = maxval
    if biggest_index == 0:
        biggest_index = 78
    elif biggest_index <= 13:
        biggest_index += 64
    elif biggest_index > 13 and biggest_index < 26:
        biggest_index += 65
    else:
        biggest_index += 23
    return chr(biggest_index)


def sort_by_text(lst, img):
    positions = []
    letter_images = lst.copy()
    for letter_image in letter_images:
        # cv2.imshow("let", letter_image)
        result = cv2.matchTemplate(img, letter_image, cv2.TM_CCOEFF_NORMED)
        h, w = letter_image.shape                                                       #there could be an error with overflow
        ih, iw = img.shape
        _, _, _, max_loc = cv2.minMaxLoc(result)
        # print(max_loc, w, h, max_loc[0] + w, max_loc[1] + h)
        cv2.rectangle(img, max_loc, (max_loc[0] + w, max_loc[1] + h), (0, 0, 255), -1)
        positions.append(max_loc)
        # cv2.imshow("img", img)
        # cv2.imshow("let", letter_image)
        # wait()
        # print(lst)

    sorted_letter_images = [image for _, image in sorted(zip(positions, lst), key=lambda x: x[0])]
    return sorted_letter_images

def get_letters(templates):
    letters = []
    # th, tw = templates.shape
    # templates = cv2.resize(templates, (tw * 10, th * 10))
    _, mask = cv2.threshold(templates, 150, 255, cv2.THRESH_BINARY_INV)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for contour in contours:
        # template_contour = np.zeros_like(templates)
        # cv2.drawContours(template_contour, contour, -1, (255, 0, 0), 4)
        # cv2.imshow("
        # Cont", template_contour)
        # wait()
        x, y, w, h = cv2.boundingRect(contour)
        letter = templates[y:y + h, x:x + w]
        h, w = letter.shape
        # if w / h >= 2:
        #     double_letter = cv2.resize(letter, (w * 20, h * 20))
        #     _, new_mask = cv2.threshold(double_letter, 150, 255, cv2.THRESH_BINARY_INV)
        #     new_contours, _ = cv2.findContours(new_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        #     x, y, w, h = cv2.boundingRect(new_contours[0])
        #
        #     letter = double_letter[y:y + h, x:x + w]
        #     letter = cv2.resize(letter, (w, h))                         #N
        #     cv2.imshow("Let", letter)
        #     wait()
        #     letters.append(letter)
        #
        #     x, y, w, h = cv2.boundingRect(new_contours[1])
        #     letter = double_letter[y:y + h, x:x + w]
        #     letter = cv2.resize(letter, (w, h))                         #M
        # letter = cv2.resize(letter, (int(w / h * HEIGHT), HEIGHT))
        letters.append(letter)
        # cv2.imshow("Let", letter)
        # wait()
        # cv2.destroyAllWindows()

    return letters

def get_text_contours(img):
    _, mask = cv2.threshold(img, 150, 255, cv2.THRESH_BINARY_INV)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contour_image = np.zeros_like(img)
    cv2.drawContours(contour_image, contours, -1, (255, 0, 0), 4)
    # cv2.imshow('Mask', mask)
    # cv2.imshow('Contours', contour_image)
    # wait()

    # for i in range(len(contours)):
    #     cv2.drawContours(contour_image, contours[i], -1, (255, 0, 0), 4)
    #     # cv2.imshow('Mask', mask)
    #     cv2.imshow('Contours', contour_image)
    #     # cv2.waitKey(200)
    #     wait()
    return contour_image

def get_letter(img, alphabet):
    biggest_val = float('-inf')
    biggest_index = 0
    biggest_pos = 0
    h, w = 0, 0
    for i in range(len(alphabet)):
        res = cv2.matchTemplate(img, alphabet[i], cv2.TM_CCOEFF_NORMED)
        min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
        # print(maxval)
        th, tw = alphabet[i].shape
        if max_val > biggest_val or (max_val + 0.2 > biggest_val and tw > w):
            biggest_index = i
            biggest_val = max_val
            biggest_pos = max_loc
            h, w = alphabet[i].shape
            cv2.imshow("let", img)
            cv2.imshow("max let", alphabet[i])
            wait()
    if biggest_index == 0:
        biggest_index = 78
    elif biggest_index <= 13:
        biggest_index += 64
    elif biggest_index > 13 and biggest_index < 26:
        biggest_index += 65
    else:
        biggest_index += 23
    if biggest_val < 0.5:
        return -1
    print(biggest_val, biggest_pos, biggest_index, w, h)
    cv2.rectangle(img, biggest_pos, (biggest_pos[0] + w, biggest_pos[1] + h), (0, 0, 255), -1)
    return biggest_index

# url = input()
url = "https://ucarecdn.com/f628d67d-377e-4d1e-867c-607ebf161ecc/"
# url = "https://ucarecdn.com/f628d67d-377e-4d1e-867c-607ebf161ecc/"
# url = "https://stepik.org/media/attachments/course/187016/corrected_templ.png"
resp = requests.get(url, stream=True, timeout=100.0).raw
image = np.asarray(bytearray(resp.read()), dtype="uint8")
image = cv2.imdecode(image, cv2.IMREAD_GRAYSCALE)
image = cut_corners(image)
ih, iw = image.shape
image = cv2.resize(image, (int(iw / ih * HEIGHT), HEIGHT))
# image_contours = get_text_contours(image)

mn = "https://stepik.org/media/attachments/course/187016/MN_templ.png"
templ = "https://stepik.org/media/attachments/course/187016/text_template.png"
templ_corrected_MN = "https://stepik.org/media/attachments/course/187016/corrected_templ.png"
with_no_q = "https://stepik.org/media/attachments/course/187016/alphabet_with_no_Q.png"

resp = requests.get(with_no_q, stream=True, timeout=100.0).raw
template = np.asarray(bytearray(resp.read()), dtype="uint8")
template = cv2.imdecode(template, cv2.IMREAD_GRAYSCALE)
# print(template.shape)
# cv2.imshow("alphabet", template)
# wait()
template = cut_corners(template)
# cv2.imshow("alphabet", template)
# wait()
print(template.shape)

alph_letters = get_letters(template.copy())
# cv2.imshow("letter A", alph_letters[0])
wait()
alph_txt = sort_by_text(alph_letters, template.copy())   #letters in alphabet order

# for i in range(len(alph_txt)):
#     print(i)
#     cv2.imshow("Dfg", alph_txt[i])
#     wait()
# for el in alph_txt:
#     cv2.imshow("let", el)
#     wait()
#     cv2.destroyAllWindows()
letters = [()]
while(True):
    res = get_letter(image, alph_txt)
    if res == -1:
        break
    letters.append(chr(res))
    print(chr(res))