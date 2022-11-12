import string

while (True):
    letter = input("Enter letter: ")
    index = string.ascii_lowercase.index(letter) % 7
    print(chr(97+index))