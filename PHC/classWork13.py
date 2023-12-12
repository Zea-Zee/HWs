def filterText(input_path, output_path, encoding='utf-8'):
    inp = open(input_path, "r", encoding=encoding)
    otp = open(output_path, "w", encoding=encoding)

    content = inp.read()
    filtered = ''.join(c for c in content if c.isalpha() or c.isspace())
    otp.write(filtered)

filterText("./data/text.txt", "./data/filtered.txt")