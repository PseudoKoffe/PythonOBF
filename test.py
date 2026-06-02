import sys

def check_key(user_key):
    # Password "I_Love_Coffe"
    target = [73, 95, 76, 111, 118, 101, 95, 67, 111, 102, 102, 101]
    
    if len(user_key) != len(target):
        return False
        
    for i in range(len(user_key)):
        if ord(user_key[i]) != target[i]:
            return False
            
    return True

def main():
    print("=== CrackMe by Koffe ===")
    key = input("Введите лицензионный ключ: ")
    
    if check_key(key):
        print("[+] Доступ разрешен! Отличная работа.")
    else:
        print("[-] Неверный ключ. Доступ заблокирован.")
        sys.exit(1)

if __name__ == "__main__":
    main()
