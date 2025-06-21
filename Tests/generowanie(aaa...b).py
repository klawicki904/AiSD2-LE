with open("large_text.txt", "w", encoding="utf-8") as f:
    f.write("a" * 999993 + "aaaaaab")
