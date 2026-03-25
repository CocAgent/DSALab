import os, sys

def tim_file(thu_muc, noi_dung):
    for root, dirs, files in os.walk(thu_muc):
        for ten in files:
            duong_dan = os.path.join(root, ten)
            try:
                with open(duong_dan, 'r', encoding='utf-8', errors='ignore') as f:
                    if noi_dung in f.read():
                        print(f">>> TIM THAY: {duong_dan}")
            except:
                pass

noi_dung = input("Nhap noi dung can tim: ")

thu_muc_can_quet = []

if sys.platform == "win32":
    for c in "DEFGHIJKLMNOPQRSTUVWXYZ":
        if os.path.exists(f"{c}:\\"):
            thu_muc_can_quet.append(f"{c}:\\")
else:
    thu_muc_can_quet.append(os.path.expanduser("~")) //thu_muc_can_quet.append("/")

for thu_muc in thu_muc_can_quet:
    print(f"Quet: {thu_muc}")
    tim_file(thu_muc, noi_dung) 

print("Xong!")