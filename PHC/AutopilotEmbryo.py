import cv2
import numpy as np
import requests

sing_links = {
    "Главная дорога": "https://ucarecdn.com/5c4d85b9-19e7-4acd-8afc-9c8e2babf6c6/",
    "Стоянка запрещена": "https://ucarecdn.com/d249f2d7-88b7-4cc0-8d0c-eb213135efad/",
    "Крутой подъем": "https://ucarecdn.com/607031cb-bd16-4225-92bd-3c3f8a15f564/",
    "Подача звукового сигнала запрещена": "https://ucarecdn.com/368f8b3a-8892-4c21-a985-8a882d9202c5/",
    "Ограничение максимальной скорости": "https://ucarecdn.com/34dca296-fe42-4201-9c24-9312c4ce3ccc/",
    "Дети": "https://ucarecdn.com/31c23597-e443-42a7-a604-fabe620790ea/",
    "Дорожные работы": "https://ucarecdn.com/5f4c6c41-659d-4ff2-86a3-46b130287078/",
    "Железнодорожный переезд без шлагбаума": "https://ucarecdn.com/d32e96b5-2f42-4c72-a3c6-e2e33ad15152/",
    "Пешеходный переход": "https://ucarecdn.com/7974d6b2-143c-41e2-b357-28a7b9a2c8d1/",
    "Велосипедная дорожка": "https://ucarecdn.com/20b23f45-3064-451a-9ba6-22fee46f7fd8/"
}

signs = {}

for key in sing_links.keys():
    resp = requests.get(sing_links[key], stream=True).raw
    image = np.asarray(bytearray(resp.read()), dtype="uint8")
    image = cv2.imdecode(image, cv2.IMREAD_COLOR)
    signs[key] = image
    # cv2.imshow("tst", signs[key])
    # cv2.waitKey(0)

sign_url = input()
sign_resp = requests.get(sign_url, stream=True).raw
sign = np.asarray(bytearray(sign_resp.read()), dtype="uint8")
sign = cv2.imdecode(sign, cv2.IMREAD_COLOR)

def get_SIFT_score(img, template):
    sift = cv2.ORB_create(5)

    kp_template, des_template = sift.detectAndCompute(template, None)
    kp_template1, des_template1 = sift.detectAndCompute(img, None)

    bf = cv2.BFMatcher()
    matches = bf.knnMatch(img, template, k=2)

    good_matches = []
    for m, n in matches:
        if m.distance < 0.75 * n.distance:
            matches.append(m)
    return len(good_matches)


def BFMatching(img1, img2):
    feat = cv2.ORB_create(5)

    kp1, des1 = feat.detectAndCompute(img1, None)
    kp2, des2 = feat.detectAndCompute(img2, None)

    bf = cv2.BFMatcher()
    matches = bf.knnMatch(des1, des2, k=2)
    good = []
    # matched_image = cv2.drawMatchesKnn(img1,kpnt1, img2, kpnt2, matches, None,matchColor=(0, 255, 0), matchesMask=None,singlePointColor=(255, 0, 0), flags=0)
    for m, n in matches:
        if m.distance < 0.75 * n.distance:
            good.append([m])

    # cv2.imshow("matches", matched_image)
    # cv2.waitKey(0)
    return len(good)

same_sign = "Главная дорога"
same_sign_val = 0

for key in signs.keys():
    # print(BFMatching(sign, signs[key]))
    res = BFMatching(sign, signs[key])
    if res > same_sign_val:
        same_sign_val = res
        same_sign = signs[key]
    # cv2.imshow("tst", signs[key])
    # cv2.waitKey(0)

print(same_sign)

# https://ucarecdn.com/5c4d85b9-19e7-4acd-8afc-9c8e2babf6c6/