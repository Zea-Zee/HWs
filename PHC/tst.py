import cv2
import numpy as np

# Создаем черно-белое изображение (в данном случае, белый квадрат на черном фоне)
image_size = 52
black_image = np.zeros((image_size, image_size), dtype=np.uint8)
black_image[10:42, 10:42] = 255

# Инвертируем изображение
inverted_image = cv2.bitwise_not(black_image)

# Отображаем исходное и инвертированное изображение
cv2.imshow('Original Image', black_image)
cv2.imshow('Inverted Image', inverted_image)
cv2.waitKey(0)
cv2.destroyAllWindows()