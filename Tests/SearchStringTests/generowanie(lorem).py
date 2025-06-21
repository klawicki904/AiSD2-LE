repeated_text = (
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam imperdiet lorem at ante mattis commodo. "
    "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Praesent vel turpis "
    "eu ligula gravida convallis. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac "
    "turpis egestas. Curabitur rhoncus, ex a fringilla tincidunt, risus lorem porttitor lorem, ac eleifend velit "
    "purus ac nulla. Aenean malesuada, tortor in congue bibendum, ex ante laoreet risus, sed rutrum lacus lorem "
    "sit amet lectus.\n"
)

with open("plik.txt", "w", encoding="utf-8") as f:
    for _ in range(10000):
        f.write(repeated_text)

    f.write("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n")
    f.write("ToJestWzorzecDoSzukania\n")
